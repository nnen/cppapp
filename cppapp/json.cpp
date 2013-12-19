/**
 * \file   json.cpp
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2013-12-04
 * 
 * \brief  Implementation file for the json class.
 */

#include "json.h"


namespace cppapp {


#define ERROR returnError(__FILE__, __LINE__)


////////////////////////////////////////////////////////////////////////////////
// JSONObject class
////////////////////////////////////////////////////////////////////////////////


Ref<JSONObject> JSONObject::getItem(std::string key, Ref<JSONObject> deflt)
{
	return JSON_MAKE_ERROR("Key error.");
}


Ref<JSONObject> JSONObject::getItem(int key)
{
	return JSON_MAKE_ERROR("Index error.");
}


void JSONObject::setItem(std::string key, Ref<JSONObject> value)
{
}


bool JSONObject::hasItem(Ref<JSONObject> key)
{
	if (key->isString()) {
		return hasItem(key->getString());
	} else if (key->isNum()) {
		return hasItem(key->getInt());
	}
	
	return false;
}


Ref<JSONObject> JSONObject::getItem(Ref<JSONObject> key)
{
	if (key->isString()) {
		return getItem(key->getString());
	} else if (key->isNum()) {
		return getItem(key->getInt());
	}
	
	return JSON_MAKE_ERROR("Key error.");
}


void JSONObject::setItem(Ref<JSONObject> key, Ref<JSONObject> value)
{
	if (key->isNum()) {
		setItem(key->getInt(), value);
	} else if (key->isString()) {
		setItem(key->getString(), value);
	}
}


bool JSONObject::equals(Ref<JSONObject> other) const
{
	if (this == other.getPtr())
		return true;
	return false;
}


Ref<JSONBoolean> JSONObject::toBool()
{
	return new JSONBoolean(getLocation(), getBool());
}


Ref<JSONNumber> JSONObject::toNum()
{
	return new JSONNumber(getLocation(), getDouble());
}


Ref<JSONString> JSONObject::toString()
{
	return new JSONString(getLocation(), getString());
}


////////////////////////////////////////////////////////////////////////////////
// JSONDict class
////////////////////////////////////////////////////////////////////////////////


bool JSONDict::hasItem(std::string key)
{
	VAR(found, _values.find(key));
	if (found == _values.end())
		return false;
	return true;
}


Ref<JSONObject> JSONDict::getItem(std::string key, Ref<JSONObject> deflt)
{
	VAR(found, _values.find(key));
	if (found == _values.end()) {
		return deflt;
	}
	return found->second;
}


void JSONDict::setItem(std::string key, Ref<JSONObject> value)
{
	_values[key] = value;
}


////////////////////////////////////////////////////////////////////////////////
// JSONList class
////////////////////////////////////////////////////////////////////////////////


Ref<JSONObject> JSONList::get(int index)
{
	if ((index < 0) || (index >= (int)_values.size())) {
		return JSONNull::getInstance();
	}
	
	return _values[index];
}


////////////////////////////////////////////////////////////////////////////////
// JSONBoolean class
////////////////////////////////////////////////////////////////////////////////


bool JSONBoolean::parse(Lexer *lexer, bool *result)
{
	lexer->skipWhitespace();
	
	if (lexer->read("true") || lexer->read("True")) {
		*result = true;
		return true;
	} else if (lexer->read("false") || lexer->read("False")) {
		*result = false;
		return true;
	}
	
	return false;
}


bool JSONBoolean::parse(std::string str, bool *result)
{
	std::istringstream in(str);
	Lexer lexer;
	lexer.input(&in, "<string>");
	
	if (!parse(&lexer, result))
		return false;
	
	lexer.skipWhitespace();
	return (lexer.peek() < 0);
}


////////////////////////////////////////////////////////////////////////////////
// JSONNumber class
////////////////////////////////////////////////////////////////////////////////


bool JSONNumber::parse(Lexer *lexer, double *result)
{
	*result = 0.0;
	
	lexer->skipWhitespace();
	
	if (!isdigit(lexer->peek()))
		return false;
	
	while (isdigit(lexer->peek())) {
		*result *= 10.0;
		*result += (double)(lexer->read() - '0');
	}
	
	if (lexer->read('.')) {
		double place = 0.1;
		
		while (isdigit(lexer->peek())) {
			*result += (double)(lexer->read() - '0') * place;
			place *= 0.1;
		}
	}
	
	return true;
}


bool JSONNumber::parse(std::string str, double *result)
{
	std::istringstream in(str);
	Lexer lexer;
	lexer.input(&in, "<string>");
	
	if (!parse(&lexer, result))
		return false;
	
	lexer.skipWhitespace();
	return (lexer.peek() < 0);
}


////////////////////////////////////////////////////////////////////////////////
// JSONString class
////////////////////////////////////////////////////////////////////////////////


bool JSONString::getBool() const
{
	bool result;
	if (JSONBoolean::parse(getValue(), &result))
		return result;
	return true;
}


double JSONString::getDouble() const
{
	double result;
	if (JSONNumber::parse(getValue(), &result))
		return result;
	return 0.0;
}


////////////////////////////////////////////////////////////////////////////////
// JSONNull class
////////////////////////////////////////////////////////////////////////////////


Ref<JSONNull> JSONNull::getInstance()
{
	static Ref<JSONNull> instance;

	if (instance.isNull())
		instance = new JSONNull();
	
	return instance;
}


////////////////////////////////////////////////////////////////////////////////
// JSONError class
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// JSONParser class
////////////////////////////////////////////////////////////////////////////////


Ref<JSONError> JSONParser::returnError(const char *fn, int line)
{
	std::ostringstream ss;
	ss << "Unexpected character: ";
	if (lexer.peek() == -1)
		ss << "EOF";
	else
		ss.put(lexer.peek());
	
	return new JSONError(
		TextLoc(fn, line),
		ss.str(),
		lexer.getLocation()
		// "Unexpected character."
	);
}


bool JSONParser::readObject(Ref<JSONObject> *result)
{
	lexer.skipWhitespace();
	
	if (readDict(result))
		return true;
	
	if (readList(result))
		return true;
	
	if (readString(result))
		return true;
	
	if (readNumber(result))
		return true;
	
	if (readBool(result))
		return true;
	
	return false;
}


bool JSONParser::readDict(Ref<JSONObject> *result)
{
	if (!lexer.read('{'))
		return false;
	
	Ref<JSONString> key;
	Ref<JSONObject> value;
	Ref<JSONObject> dict = new JSONDict(lexer.getLocation());
	*result = dict;
	
	while(lexer.read(','));
	if (lexer.read('}'))
		return true;
	
	while (true) {
		if (!readKeyValue(&key, &value))
			break;

		if (value->isError()) {
			*result = value;
			return true;
		}
		
		dict->setItem(key, value);
		
		if (!lexer.read(','))
			break;
		while (lexer.read(','));
	}

	if (!lexer.read('}')) {
		*result = ERROR;
		return true;
	}
	
	return true;
}


bool JSONParser::readKeyValue(Ref<JSONString> *key, Ref<JSONObject> *value)
{
	Ref<JSONObject> k;
	
	if (!readString(&k))
		return false;
	
	if (k->isError()) {
		*value = k;
		return true;
	}

	*key = k;
	
	if (!lexer.read(':')) {
		*value = ERROR;
		return true;
	}
	
	if (!readObject(value)) {
		*value = ERROR;
		return true;
	}
	
	return true;
}


bool JSONParser::readList(Ref<JSONObject> *result)
{
	if (!lexer.read('['))
		return false;
	
	Ref<JSONObject> item;
	Ref<JSONList> list = new JSONList(lexer.getLocation());
	*result = list;
	
	while (lexer.read(','));
	if (lexer.read(']'))
		return true;
	
	while (true) {
		if (!readObject(&item))
			break;

		if (item->isError()) {
			*result = item;
			return true;
		}

		list->append(item);
		
		if (!lexer.read(','))
			break;
		while (lexer.read(','));
	}
	
	if (!lexer.read(']')) {
		*result = ERROR;
		return true;
	}
	
	return true;
}


bool JSONParser::readString(Ref<JSONObject> *result)
{
	if (!lexer.read('"'))
		return false;
	
	TextLoc loc = lexer.getLocation();
	bool escape = false;
	std::ostringstream oss;
	
	while (true) {
		if (escape) {
			escape = false;
			oss.put(lexer.read());
			continue;
		}
		
		switch (lexer.peek()) {
		case '"':
			lexer.read();
			*result = new JSONString(loc, oss.str());
			return true;
		
		case '\\':
			lexer.read();
			escape = true;
			break;
		
		case -1:
			*result = ERROR;
			return true;
		
		default:
			oss.put(lexer.read());
			break;
		}
	}
	
	return true;
}


bool JSONParser::readNumber(Ref<JSONObject> *result)
{
	if (!isdigit(lexer.peek()))
		return false;
	
	TextLoc loc = lexer.getLocation();
	
	double value = 0.0;
	JSONNumber::parse(&lexer, &value);
	
	*result = new JSONNumber(loc, value);
	return true;
}


bool JSONParser::readBool(Ref<JSONObject> *result)
{
	TextLoc loc = lexer.getLocation();
	
	bool value;
	if (!JSONBoolean::parse(&lexer, &value))
		return false;
	
	*result = new JSONBoolean(loc, value);
	return true;
}


Ref<JSONObject> JSONParser::parse(std::istream *input, std::string fileName)
{
	lexer.input(input, fileName);
	
	Ref<JSONObject> result;
	if (!readObject(&result))
		result = ERROR;
	
	return result;
}


} // namespace cppapp


