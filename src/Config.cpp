/**
 * \file   Config.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-04-28
 * 
 * \brief Implementation file for the Config class.
 */

#include "Config.h"
#include "Logger.h"

#include <cstdlib>

#include "utils.h"
#include "string_utils.h"


namespace cppapp {


////////////////////////////////////////////////////////////////////////////////
// ConfigValue
////////////////////////////////////////////////////////////////////////////////


/**
 * Constructor.
 */
ConfigValue::ConfigValue(string name, string rawValue) :
	name_(name), rawValue_(trim(rawValue))
{
}


/**
 *
 */
bool ConfigValue::asBool() const
{
	string l = toLower(rawValue_);
	
	if ((l.compare("f") == 0) ||
	    (l.compare("false") == 0) ||
	    (l.compare("0") == 0))
		return false;
	return l.size() > 0;
}


int ConfigValue::asInteger() const
{
	return atoi(rawValue_.c_str());
}


float ConfigValue::asFloat() const
{
	return atof(rawValue_.c_str());
}


Ref<ConfigValue> ConfigValue::make(string name, string value)
{
	return new ConfigValue(name, value);
}


Ref<ConfigValue> ConfigValue::make(string name, bool value)
{
	return new ConfigValue(name,
					   (value ? "true" : "false"));
}


////////////////////////////////////////////////////////////////////////////////
// Config
////////////////////////////////////////////////////////////////////////////////


Ref<Config> Config::globalConfig_;


/**
 *
 */
void Config::setDefault(Ref<ConfigValue> value)
{
	defaults_[value->getName()] = value;
}


/**
 *
 */
void Config::set(Ref<ConfigValue> value)
{
	values_[value->getName()] = value;
}


/**
 *
 */
Ref<ConfigValue> Config::get(const string &key)
{
	VAR(it, values_.find(key));
	if (it != values_.end()) return it->second;
	
	it = defaults_.find(key);
	if (it != values_.end()) return it->second;
	
	return NULL;
}


Ref<ConfigValue> Config::get(const string &key, const string &deflt)
{
	VAR(result, get(key));
	if (result.isNull()) return new ConfigValue(key, deflt);
	return result;
}


/**
 *
 */
void Config::dump(ostream &output)
{
	output << "# Defaults" << endl;
	
	FOR_EACH(defaults_, it) {
		output << it->first << " = " << it->second->asString() << endl;
	}
	
	output << "# Values" << endl;
	
	FOR_EACH(values_, it) {
		output << it->first << " = " << it->second->asString() << endl;
	}
}


////////////////////////////////////////////////////////////////////////////////
// ConfigParser
////////////////////////////////////////////////////////////////////////////////


/**
 *
 */
void ConfigParser::handleError()
{
	LOG_ERROR("Error occured while reading configuration from " <<
			input_->getName() << " on line " << loc_.line << ".");
	error_ = true;
}


/**
 *
 */
bool ConfigParser::shouldContinue()
{
	return ((!error_) && (input_->getStream()->good()));
}


/**
 *
 */
void ConfigParser::skipWhitespace()
{
	if (!shouldContinue()) return;
	
	while ((peek() != '\n') && (isspace(peek())))
		get();
}


/**
 *
 */
void ConfigParser::readComment()
{
	if (!shouldContinue()) return;
	while (peek() != '\n') get();
}


/**
 *
 */
void ConfigParser::readKey()
{
	if (!shouldContinue()) return;
	
	ostringstream output;
	
	skipWhitespace();
	
	while (isKeyChar(peek()))
		output.put(get());
	
	string result = output.str();
	
	if (result.size() == 0) {
		handleError();
		return;
	}
	
	key_ = result;
}


void ConfigParser::readValue()
{
	if (!shouldContinue()) return;
	
	ostringstream output;
	
	skipWhitespace();
	
	while ((peek() != '\n') && (peek() != EOF))
		output.put(get());
	
	value_ = output.str();
}


void ConfigParser::readLine()
{
	if (!shouldContinue()) return;
	
	skipWhitespace();
	
	if (peek() == '#') {
		readComment();
	} else if (isalnum(peek())) {
		readKey();
		skipWhitespace();
		expect('=');
		readValue();
		
		ConfigValue *cfgval = new ConfigValue(key_, value_);
		config_->set(cfgval);
	}
	
	skipWhitespace();
}


/**
 * Constructor.
 */
ConfigParser::ConfigParser(Ref<Config> config) :
	config_(config)
{
}


/**
 *
 */
void ConfigParser::parse(Ref<Input> input)
{
	input_ = input;
	loc_ = TextLoc();
	error_ = false;
	key_ = "";
	value_ = "";

	if (!input->getStream()->good()) {
		LOG_WARNING("Cannot read configuration from " <<
				  input->getName() <<
				  ". If the stream is a file, it may not exist.");
		return;
	}
	
	if (config_.isNull())
		config_ = new Config();
	
	while (true) {
		readLine();
		if (peek() != '\n') break;
		get();
	}
	
	if (peek() != EOF) handleError();
}


} // namespace cppapp

