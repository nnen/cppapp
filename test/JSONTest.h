/**
 * \file   JSONTest.h
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2013-12-06
 *
 * \brief  Header file for the JSONTest class.
 */

#ifndef JSONTEST_OXFQ7L0M
#define JSONTEST_OXFQ7L0M


#include <sstream>
#include <iostream>
using namespace std;

#include <cppapp/cppapp.h>
using namespace cppapp;


class LexerTest : public TestCase {
public:
	LexerTest()
	{
		TEST_ADD(LexerTest, testLexer);
	}
	
	void testLexer()
	{
		Lexer         lexer;
		istringstream in("{:}");
		
		lexer.input(&in, "<string>");
		
		TEST_ASSERT(lexer.read('{'), "lookahead character is not what is expected");
		TEST_ASSERT(lexer.read(':'), "lookahead character is not what is expected");
		TEST_ASSERT(lexer.read('}'), "lookahead character is not what is expected");
	}
};

RUN_SUITE(LexerTest);


class JSONBooleanTest : public TestCase {
public:
	JSONBooleanTest()
	{
		TEST_ADD(JSONBooleanTest, testParse);
		TEST_ADD(JSONBooleanTest, testGetMethods);
	}
	
	void testParse()
	{
		bool value;
		
		TEST_ASSERT(JSONBoolean::parse("true", &value), "failed to parse legal string");
		TEST_EQUALS(true, value, "parsed the string, but returned invalid value");
		
		TEST_ASSERT(JSONBoolean::parse("True", &value), "failed to parse legal string");
		TEST_EQUALS(true, value, "parsed the string, but returned invalid value");
		
		TEST_ASSERT(JSONBoolean::parse("false", &value), "failed to parse legal string");
		TEST_EQUALS(false, value, "parsed the string, but returned invalid value");

		TEST_ASSERT(JSONBoolean::parse("False", &value), "failed to parse legal string");
		TEST_EQUALS(false, value, "parsed the string, but returned invalid value");
		
		TEST_ASSERT(!JSONBoolean::parse("xxx", &value), "parsed illegal string");
	}
	
	void testGetMethods()
	{
		TextLoc loc;
		
		Ref<JSONBoolean> val = new JSONBoolean(loc, true);
		TEST_EQUALS(true, val->getBool(), "");
		TEST_EQUALS(1.0, val->getDouble(), "");
		TEST_EQUALS("true", val->getString(), "");
		
		val = new JSONBoolean(loc, false);
		TEST_EQUALS(false, val->getBool(), "");
		TEST_EQUALS(0.0, val->getDouble(), "");
		TEST_EQUALS("false", val->getString(), "");
	}
};

RUN_SUITE(JSONBooleanTest);


class JSONNumberTest : public TestCase {
public:
	JSONNumberTest()
	{
		TEST_ADD(JSONNumberTest, testParse);
	}

	void testParse()
	{
		double value;
		
		TEST_ASSERT(JSONNumber::parse("0", &value), "failed to parse legal string");
		TEST_EQUALS(0, value, "parsed the string, but returned invalid value");
		
		TEST_ASSERT(JSONNumber::parse("1", &value), "failed to parse legal string");
		TEST_EQUALS(1, value, "parsed the string, but returned invalid value");
		
		TEST_ASSERT(JSONNumber::parse("123", &value), "failed to parse legal string");
		TEST_EQUALS(123, value, "parsed the string, but returned invalid value");
		
		TEST_ASSERT(JSONNumber::parse("1.23", &value), "failed to parse legal string");
		TEST_EQUALS(1.23, value, "parsed the string, but returned invalid value");
		
		TEST_ASSERT(!JSONNumber::parse("xxx", &value), "failed to parse legal string");
	}

