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
	
	virtual bool            hasItem(std::string key) { return false; }
	virtual bool            hasItem(int index)       { return false; }
	virtual Ref<JSONObject> getItem(std::string key, Ref<JSONObject> deflt);
	virtual Ref<JSONObject> getItem(std::string key) { return getItem(key, NULL); }
	virtual Ref<JSONObject> getItem(int key);
	virtual void            setItem(std::string key, Ref<JSONObject> value);
	virtual void            setItem(int key, Ref<JSONObject> value) {}
	
	virtual bool            hasItem(Ref<JSONObject> key);
	virtual Ref<JSONObject> getItem(Ref<JSONObject> key);
	virtual void            setItem(Ref<JSONObject> key, Ref<JSONObject> value);
	
	virtual bool equals(Ref<JSONObject> other) const;
	
	virtual Ref<JSONBoolean> toBool();
	virtual Ref<JSONNumber>  toNum();
	virtual Ref<JSONString>  toString();
	
	virtual bool        getBool() const   { return true; }
	virtual double      getDouble() const { return 0; }
	virtual int         getInt() const    { return (int)getDouble(); }
	virtual std::string getString() const { return "JSONObject"; }
};


//// JSONDict ///////////////////////////////////////////////////////

class JSONString;


class JSONDict : public JSONObject {
public:
	typedef std::map<std::string, Ref<JSONObject> > Map;

private:
	Map _values;

public:
	JSONDict(TextLoc loc) :
		JSONObject(loc)
	{}
	
	virtual ~JSONDict() { _values.clear(); }
	
	virtual bool isDict() const { return true; }
	
	virtual int getSize() const { return _values.size(); }
	
	virtual bool hasItem(std::string key);
	virtual Ref<JSONObject> getItem(std::string key, Ref<JSONObject> deflt);
	virtual void setItem(std::string key, Ref<JSONObject> value);
	
	void update(Ref<JSONDict> dict)
	{
		FOR_EACH(*dict, it) {
			setItem(it->first, it->second);
		}
	}
	
	Map::iterator begin() { return _values.begin(); }
	Map::iterator end()   { return _values.end(); }
	Map::const_iterator begin() const { return _values.begin(); }
	Map::const_iterator end()   const { return _values.end(); }
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
	
	virtual int getSize() const { return _values.size(); }
	
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
	
	virtual Ref<JSONBoolean> toBool() { return this; }
	
	virtual bool        getBool()   const { return getValue(); }
	virtual double      getDouble() const { return getValue() ? 1 : 0; }
	virtual std::string getString() const { return getValue() ? "true" : "false"; }
	
	static bool parse(Lexer *lexer, bool *result);
	static bool parse(std::string str, bool *result);
};


//// JSONNumber /////////////////////////////////////////////////////

class JSONNumber : public JSONScalar<double> {
public:
	JSONNumber(TextLoc loc, double value) :
		JSONScalar(loc, value)
	{}
	
	virtual bool isNum() const { return true; }

	virtual Ref<JSONNumber> toNum() { return this; }

	virtual bool   getBool()   const { return getValue() != 0.0; }
	virtual double getDouble() const { return getValue(); }
	virtual std::string getString() const {
		std::ostringstream out;
		out << getValue();
		return out.str();
	}
	
	static bool parse(Lexer *lexer, double *result);
	static bool parse(std::string str, double *result);
};


//// JSONString /////////////////////////////////////////////////////

class JSONString : public JSONScalar<std::string> {
public:
	JSONString(TextLoc loc, std::string value) :
		JSONScalar(loc, value)
	{}
	
	virtual bool isString() const { return true; }
	
	virtual Ref<JSONString> toString() { return this; }
	
	virtual bool getBool() const;
	virtual double getDouble() const;
	virtual std::string getString() const { return getValue(); }
};


//// JSONNull ///////////////////////////////////////////////////////

class JSONNull : public JSONObject {
public:
	virtual bool isNull() const { return true; }
	
	virtual bool getBool() const { return false; }
	virtual double getDouble() const { return 0.0; }
	virtual std::string getString() const { return "null"; }
	
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
	
	virtual bool getBool() const { return false; }
	virtual std::string getString() const
	{
		std::ostringstream out;
		out << message_;
		out << " (" << getLocation().fileName << ":" << getLocation().line << ")";
		return out.str();
	}
};


#define JSON_MAKE_ERROR(msg) new JSONError( \
	TextLoc(__FILE__, __LINE__),      \
	(msg),                            \
	TextLoc(__FILE__, __LINE__)       \
);


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
	
	static bool strToBool(Lexer *lexer, double *result);
	static bool strToDouble(Lexer *lexer, double *result);
};


} // namespace cppapp


#endif /* end of include guard: JSON_2HV25IX3 */

