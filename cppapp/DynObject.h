/**
 * \file   DynObject.h
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2013-12-04
 *
 * \brief  Header file for the json class.
 */

#ifndef Dyn_2HV25IX3
#define Dyn_2HV25IX3


#include <string>
#include <vector>
#include <map>
#include <istream>
#include <sstream>

#include "Object.h"
#include "TextLoc.h"
#include "Lexer.h"
#include "Logger.h"
#include "PrettyPrinter.h"
#include "utils.h"
#include "string_utils.h"


namespace cppapp {


/** \addtogroup dynamic
 *
 * \brief Dynamic object system.
 *
 * @{
 */


//// DynObject //////////////////////////////////////////////////////

class DynBoolean;
class DynNumber;
class DynString;


class DynObject : public Object {
private:
	TextLoc location;

public:
	DynObject() :
		location("<unknown>")
	{}
	
	DynObject(TextLoc location) :
		location(location)
	{}
	
	virtual ~DynObject() {}
	
	TextLoc getLocation() const { return location; }
	
	virtual void print(Ref<PrettyPrinter> printer, int level = 0);
	
	virtual bool isDict()   const { return false; }
	virtual bool isList()   const { return false; }
	virtual bool isBool()   const { return false; }
	virtual bool isNum()    const { return false; }
	virtual bool isString() const { return false; }
	virtual bool isNull()   const { return false; }
	virtual bool isError()  const { return false; }
	
	virtual int getSize() const { return 0; }
	
	/**
	 * \name Subscription Protocol
	 */
	///@{
	virtual bool            hasStrItem(std::string key) { return false; }
	virtual Ref<DynObject>  getStrItem(std::string key, Ref<DynObject> deflt);
	virtual Ref<DynObject>  getStrItem(std::string key) { return getStrItem(key, NULL); }
	virtual void            setStrItem(std::string key, Ref<DynObject> value) {}
	
	virtual bool            hasIntItem(int index)       { return false; }
	virtual Ref<DynObject>  getIntItem(int key);
	virtual void            setIntItem(int key, Ref<DynObject> value) {}
	
	virtual bool            hasItem(Ref<DynObject> key);
	virtual Ref<DynObject>  getItem(Ref<DynObject> key);
	virtual void            setItem(Ref<DynObject> key, Ref<DynObject> value);
	
	virtual Ref<DynObject>  getDottedItem(std::string key);
	virtual void            setDottedItem(std::string key, Ref<DynObject> value);
	
	virtual bool            getStrBool(std::string key, bool defaultValue);
	virtual int             getStrInt(std::string key, int defaultValue);
	virtual double          getStrDouble(std::string key, double defaultValue);
	virtual std::string     getStrString(std::string key, std::string defaultValue);
	
	virtual Ref<DynObject>  getKeys();
	///@}
	
	virtual bool equals(Ref<DynObject> other) const;
	
	/**
	 * \name Iteration Protocol
	 */
	///@{
	virtual Ref<DynObject> getIterator() { return this; }
	virtual Ref<DynObject> getNext();
	///@}
	
	virtual Ref<DynBoolean> toBool();
	virtual Ref<DynNumber>  toNum();
	virtual Ref<DynString>  toString();
	
	/**
	 * \name C Type Conversion
	 */
	///@{
	virtual bool        getBool() const   { return true; }
	virtual double      getDouble() const { return 0; }
	virtual int         getInt() const    { return (int)getDouble(); }
	virtual std::string getString() const { return "DynObject"; }
	
	operator bool()        { return getBool(); }
	operator double()      { return getDouble(); }
	operator int()         { return getInt(); }
	operator std::string() { return getString(); }
	///@}
};


#define DYN_FOR_EACH(it_name__, collection__) \
	for ( \
		Ref<DynObject> iterator__ = collection__->getIterator(), \
		it_name__ = iterator__->getNext(); \
		!it_name__->isError(); \
		it_name__ = iterator__->getNext() \
	)


//// DynDict ////////////////////////////////////////////////////////

class DynString;


class DynDict : public DynObject {
public:
	typedef std::map<std::string, Ref<DynObject> > Map;

private:
	Map _values;

public:
	DynDict(TextLoc loc) :
		DynObject(loc)
	{}
	
	virtual ~DynDict() { _values.clear(); }
	
	virtual bool isDict() const { return true; }
	
	virtual int getSize() const { return _values.size(); }
	
	virtual void print(Ref<PrettyPrinter> printer, int level = 0);
	
	virtual bool           hasStrItem(std::string key);
	virtual Ref<DynObject> getStrItem(std::string key, Ref<DynObject> deflt);
	virtual void           setStrItem(std::string key, Ref<DynObject> value);
	
	virtual Ref<DynObject> getKeys();
	
	void update(Ref<DynDict> dict)
	{
		FOR_EACH(*dict, it) {
			setStrItem(it->first, it->second);
		}
	}
	
