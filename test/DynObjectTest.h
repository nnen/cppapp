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
