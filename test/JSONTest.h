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
		Lexer lexer;
		lexer.input("{:}");
		
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
		
		TEST_ASSERT(DynBoolean::parse("true", &value), "failed to parse legal string");
		TEST_EQUALS(true, value, "parsed the string, but returned invalid value");
		
		TEST_ASSERT(DynBoolean::parse("True", &value), "failed to parse legal string");
		TEST_EQUALS(true, value, "parsed the string, but returned invalid value");
		
		TEST_ASSERT(DynBoolean::parse("false", &value), "failed to parse legal string");
		TEST_EQUALS(false, value, "parsed the string, but returned invalid value");

		TEST_ASSERT(DynBoolean::parse("False", &value), "failed to parse legal string");
		TEST_EQUALS(false, value, "parsed the string, but returned invalid value");
		
		TEST_ASSERT(!DynBoolean::parse("xxx", &value), "parsed illegal string");
	}
	
	void testGetMethods()
	{
		TextLoc loc;
		
		Ref<DynBoolean> val = new DynBoolean(loc, true);
		TEST_EQUALS(true, val->getBool(), "");
		TEST_EQUALS(1.0, val->getDouble(), "");
		TEST_EQUALS("true", val->getString(), "");
		
		val = new DynBoolean(loc, false);
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
		
		TEST_ASSERT(DynNumber::parse("0", &value), "failed to parse legal string");
		TEST_EQUALS(0, value, "parsed the string, but returned invalid value");
		
		TEST_ASSERT(DynNumber::parse("1", &value), "failed to parse legal string");
		TEST_EQUALS(1, value, "parsed the string, but returned invalid value");
		
		TEST_ASSERT(DynNumber::parse("123", &value), "failed to parse legal string");
		TEST_EQUALS(123, value, "parsed the string, but returned invalid value");
		
		TEST_ASSERT(DynNumber::parse("1.23", &value), "failed to parse legal string");
		TEST_EQUALS(1.23, value, "parsed the string, but returned invalid value");
		
		TEST_ASSERT(!DynNumber::parse("xxx", &value), "failed to parse legal string");
	}

	void testGetMethods()
	{
		TextLoc loc;

		Ref<DynNumber> val = new DynNumber(loc, 0);
		TEST_EQUALS(false, val->getBool(), "");
		TEST_EQUALS(0.0, val->getDouble(), "");
		TEST_EQUALS("0", val->getString(), "");
		
		val = new DynNumber(loc, 1);
		TEST_EQUALS(true, val->getBool(), "");
		TEST_EQUALS(1.0, val->getDouble(), "");
		TEST_EQUALS("1", val->getString(), "");
	}
};

RUN_SUITE(JSONNumberTest);


/**
 * \todo Write documentation for class JSONTest.
 */
class JSONParserTest : public TestCase {
private:
	
public:
	/**
	 * Constructor.
	 */
	JSONParserTest()
	{
		TEST_ADD(JSONParserTest, testParseComplex);
		TEST_ADD(JSONParserTest, testParseEmptyDict00);
		TEST_ADD(JSONParserTest, testParseEmptyDict01);
		TEST_ADD(JSONParserTest, testParseEmptyList00);
		TEST_ADD(JSONParserTest, testParseEmptyList01);
		TEST_ADD(JSONParserTest, testParseEmptyString00);
		TEST_ADD(JSONParserTest, testParseEmptyString01);
		TEST_ADD(JSONParserTest, testParseNumber);
		TEST_ADD(JSONParserTest, testParseBool);
		TEST_ADD(JSONParserTest, testParseDict);
		//TEST_ADD(JSONParserTest, testParseDict);
		TEST_ADD(JSONParserTest, testParseComment);
		TEST_ADD(JSONParserTest, testParseHashComment);
	}
	
	void testParseComplex()
	{
		JSONParser parser;
		Ref<DynObject> result = parser.parse(
			"{\n"
			"	\"first\": 1,\n"
			"	\"second\": {},\n"
			"	\"third\": {\n"
			"		\"first\": [],"
			"		\"second\": [1, 2],"
			"		\"third\": [1, 2, 3, ],"
			"	},\n"
			"}\n"
		);
		TEST_ASSERT(result->isDict(), "the result is not a DynDict");

		result = parser.parse(
			"{\n"
			"	first = 1;\n"
			"	second {};\n"
			"	third {\n"
			"		first: [];"
			"		second = [1; 2];"
			"		third: [1, 2, 3, ];"
			"	},\n"
			"}\n"
		);
		TEST_ASSERT(result->isDict(), "the result is not a DynDict");

	}
	
