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


Ref<JSONObject> JSONObject::getItem(std::string key)
{
	return getItem(new JSONString(TextLoc(), key));
}


Ref<JSONObject> JSONObject::getItem(int key)
{
	return getItem(new JSONNumber(TextLoc(), key));
}


void JSONObject::setItem(std::string key, Ref<JSONObject> value)
{
}


void JSONObject::setItem(int key, Ref<JSONObject> value)
{
}


Ref<JSONObject> JSONObject::getItem(Ref<JSONObject> key)
{
	return JSONNull::getInstance();
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
	return new JSONBoolean(getLocation(), true);
}


Ref<JSONNumber> JSONObject::toNum()
{
	return new JSONNumber(getLocation(), 0);
}


Ref<JSONString> JSONObject::toString()
{
	return new JSONString(getLocation(), "JSONObject");
}


////////////////////////////////////////////////////////////////////////////////
// JSONDict class
////////////////////////////////////////////////////////////////////////////////


void JSONDict::set(Ref<JSONString> key, Ref<JSONObject> value)
{
	_values[key->getValue()] = value;
}


bool JSONDict::hasKey(Ref<JSONString> key)
{
	VAR(found, _values.find(key->getValue()));
	if (found == _values.end())
		return false;
	return true;
}


Ref<JSONObject> JSONDict::get(Ref<JSONString> key, Ref<JSONObject> deflt)
{
	VAR(found, _values.find(key->getValue()));
	if (found == _values.end()) {
		return deflt;
	}
	return found->second;
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


Ref<JSONString> JSONError::toString()
{
	std::ostringstream ss;
	ss << message_;
	ss << " (" << getLocation().fileName << ":" << getLocation().line << ")";
	return new JSONString(TextLoc(), ss.str());
}


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
	Ref<JSONDict>   dict = new JSONDict(lexer.getLocation());
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
		
		dict->set(key, value);
		
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
	
	while (isdigit(lexer.peek())) {
		value *= 10.0;
		value += (double)(lexer.read() - '0');
	}

	if (lexer.read('.')) {
		double place = 0.1;
		
		while (isdigit(lexer.peek())) {
			value += (double)(lexer.read() - '0') * place;
			place *= 0.1;
		}
	}
	
	*result = new JSONNumber(loc, value);
	return true;
}


bool JSONParser::readBool(Ref<JSONObject> *result)
{
	TextLoc loc = lexer.getLocation();
	
	if (lexer.read("true") || lexer.read("True")) {
		*result = new JSONBoolean(loc, true);
		return true;
	} else if (lexer.read("false") || lexer.read("False")) {
		*result = new JSONBoolean(loc, false);
		return true;
	}
	
	return false;
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


