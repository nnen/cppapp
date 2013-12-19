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


#endif /* end of include guard: DYNOBJECTTEST_ZZGY7R */

