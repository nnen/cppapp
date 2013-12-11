/**
 * \file   Config.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-04-28
 *
 * \brief Header file for the Config class.
 */

#ifndef CONFIG_2HV25IX3
#define CONFIG_2HV25IX3


#include <string>
#include <sstream>
#include <iostream>
#include <map>
using namespace std;

#include "Object.h"
#include "Input.h"


namespace cppapp {


class ConfigValue : public Object {
private:
	string name_;
	string rawValue_;

public:
	ConfigValue(string name, string rawValue);
	virtual ~ConfigValue() {}
	
	string getName() { return name_; }
	
	string asString() const { return rawValue_; }
	bool   asBool() const;
	int    asInteger() const;
	float  asFloat() const;
	
	template<class T>
	static Ref<ConfigValue> make(string name, T value)
	{
		ostringstream s;
		s << value;
		return new ConfigValue(name, s.str());
	}
	
	static Ref<ConfigValue> make(string name, string value);
	static Ref<ConfigValue> make(string name, bool value);
};


/**
 * \todo Write documentation for class Config.
 */
class Config : public Object {
private:
	map<string, Ref<ConfigValue> > defaults_;
	map<string, Ref<ConfigValue> > values_;

	static Ref<Config> globalConfig_;
	
public:
	static Ref<Config> globalConfig()
	{
		if (globalConfig_.isNull())
			globalConfig_ = new Config();
		return globalConfig_;
	}
	static void setGlobalConfig(Ref<Config> config) { globalConfig_ = config; }
	
	Config() {}
	//Config(const Config& other);
	virtual ~Config() {}
	
	void setDefault(Ref<ConfigValue> value);
	void set(Ref<ConfigValue> value);
	
	Ref<ConfigValue> get(const string &key);
	Ref<ConfigValue> get(const string &key, const string &deflt);
	
	void dump(ostream &output);
};


struct TextLoc2 {
	int line;
	int column;

	TextLoc2() :
		line(1), column(1)
	{ }
	
	TextLoc2(int line, int column) :
		line(line), column(column)
	{ }
	
	TextLoc2 newline() { return TextLoc2(line + 1, 1); }

	void operator++(int dummy)
	{
		column++;
	}
};


class ConfigParser : public Object {
private:
	Ref<Config> config_;
	
	Ref<Input> input_;
	
	TextLoc2 loc_;
	bool error_;
	string key_;
	string value_;
	
	inline bool isKeyChar(int c)
	{
		return (!isspace(c)) && (c != '=');
	}
	
	inline int peek() { return input_->getStream()->peek(); }
	
	inline int get()
	{
		int result = input_->getStream()->get();
		if (result == EOF) return result;
		loc_++;
		if (result == '\n') loc_ = loc_.newline();
		return result;
	}
	
	inline void expect(int ch)
	{
		if (peek() != ch) {
			handleError();
			return;
		}
		get();
	}
	
	void handleError();
	bool shouldContinue();
	
	void skipWhitespace();
	void readComment();
	void readKey();
	void readValue();
	void readLine();

public:
	ConfigParser(Ref<Config> config);
	virtual ~ConfigParser() {}

	Ref<Config> getConfig() { return config_; }
	
	void parse(Ref<Input> input);
};


} // namespace cppapp


#endif /* end of include guard: CONFIG_2HV25IX3 */

