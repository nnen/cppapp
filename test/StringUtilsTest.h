/**
 * \file   StringUtilsTest.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-06-26
 *
 * \brief  Header file for the StringUtilsTest class.
 */

#ifndef STRINGUTILSTEST_XEELWV0Y
#define STRINGUTILSTEST_XEELWV0Y


#include <cppapp/cppapp.h>
using namespace cppapp;


/**
 * \todo Write documentation for class StringUtilsTest.
 */
class StringUtilsTest : public TestCase {
public:
	/**
	 * Constructor.
	 */
	StringUtilsTest()
	{
		TEST_ADD(StringUtilsTest, testSplit);
	}
	
	void testSplit()
	{
		vector<string> result;
		Strings::split("first.second.third", ".", &result);
		
		TEST_EQUALS(3, result.size(), "the split string doesn't have the expected number of items");
		TEST_EQUALS(string("first"), result[0], "the 1. item in the split string is not what was expected");
		TEST_EQUALS(string("second"), result[1], "the 2. item in the split string is not what was expected");
		TEST_EQUALS(string("third"), result[2], "the 3. item in the split string is not what was expected");

		result.clear();
		Strings::split("first..second", ".", &result);
		
		TEST_EQUALS(3, result.size(), "the split string doesn't have the expected number of items");
		TEST_EQUALS(string("first"), result[0], "the 1. item in the split string is not what was expected");
		TEST_EQUALS(string(""), result[1], "the 2. item in the split string is not what was expected");
		TEST_EQUALS(string("second"), result[2], "the 3. item in the split string is not what was expected");

		result.clear();
		Strings::split("simple", ".", &result);
		
		TEST_EQUALS(1, result.size(), "the split string doesn't have the expected number of items");
		TEST_EQUALS(string("simple"), result[0], "the 1. item in the split string is not what was expected");

	}
};

RUN_SUITE(StringUtilsTest);


#endif /* end of include guard: STRINGUTILSTEST_XEELWV0Y */