	void testParseEmptyDict00()
	{
		JSONParser parser;
		Ref<DynObject> result = parser.parse("{}");
		TEST_ASSERT(result->isDict(), "the result is not a JSONDict as expected");
	}
	
	void testParseEmptyDict01()
	{
		JSONParser parser;
		Ref<DynObject> result = parser.parse("{");
		TEST_ASSERT(result->isError(),
				  "the result is not a JSONError as expected");

	}
	
	void testParseEmptyList00()
	{
		JSONParser parser;
		Ref<DynObject> result = parser.parse("[]");
		TEST_ASSERT(result->isList(),
				  "the result is not a JSONList as expected");
	}
	
	void testParseEmptyList01()
	{
		JSONParser parser;
		Ref<DynObject> result = parser.parse("[");
		TEST_ASSERT(result->isError(),
				  "the result is not a JSONError as expected");
	}
	
	void testParseEmptyString00()
	{
		JSONParser parser;
		Ref<DynObject> result = parser.parse("\"\"");
		TEST_ASSERT(result->isString(),
				  "the result is not a JSONString as expected");
	}
	
	void testParseEmptyString01()
	{
		JSONParser parser;
		Ref<DynObject> result = parser.parse("\"");
		TEST_ASSERT(result->isError(),
				  "the result is not a JSONError as expected");
	}

	
	void testParseNumber(const char *json, double value)
	{
		JSONParser parser;
		Ref<DynObject> result = parser.parse(json);
		TEST_ASSERT(result->isNum(), "the result is not a JSONNumber as expected");
		Ref<DynNumber> number = result;
		TEST_EQUALS(value, number->getValue(), "the value of the json number is not 0 as expected");
	}
	
	void testParseNumber()
	{
		testParseNumber("0", 0.0);
		testParseNumber("1", 1.0);
		testParseNumber("-1", -1.0);
		testParseNumber("0.1", 0.1);
		testParseNumber("0.12345", 0.12345L);
		testParseNumber("12345.12", 12345.12L);
	}
	
	void testParseBool(const char *json, bool value)
	{
		JSONParser parser;
		Ref<DynObject> result = parser.parse(json);
		TEST_ASSERT(result->isBool(), "the result is not a JSONNumber as expected");
		Ref<DynBoolean> number = result;
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
		JSONParser parser;
		Ref<DynObject> result = parser.parse("{\"value\": 12, \"some_list\": [1, 2, 3]}");
		TEST_ASSERT(result->isDict(), "the result is not a JSONDict as expected");
		
		Ref<DynDict> dict = result;
		TEST_EQUALS(2, dict->getSize(), "the size of the json dict is not what is expected");
		
		TEST_EQUALS(2, result->getSize(), "the size of the json dict is not what is expected");
		TEST_ASSERT(result->hasStrItem("value"), "the parsed json dict doesn't have the expected keys");
		TEST_ASSERT(result->getStrItem("value")->getInt() == 12, "a json dict item doesn't have the expected value");
		TEST_ASSERT(result->hasStrItem("some_list"), "the parsed json dict doesn't have the expected keys");
		
		result = parser.parse("{\"value\" = 12, \"some_list\" = [1, 2, 3]}");
		TEST_ASSERT(result->isDict(), "dictionary with equal signs is not parsed as expected");
		TEST_EQUALS(2, result->getSize(), "size of json dict with equals signs is not what is expected");
		
		result = parser.parse("{value { a = 1, b = 2 }}");
		TEST_ASSERT(result->isDict(), "dictionary without colons or equal signs is not parsed as expected");
	}
	
	void testParseComment()
	{
		JSONParser parser;
		Ref<DynObject> result = parser.parse("// some comment\n// 13\n10");
		TEST_ASSERT(result->isNum(), "the result is not a JSONNumber as expected");
		
		TEST_EQUALS(10, result->getInt(), "the result does not have the expected value");
		
		result = parser.parse(
			"{\n"
			"	\"first_key\": 10,\n"
			"	// some comment,\n"
			"	\"key\": \"value\",\n"
			"}"
		);
		if (result->isError()) {
			LOG_ERROR(result->getString());
		}
		TEST_ASSERT(result->isDict(), "the result should be a dict");
		TEST_EQUALS(2, result->getSize(), "there should be two items in the dict");
	}
	
	void testParseHashComment()
	{
		JSONParser parser;
		Ref<DynObject> result = parser.parse("# some comment\n# 13\n10");
		TEST_ASSERT(result->isNum(), "the result is not a JSONNumber as expected");
		TEST_ASSERT(10 == result->getInt(), "the result should be equal to 10");
	}

};

RUN_SUITE(JSONParserTest);

#endif /* end of include guard: JSONTEST_OXFQ7L0M */

