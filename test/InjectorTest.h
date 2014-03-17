/**
 * \file   InjectorTest.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-12-23
 *
 * \brief  Header file for the InjectorTest class.
 */

#ifndef INJECTORTEST_6ZU9RDQK
#define INJECTORTEST_6ZU9RDQK


#include <cppapp/cppapp.h>
using namespace cppapp;


class DITestObjectA : public DIObject {
public:
	virtual ~DITestObjectA() {}
};


class DITestObjectB : public DIObject {
public:
	virtual ~DITestObjectB() {}
};


Ref<DIObject> testCreate(Ref<DynObject> config, Ref<DIObject> parent)
{
	return new DITestObjectA();
}


/**
 * \todo Write documentation for class InjectorTest.
 */
class InjectorTest : public TestCase {
public:
	/**
	 * Constructor.
	 */
	InjectorTest()
	{
		TEST_ADD(InjectorTest, testRegisterFunction);
		TEST_ADD(InjectorTest, testComplex01);
		TEST_ADD(InjectorTest, testComplex02);
	}

	virtual ~InjectorTest() {}
	
	void testRegisterFunction()
	{
		CPPAPP_DI_FUNCTION("test.testFactory", testCreate);
		
		Ref<DIFactory> factory = Injector::getInstance().getFactory("test.testFactory");
		TEST_ASSERT(factory.isNotNull(), "DI factory should not be null");
	}
	
	void testComplex01()
	{
		Injector::getInstance().clear();
		
		CPPAPP_DI_FUNCTION("test.testFactory", testCreate);
		
		JSONParser parser;
		Ref<DynObject> config = parser.parse(
			"["
			"	{"
			"		\"key\":     \"test.testPlan\","
			"		\"factory\": \"test.testFactory\","
			"	},"
			"]"
		);
		TEST_ASSERT(config.isNotNull(), "the config should not be null");
		
		Injector::getInstance().makePlans(config);
		
		Ref<DIPlan> plan = Injector::getInstance().getPlan("test.testPlan");
		TEST_ASSERT(plan.isNotNull(), "the plan should not be null");
		
		Ref<DITestObjectA> obj =
			Injector::getInstance().instantiateAs<DITestObjectA>("test.testPlan");
		TEST_ASSERT(obj.isNotNull(), "the resulting object should not be null");
	}
	
	void testComplex02()
	{
		Injector::getInstance().clear();
		
		CPPAPP_DI_FUNCTION("test.testFactory", testCreate);
		
		JSONParser parser;
		Ref<DynObject> config = parser.parse(
			"["
			"	{"
			"		\"key\":     \"test.testPlan01\","
			"		\"factory\": \"test.testFactory\","
			"	},"
			"	{"
			"		\"key\":     \"test.testPlan02\","
			"		\"factory\": \"test.testFactory\","
			"	},"
			"]"
		);
		TEST_ASSERT(config.isNotNull(), "the config should not be null");
		
		Injector::getInstance().makePlans(config);
		
		Ref<DIPlan> plan = Injector::getInstance().getPlan("test.testPlan01");
		TEST_ASSERT(plan.isNotNull(), "the plan should not be null");
		
		plan = Injector::getInstance().getPlan("test.testPlan02");
		TEST_ASSERT(plan.isNotNull(), "the plan should not be null");

		Ref<DITestObjectA> obj =
			Injector::getInstance().instantiateAs<DITestObjectA>("test.testPlan01");
		TEST_ASSERT(obj.isNotNull(), "the resulting object should not be null");
		
		obj =
			Injector::getInstance().instantiateAs<DITestObjectA>("test.testPlan02");
		TEST_ASSERT(obj.isNotNull(), "the resulting object should not be null");
		
		Ref<DITestObjectB> obj2 =
			Injector::getInstance().instantiateAs<DITestObjectB>("test.testPlan01");
		TEST_ASSERT(obj2.isNull(), "the cast to DITestObjectB should fail");
	}

};

RUN_SUITE(InjectorTest);


#endif /* end of include guard: INJECTORTEST_6ZU9RDQK */