	void testGetMethods()
	{
		TextLoc loc;

		Ref<JSONNumber> val = new JSONNumber(loc, 0);
		TEST_EQUALS(false, val->getBool(), "");
		TEST_EQUALS(0.0, val->getDouble(), "");
		TEST_EQUALS("0", val->getString(), "");
		
		val = new JSONNumber(loc, 1);
		TEST_EQUALS(true, val->getBool(), "");
		TEST_EQUALS(1.0, val->getDouble(), "");
		TEST_EQUALS("1", val->getString(), "");
	}
};

RUN_SUITE(JSONNumberTest);


/**
 * \todo Write documentation for class JSONTest.
 */
class JSONTest : public TestCase {
private:
	
public:
	/**
	 * Constructor.
	 */
	JSONTest()
	{
		TEST_ADD(JSONTest, testParseEmptyDict);
		TEST_ADD(JSONTest, testParseEmptyList);
		TEST_ADD(JSONTest, testParseEmptyString);
		TEST_ADD(JSONTest, testParseNumber);
		TEST_ADD(JSONTest, testParseBool);
		TEST_ADD(JSONTest, testParseDict);
	}
	
	void testParseEmptyDict()
	{
		istringstream iss("{}");
		JSONParser parser;
		
		Ref<JSONObject> result = parser.parse(&iss, "<string>");
		TEST_ASSERT(result->isDict(), "the result is not a JSONDict as expected");
	}
	
	void testParseEmptyList()
	{
		istringstream iss("[]");
		JSONParser parser;
		
		Ref<JSONObject> result = parser.parse(&iss, "<string>");
		TEST_ASSERT(result->isList(), "the result is not a JSONList as expected");
	}

	void testParseEmptyString()
	{
		istringstream iss("\"\"");
		JSONParser parser;
		
		Ref<JSONObject> result = parser.parse(&iss, "<string>");
		TEST_ASSERT(result->isString(), "the result is not a JSONString as expected");
	}
	
	void testParseNumber(const char *json, double value)
	{
		istringstream iss(json);
		JSONParser parser;
		
		Ref<JSONObject> result = parser.parse(&iss, "<string>");
		TEST_ASSERT(result->isNum(), "the result is not a JSONNumber as expected");
		Ref<JSONNumber> number = result;
		TEST_EQUALS(value, number->getValue(), "the value of the json number is not 0 as expected");
	}
	
	void testParseNumber()
	{
		testParseNumber("0", 0.0);
		testParseNumber("1", 1.0);
		testParseNumber("0.1", 0.1);
		testParseNumber("0.12345", 0.12345L);
		testParseNumber("12345.12", 12345.12L);
	}

	void testParseBool(const char *json, bool value)
	{
		istringstream iss(json);
		JSONParser parser;
		
		Ref<JSONObject> result = parser.parse(&iss, "<string>");
		TEST_ASSERT(result->isBool(), "the result is not a JSONNumber as expected");
		Ref<JSONBoolean> number = result;
		TEST_EQUALS(value, number->getValue(), "the value of the json bool is not what was expected");
	}

	void testParseBool()
	{
		testParseBool("true", true);
		testParseBool("True", true);
		testParseBool("false", false);
		testParseBool("False", false);
	}
	
	void testParseDict()
	{
		istringstream iss("{\"value\": 12, \"some_list\": [1, 2, 3]}");
		JSONParser parser;
		
		Ref<JSONObject> result = parser.parse(&iss, "<string>");
		TEST_ASSERT(result->isDict(), "the result is not a JSONDict as expected");
		
		Ref<JSONDict> dict = result;
		TEST_EQUALS(2, dict->getSize(), "the size of the json dict is not what is expected");
		
		TEST_EQUALS(2, result->getSize(), "the size of the json dict is not what is expected");
		TEST_ASSERT(result->hasItem("value"), "the parsed json dict doesn't have the expected keys");
		TEST_ASSERT(result->getItem("value")->getInt() == 12, "a json dict item doesn't have the expected value");
		TEST_ASSERT(result->hasItem("some_list"), "the parsed json dict doesn't have the expected keys");
	}
};

RUN_SUITE(JSONTest);

#endif /* end of include guard: JSONTEST_OXFQ7L0M */

