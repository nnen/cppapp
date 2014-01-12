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
// JSONParser class
////////////////////////////////////////////////////////////////////////////////


Ref<DynError> JSONParser::returnError(const char *fn, int line)
{
	std::ostringstream ss;
	ss << "Unexpected character: ";
	if (lexer.peek() == -1)
		ss << "EOF";
	else
		ss.put(lexer.peek());
	
	return new DynError(
		TextLoc(fn, line),
		ss.str(),
		lexer.getLocation()
		// "Unexpected character."
	);
}


void JSONParser::skipWhitespace()
{
	while (true) {
		lexer.skipWhitespace();
		
		if (!lexer.read("//"))
			break;
		
		while (lexer.peek() != '\n')
			lexer.read();
	}
}


bool JSONParser::readObject(Ref<DynObject> *result)
{
	skipWhitespace();
	
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

	if (readNull(result))
		return true;
	
	return false;
}


bool JSONParser::readDict(Ref<DynObject> *result)
{
	if (!lexer.read('{'))
		return false;
	
	Ref<DynString> key;
	Ref<DynObject> value;
	Ref<DynObject> dict = new DynDict(lexer.getLocation());
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


bool JSONParser::readKeyValue(Ref<DynString> *key, Ref<DynObject> *value)
{
	Ref<DynObject> k;
	
	if (!(readString(&k) || readKeyword(&k)))
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


bool JSONParser::readList(Ref<DynObject> *result)
{
	if (!lexer.read('['))
		return false;
	
	Ref<DynObject> item;
	Ref<DynList> list = new DynList(lexer.getLocation());
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


bool JSONParser::readString(Ref<DynObject> *result)
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
			*result = new DynString(loc, oss.str());
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


bool JSONParser::readKeyword(Ref<DynObject> *result)
{
	skipWhitespace();
	
	TextLoc loc = lexer.getLocation();
	std::ostringstream oss;
	
	if (!(isalpha(lexer.peek()) || lexer.peek() == '_'))
		return false;
	
	while (isalnum(lexer.peek()) || lexer.peek() == '_') {
		oss.put(lexer.read());
	}
	
	*result = new DynString(loc, oss.str());
	
	return true;
}


bool JSONParser::readNumber(Ref<DynObject> *result)
{
	if (!(isdigit(lexer.peek()) || lexer.peek() == '-'))
		return false;
	
	TextLoc loc = lexer.getLocation();
	
	double value = 0.0;
	DynNumber::parse(&lexer, &value);
	
	*result = new DynNumber(loc, value);
	return true;
}


bool JSONParser::readBool(Ref<DynObject> *result)
{
	TextLoc loc = lexer.getLocation();
	
	bool value;
	if (!DynBoolean::parse(&lexer, &value))
		return false;
	
	*result = new DynBoolean(loc, value);
	return true;
}


bool JSONParser::readNull(Ref<DynObject> *result)
{
	TextLoc loc = lexer.getLocation();
	
	if (!lexer.read("null"))
		return false;
	
	*result = DynNull::getInstance();
	return true;
}


Ref<DynObject> JSONParser::parse(std::istream *input, std::string fileName)
{
	return parse(new StreamInput(fileName, input));
}


Ref<DynObject> JSONParser::parse(Ref<Input> input)
{
	lexer.input(input);
	
	Ref<DynObject> result;
	if (!readObject(&result))
		result = ERROR;
	
	return result;
}


Ref<DynObject> JSONParser::parse(std::string input)
{
	return parse(new StreamInput(input));
}


} // namespace cppapp