	Map::iterator begin() { return _values.begin(); }
	Map::iterator end()   { return _values.end(); }
	Map::const_iterator begin() const { return _values.begin(); }
	Map::const_iterator end()   const { return _values.end(); }
};


//// DynList ////////////////////////////////////////////////////////

class DynList : public DynObject {
public:
	typedef std::vector<Ref<DynObject> > Vector;

private:
	std::vector<Ref<DynObject> > _values;

public:
	DynList(TextLoc loc) :
		DynObject(loc)
	{}
	
	virtual ~DynList() { _values.clear(); }
	
	virtual bool isList() const { return true; }
	
	virtual int getSize() const { return _values.size(); }
	
	virtual void print(Ref<PrettyPrinter> printer, int level = 0);
	
	virtual bool            hasIntItem(int index);
	virtual Ref<DynObject>  getIntItem(int key);
	virtual void            setIntItem(int key, Ref<DynObject> value);

	virtual Ref<DynObject> getIterator();
	
	void append(Ref<DynObject> obj) { _values.push_back(obj); }
	
	Vector::iterator begin() { return _values.begin(); }
	Vector::iterator end()   { return _values.end(); }
};


class DynListIter : public DynObject {
private:
	bool                      started_;
	DynList::Vector::iterator iterator_;
	Ref<DynList>              list_;

public:
	DynListIter(Ref<DynList> list) :
		started_(false),
		list_(list)
	{ }
	
	virtual Ref<DynObject> getNext();
};


#define DYN_NEW_LIST (new cppapp::DynList(TextLoc(__FILE__, __LINE__)))


//// DynScalar //////////////////////////////////////////////////////

template<class T>
class DynScalar : public DynObject {
private:
	T _value;

public:
	DynScalar(TextLoc loc, T value) :
		DynObject(loc), _value(value)
	{}
	
	T getValue() const { return _value; }
	void setValue(T value) { _value = value; }
};


//// DynBoolean /////////////////////////////////////////////////////

class DynBoolean : public DynScalar<bool> {
public:
	DynBoolean(TextLoc loc, bool value) :
		DynScalar<bool>(loc, value)
	{}
	
	virtual void print(Ref<PrettyPrinter> printer, int level = 0);
	
	virtual bool isBool() const { return true; }
	
	virtual Ref<DynBoolean> toBool() { return this; }
	
	virtual bool        getBool()   const { return getValue(); }
	virtual double      getDouble() const { return getValue() ? 1 : 0; }
	virtual std::string getString() const { return getValue() ? "true" : "false"; }
	
	static bool parse(Lexer *lexer, bool *result);
	static bool parse(std::string str, bool *result);
};


//// DynNumber //////////////////////////////////////////////////////

class DynNumber : public DynScalar<double> {
public:
	DynNumber(TextLoc loc, double value) :
		DynScalar<double>(loc, value)
	{}
	
	virtual void print(Ref<PrettyPrinter> printer, int level = 0);
	
	virtual bool isNum() const { return true; }

	virtual Ref<DynNumber> toNum() { return this; }

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


//// DynString //////////////////////////////////////////////////////

class DynString : public DynScalar<std::string> {
public:
	DynString(TextLoc loc, std::string value) :
		DynScalar<std::string>(loc, value)
	{}
	
	virtual void print(Ref<PrettyPrinter> printer, int level = 0);
	
	virtual bool isString() const { return true; }
	
	virtual Ref<DynString> toString() { return this; }
	
	virtual bool getBool() const;
	virtual double getDouble() const;
	virtual std::string getString() const { return getValue(); }
};


#define DYN_NEW_STRING(value) (new cppapp::DynString(TextLoc(__FILE__, __LINE__), (value)))


//// DynNull ////////////////////////////////////////////////////////

class DynNull : public DynObject {
public:
	virtual bool isNull() const { return true; }
	
	virtual bool getBool() const { return false; }
	virtual double getDouble() const { return 0.0; }
	virtual std::string getString() const { return "null"; }
	
	static Ref<DynNull> getInstance();
};


//// DynError ///////////////////////////////////////////////////////

class DynError : public DynObject {
private:
	std::string message_;
	TextLoc     errorLoc_;

public:
	DynError(TextLoc loc, std::string message, TextLoc errorLoc) :
		DynObject(loc), message_(message), errorLoc_(errorLoc)
	{}
	
	virtual bool isError() const { return true; }
	virtual TextLoc getErrorLoc() const { return errorLoc_; }
	
	virtual bool getBool() const { return false; }
	virtual std::string getString() const
	{
		std::ostringstream out;
		out << message_;
		out << " (" << getLocation().fileName << ":" << getLocation().line << ")";
		return out.str();
	}
};


#define DYN_MAKE_ERROR(msg) new DynError( \
	TextLoc(__FILE__, __LINE__),      \
	(msg),                            \
	TextLoc(__FILE__, __LINE__)       \
);


/** @} */


} // namespace cppapp


#endif /* end of include guard: Dyn_2HV25IX3 */

