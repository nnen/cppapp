/**
 * \file   DynObject.cpp
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2013-12-04
 * 
 * \brief  Implementation file for the json class.
 */

#include "DynObject.h"


namespace cppapp {


#define ERROR returnError(__FILE__, __LINE__)


////////////////////////////////////////////////////////////////////////////////
// DynObject class
////////////////////////////////////////////////////////////////////////////////


Ref<DynObject> DynObject::getStrItem(std::string key, Ref<DynObject> deflt)
{
	return DYN_MAKE_ERROR("Key error.");
}


Ref<DynObject> DynObject::getIntItem(int key)
{
	return DYN_MAKE_ERROR("Index error.");
}


bool DynObject::hasItem(Ref<DynObject> key)
{
	if (key->isString()) {
		return hasStrItem(key->getString());
	} else if (key->isNum()) {
		return hasIntItem(key->getInt());
	}
	
	return false;
}


Ref<DynObject> DynObject::getItem(Ref<DynObject> key)
{
	if (key->isString()) {
		return getStrItem(key->getString());
	} else if (key->isNum()) {
		return getIntItem(key->getInt());
	}
	
	return DYN_MAKE_ERROR("Key error.");
}


void DynObject::setItem(Ref<DynObject> key, Ref<DynObject> value)
{
	if (key->isNum()) {
		setIntItem(key->getInt(), value);
	} else if (key->isString()) {
		setStrItem(key->getString(), value);
	}
}


Ref<DynObject> DynObject::getDottedItem(std::string key)
{
	std::vector<std::string> nameParts;
	Strings::split(key, ".", &nameParts);
	
	Ref<DynObject> result = this;
	
	for (int i = 0; i < nameParts.size(); i++) {
		result = result->getStrItem(nameParts[i]);
		if (result->isError())
			return result;
	}
	
	return result;
}


void DynObject::setDottedItem(std::string key, Ref<DynObject> value)
{
	std::vector<std::string> nameParts;
	Strings::split(key, ".", &nameParts);

	Ref<DynObject> target = this;
	
	int i = 0;
	for (; i < nameParts.size() - 1; i++) {
		target = target->getStrItem(nameParts[i]);
		if (target->isError())
			return;
	}
	
	target->setStrItem(nameParts[i], value);
}


bool DynObject::equals(Ref<DynObject> other) const
{
	if (this == other.getPtr())
		return true;
	return false;
}


Ref<DynBoolean> DynObject::toBool()
{
	return new DynBoolean(getLocation(), getBool());
}


Ref<DynNumber> DynObject::toNum()
{
	return new DynNumber(getLocation(), getDouble());
}


Ref<DynString> DynObject::toString()
{
	return new DynString(getLocation(), getString());
}


////////////////////////////////////////////////////////////////////////////////
// DynDict class
////////////////////////////////////////////////////////////////////////////////


bool DynDict::hasStrItem(std::string key)
{
	VAR(found, _values.find(key));
	if (found == _values.end())
		return false;
	return true;
}


Ref<DynObject> DynDict::getStrItem(std::string key, Ref<DynObject> deflt)
{
	VAR(found, _values.find(key));
	if (found == _values.end()) {
		return deflt;
	}
	return found->second;
}


void DynDict::setStrItem(std::string key, Ref<DynObject> value)
{
	_values[key] = value;
}


////////////////////////////////////////////////////////////////////////////////
// DynList class
////////////////////////////////////////////////////////////////////////////////


bool DynList::hasIntItem(int index)
{
	if ((index < 0) || (index >= _values.size()))
		return false;
	return true;
}


Ref<DynObject> DynList::getIntItem(int key)
{
	if ((key < 0) || (key >= _values.size()))
		return DYN_MAKE_ERROR("");
	
	return _values[key];
}


void DynList::setIntItem(int key, Ref<DynObject> value)
{
	if ((key < 0) || (key >= _values.size()))
		return;

	_values[key] = value;
}


Ref<DynObject> DynList::get(int index)
{
	if ((index < 0) || (index >= (int)_values.size())) {
		return DynNull::getInstance();
	}
	
	return _values[index];
}


////////////////////////////////////////////////////////////////////////////////
// DynBoolean class
////////////////////////////////////////////////////////////////////////////////


bool DynBoolean::parse(Lexer *lexer, bool *result)
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


bool DynBoolean::parse(std::string str, bool *result)
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
// DynNumber class
////////////////////////////////////////////////////////////////////////////////


bool DynNumber::parse(Lexer *lexer, double *result)
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


bool DynNumber::parse(std::string str, double *result)
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
// DynString class
////////////////////////////////////////////////////////////////////////////////


bool DynString::getBool() const
{
	bool result;
	if (DynBoolean::parse(getValue(), &result))
		return result;
	return true;
}


double DynString::getDouble() const
{
	double result;
	if (DynNumber::parse(getValue(), &result))
		return result;
	return 0.0;
}


////////////////////////////////////////////////////////////////////////////////
// DynNull class
////////////////////////////////////////////////////////////////////////////////


Ref<DynNull> DynNull::getInstance()
{
	static Ref<DynNull> instance;

	if (instance.isNull())
		instance = new DynNull();
	
	return instance;
}


////////////////////////////////////////////////////////////////////////////////
// DynError class
////////////////////////////////////////////////////////////////////////////////


} // namespace cppapp


