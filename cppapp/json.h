/**
 * \file   json.h
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2013-12-04
 *
 * \brief  Header file for the json class.
 */

#ifndef JSON_2HV25IX3
#define JSON_2HV25IX3


#include <string>
#include <vector>
#include <map>
#include <istream>
#include <sstream>

#include "Object.h"
#include "Lexer.h"
#include "utils.h"


namespace cppapp {


//// JSONObject /////////////////////////////////////////////////////

class JSONBoolean;
class JSONNumber;
class JSONString;


class JSONObject : public Object {
private:
	TextLoc location;

public:
	JSONObject() :
		location("<unknown>")
	{}
	
	JSONObject(TextLoc location) :
		location(location)
	{}
	
	virtual ~JSONObject() {}
	
	TextLoc getLocation() const { return location; }
	
	virtual bool isDict()   const { return false; }
	virtual bool isList()   const { return false; }
	virtual bool isBool()   const { return false; }
	virtual bool isNum()    const { return false; }
	virtual bool isString() const { return false; }
	virtual bool isNull()   const { return false; }
	virtual bool isError()  const { return false; }
	
	virtual int getSize() const { return 0; }
	
	virtual bool            hasItem(Ref<JSONObject> key) { return false; }
	virtual Ref<JSONObject> getItem(std::string key);
	virtual Ref<JSONObject> getItem(int key);
	virtual void            setItem(std::string key, Ref<JSONObject> value);
	virtual void            setItem(int key, Ref<JSONObject> value);
	
	virtual Ref<JSONObject> getItem(Ref<JSONObject> key);
	virtual void            setItem(Ref<JSONObject> key, Ref<JSONObject> value);
	
	virtual bool equals(Ref<JSONObject> other) const;
	
	virtual Ref<JSONBoolean> toBool();
	virtual Ref<JSONNumber>  toNum();
	virtual Ref<JSONString>  toString();
	
	virtual bool        getBool() const;
	virtual int         getInt() const;
	virtual double      getDouble() const;
	virtual std::string getString() const;
};


//// JSONDict ///////////////////////////////////////////////////////

class JSONString;


class JSONDict : public JSONObject {
private:
	std::map<std::string, Ref<JSONObject> > _values;

public:
	JSONDict(TextLoc loc) :
		JSONObject(loc)
	{}
	
	virtual ~JSONDict() { _values.clear(); }
	
	virtual bool isDict() const { return true; }

	int getSize() { return _values.size(); }

	void set(Ref<JSONString> key, Ref<JSONObject> value);
	bool hasKey(Ref<JSONString> key);
	Ref<JSONObject> get(Ref<JSONString> key, Ref<JSONObject> deflt);
};


//// JSONList ///////////////////////////////////////////////////////

class JSONList : public JSONObject {
private:
	std::vector<Ref<JSONObject> > _values;

public:
	JSONList(TextLoc loc) :
		JSONObject(loc)
	{}
	
	virtual ~JSONList() { _values.clear(); }
	
	virtual bool isList() const { return true; }
	
	int getSize() { return _values.size(); }
	
	void append(Ref<JSONObject> obj) { _values.push_back(obj); }
	Ref<JSONObject> get(int index);
};


//// JSONScalar /////////////////////////////////////////////////////

template<class T>
class JSONScalar : public JSONObject {
private:
	T _value;

public:
	JSONScalar(TextLoc loc, T value) :
		JSONObject(loc), _value(value)
	{}
	
	T getValue() const { return _value; }
	void setValue(T value) { _value = value; }
};


//// JSONBoolean ////////////////////////////////////////////////////

class JSONBoolean : public JSONScalar<bool> {
public:
	JSONBoolean(TextLoc loc, bool value) :
		JSONScalar(loc, value)
	{}

	virtual bool isBool() const { return true; }
};


//// JSONNumber /////////////////////////////////////////////////////

class JSONNumber : public JSONScalar<double> {
public:
	JSONNumber(TextLoc loc, double value) :
		JSONScalar(loc, value)
	{}
	
	virtual bool isNum() const { return true; }
};


//// JSONString /////////////////////////////////////////////////////

class JSONString : public JSONScalar<std::string> {
public:
	JSONString(TextLoc loc, std::string value) :
		JSONScalar(loc, value)
	{}
	
	virtual bool isString() const { return true; }
};


//// JSONNull ///////////////////////////////////////////////////////

class JSONNull : public JSONObject {
public:
	virtual bool isNull() const { return true; }
	
	static Ref<JSONNull> getInstance();
};


//// JSONError //////////////////////////////////////////////////////

class JSONError : public JSONObject {
private:
	std::string message_;
	TextLoc     errorLoc_;

public:
	JSONError(TextLoc loc, std::string message, TextLoc errorLoc) :
		JSONObject(loc), message_(message)
	{}
	
	virtual bool isError() const { return true; }

	virtual Ref<JSONString> toString();
};


//// JSONParser /////////////////////////////////////////////////////

class JSONParser {
private:
	Lexer lexer;
	
	Ref<JSONError> returnError(const char *fn, int line);
	
	bool readObject(Ref<JSONObject> *result);
	
	bool readDict(Ref<JSONObject> *result);
	bool readKeyValue(Ref<JSONString> *key, Ref<JSONObject> *value);
	
	bool readList(Ref<JSONObject> *result);
	bool readString(Ref<JSONObject> *result);
	bool readNumber(Ref<JSONObject> *result);
	bool readBool(Ref<JSONObject> *result);

public:
	Ref<JSONObject> parse(std::istream *input, std::string fileName);
};


} // namespace cppapp


#endif /* end of include guard: JSON_2HV25IX3 */

