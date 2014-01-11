/**
 * \file   DynObjectTest.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-12-19
 *
 * \brief  Header file for the DynObjectTest class.
 */

#ifndef DYNOBJECTTEST_ZZGY7R
#define DYNOBJECTTEST_ZZGY7R


#include <cppapp/cppapp.h>
using namespace cppapp;


/**
 * \todo Write documentation for class DynObjectTest.
 */
class DynObjectTest : public TestCase {
private:
	
public:
	/**
	 * Constructor.
	 */
	DynObjectTest()
	{
		TEST_ADD(DynObjectTest, testGetDottedItem);
	}
	
	void testGetDottedItem()
	{
		Ref<DynObject> obj = new DynDict(TextLoc());
		obj->setStrItem("first", new DynDict(TextLoc()));
		obj->getStrItem("first")->setStrItem("second", new DynDict(TextLoc()));
		obj->getStrItem("first")->setStrItem("x", new DynNumber(TextLoc(), 13));
		obj->getStrItem("first")
			->getStrItem("second")
			->setStrItem("third", new DynNumber(TextLoc(), 7));
		
		Ref<DynObject> res = obj->getDottedItem("first.second.third");

		TEST_ASSERT(!res->isError(), "result should not be an error");
		TEST_ASSERT(res->isNum(), "result should be a number");
		TEST_EQUALS(7, res->getInt(), "result should be equal to 7");
		
		res = obj->getDottedItem("first.x");

		TEST_ASSERT(!res->isError(), "result should not be an error");
		TEST_ASSERT(res->isNum(), "result should be a number");
		TEST_EQUALS(13, res->getInt(), "result should be equal to 13");
		
		obj->setDottedItem("first.y", new DynNumber(TextLoc(), 1337));
		res = obj->getDottedItem("first.y");

		TEST_ASSERT(!res->isError(), "result should not be an error");
		TEST_ASSERT(res->isNum(), "result should be a number");
		TEST_EQUALS(1337, res->getInt(), "result should be equal to 1337");
	}
};

RUN_SUITE(DynObjectTest);


class DynNumberTest : public TestCase {
public:
	DynNumberTest()
	{
		TEST_ADD(DynNumberTest, testParse00);
	}
	
	void testParse00()
	{
		double result = 0.0;
		TEST_ASSERT(DynNumber::parse("1", &result), "string should be parsed");
		TEST_EQUALS(1, result, "result should be 1");
		
		result = 0.0;
		TEST_ASSERT(DynNumber::parse("-1", &result), "string should be parsed");
		TEST_EQUALS(-1, result, "result should be 1");

		result = 0.0;
		TEST_ASSERT(DynNumber::parse("1234", &result), "string should be parsed");
		TEST_EQUALS(1234, result, "result should be 1");
		
		result = 0.0;
		TEST_ASSERT(DynNumber::parse("-1234", &result), "string should be parsed");
		TEST_EQUALS(-1234, result, "result should be 1");
		
		result = 0.0;
		TEST_ASSERT(DynNumber::parse("1234.12", &result), "string should be parsed");
		TEST_EQUALS(1234.12, result, "result should be 1");
		
		result = 0.0;
		TEST_ASSERT(DynNumber::parse("-1234.12", &result), "string should be parsed");
		TEST_EQUALS(-1234.12, result, "result should be 1");
		
		// TEST_ASSERT(!DynNumber::parse("-", &result), "string should not be parsed");
	}
};

RUN_SUITE(DynNumberTest);


class DynStringTest : public TestCase {
public:
	DynStringTest()
	{
		TEST_ADD(DynStringTest, testGetBool00);
		TEST_ADD(DynStringTest, testGetDouble00);
		TEST_ADD(DynStringTest, testGetInt00);
		TEST_ADD(DynStringTest, testGetString00);
	}
	
	void testGetBool00()
	{
		Ref<DynObject> str = new DynString(TextLoc(), "true");
		TEST_EQUALS(true, str->getBool(), "the string should convert to true");
		
		str = new DynString(TextLoc(), "false");
		TEST_EQUALS(false, str->getBool(), "the string should convert to false");
	}
	
	void testGetDouble00()
	{
		for (double i = 0; i < 1000; i += 10) {
			std::stringstream s;
			s << i;
			
			Ref<DynObject> str = new DynString(CPPAPP_TEXT_LOC, s.str());
			TEST_EQUALS(i, str->getDouble(), "the string should convert to a number");
		}
	}
	
	void testGetInt00()
	{
		for (int i = 0; i < 1000; i += 10) {
			std::stringstream s;
			s << i;
			
			Ref<DynObject> str = new DynString(CPPAPP_TEXT_LOC, s.str());
			TEST_EQUALS(i, str->getInt(), "the string should convert to a number");
		}
	}

	void testGetString00()
	{
		Ref<DynObject> str = new DynString(CPPAPP_TEXT_LOC, "hello");
		TEST_EQUALS(std::string("hello"), str->getString(), "the string should equal to itself");
	}
};

RUN_SUITE(DynStringTest);


class DynListTest : public TestCase {
public:
	DynListTest()
	{
		TEST_ADD(DynListTest, testGetIterator);
	}
	
	void testGetIterator()
	{
		Ref<DynList> list = new DynList(CPPAPP_TEXT_LOC);
		int index = 0;
		
		DYN_FOR_EACH(item, list) {
			TEST_ASSERT(item == list->getIntItem(index), "iterated item doesn't have the expected value");
			index++;
		}
		TEST_EQUALS(0, index, "the cycle made unexpected number of iterations");
		
		list->append(new DynNumber(CPPAPP_TEXT_LOC, 98));
		list->append(new DynNumber(CPPAPP_TEXT_LOC, 23));
		list->append(new DynNumber(CPPAPP_TEXT_LOC, 54));
		list->append(new DynNumber(CPPAPP_TEXT_LOC, 42));
		TEST_EQUALS(4, list->getSize(), "list has unexpected size");
		
		int index2 = 0;
		DYN_FOR_EACH(item2, list) {
			TEST_ASSERT(item2 == list->getIntItem(index2), "iterated item doesn't have the eitem2pected value");
			index2++;
		}
		TEST_EQUALS(4, index2, "the cycle made unexpected number of iterations");
	}
};

RUN_SUITE(DynListTest);


#endif /* end of include guard: DYNOBJECTTEST_ZZGY7R */

