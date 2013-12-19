/**
 * \file   Test.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-06-04
 * 
 * \brief  Implementation file for the Test class.
 */


#include "Test.h"
#include "Logger.h"
#include "utils.h"


namespace cppapp {


////////////////////////////////////////////////////////////////////////////////
// TEST REF
////////////////////////////////////////////////////////////////////////////////


TestResult TestRef::run()
{
	TestResult result;
	
	setUp();
	
	try {
		executeMethod();
	} catch (TestResult &r) {
		result = r;
	} catch (std::exception &e) {
		result = TestResult(e);
	} catch (...) {
		result = TestResult(false, "", 0, "", "unknown exception");
	}
	
	result.expectedFailure = getExpectFailure();
	
	tearDown();
	
	return result;
}


TestResult TestRef::debug()
{
	TestResult result;
	
	setUp();
	executeMethod();
	tearDown();
	
	return result;
}


////////////////////////////////////////////////////////////////////////////////
// TEST RESULT
////////////////////////////////////////////////////////////////////////////////


/**
 * Constructor.
 */
TestResult::TestResult(std::exception &e) :
	success(false), exception(true),
	expectedFailure(false), expectedException(false),
	hasLocation(false), file(""), line(0),
	assertion(""), message(e.what())
{ }


/**
 * Constructor.
 */
TestResult::TestResult(std::exception &e,
				   const TestBacktrace &backtrace) :
	success(false), exception(true),
	expectedFailure(false), expectedException(false),
	hasLocation(false), file(""), line(0),
	assertion(""), message(e.what()),
	backtrace(backtrace)
{ }


////////////////////////////////////////////////////////////////////////////////
// TEST RESULTS
////////////////////////////////////////////////////////////////////////////////


void TestResults::add(const TestResults &result)
{
	results_.insert(results_.end(), result.begin(), result.end());
}


////////////////////////////////////////////////////////////////////////////////
// TEST SUITE
////////////////////////////////////////////////////////////////////////////////


//TestSuite TestSuite::defaultSuite_;


TestSuite::TestSuite() :
	name_("<Unnamed>")
{ }


TestSuite::TestSuite(std::string name) :
	name_(name)
{ }


TestSuite::~TestSuite()
{
	FOR_EACH(tests_, test) {
		*test = NULL;
	}
	tests_.clear();
}


void TestSuite::add(Ref<TestRef> test)
{
	tests_.push_back(test);
}


void TestSuite::add(const TestSuite &suite)
{
	tests_.insert(tests_.end(), suite.begin(), suite.end());
}


void TestSuite::run()
{
	TextTestRunner runner;
	runner.run(*this);
}


void TestSuite::debug()
{
	TextTestRunner runner;
	runner.debug(*this);
}


TestSuite& TestSuite::getDefaultSuite()
{
	static TestSuite defaultSuite;
	return defaultSuite;
}


void TestSuite::runDefault()
{
	getDefaultSuite().run();
}


////////////////////////////////////////////////////////////////////////////////
// TEST CASE
////////////////////////////////////////////////////////////////////////////////


void TestCase::assert_(bool value, const char *file, int line,
				   const char *assertion, const char *message)
{
	if (!value)
		throw TestResult(value, file, line, assertion, message);
		//throw TestFailure(file, line, assertion, message);
}


void TestCase::reset()
{
}


////////////////////////////////////////////////////////////////////////////////
// TEST RUNNER
////////////////////////////////////////////////////////////////////////////////


void TestRunner::run(const TestSuite &tests)
{
	testCount_ = 0;
	failureCount_ = 0;
	exceptionCount_ = 0;
	expectedFailureCount_ = 0;
	
	startTests(tests);
	
	FOR_EACH(tests, test) {
		startTest(*test);
		
		TestResult result = (*test)->run();
		if (!result.success) {
			if (result.expectedFailure)
				expectedFailureCount_++;
			else if (result.exception)
				exceptionCount_++;
			else
				failureCount_++;
		}
		
		addTestResult(*test, result);
		
		testCount_++;
	}
	
	finishTests(tests);
}


void TestRunner::debug(const TestSuite &tests)
{
	startDebug(tests);
	
	FOR_EACH(tests, test) {
		startTestDebug(*test);
		
		TestResult result = (*test)->debug();
		
		finishTestDebug(*test, result);
	}
	
	finishDebug(tests);
}


////////////////////////////////////////////////////////////////////////////////
// TEXT TEST RUNNER
////////////////////////////////////////////////////////////////////////////////


void TextTestRunner::startTests(const TestSuite &tests)
{
	*output_ << "Running " << tests.getCount() << " tests..." << std::endl;
}


void TextTestRunner::finishTests(const TestSuite &tests)
{
	for (int i = 0; i < 80; i++)
		*output_ << "=";
	*output_ << std::endl;
	*output_ << "\033[1m" << getTestCount() << " tests run, ";
	
	if (getFailureCount() == 0)
		*output_ << getFailureCount() << " failures, ";
	else
		*output_ << "\033[1;31m" << getFailureCount() << " failure(s)\033[0m, ";
	
	if (getExceptionCount() == 0)
		*output_ << getExceptionCount() << " exceptions";
	else
		*output_ << "\033[1;33m" << getExceptionCount() << " exception(s)\033[0m";
	
	if (getExpectedFailureCount() == 0) {
		*output_ << "." << std::endl;
	} else {
		*output_ << ", ";
		*output_ << getExpectedFailureCount() << " expected failure(s)." << std::endl;
	}
}


void TextTestRunner::startTest(Ref<TestRef> test)
{
	*output_ << test->getTestCase()->getName() << "::" << test->getName() << "... " <<
		std::flush;
}


void TextTestRunner::addTestResult(Ref<TestRef> test, TestResult result)
{
	if (result.success) {
		*output_ << "OK" << std::flush;
	} else if (result.expectedFailure) {
		if (result.exception)
			*output_ << "EXCEPTION";
		else
			*output_ << "FAILURE";
		*output_ << " (expected)";
	} else {
		if (result.exception)
			*output_ << "\033[1;33mEXCEPTION\033[0m" << std::flush;
		else
			*output_ << "\033[1;31mFAILURE\033[0m" << std::flush;
	}
	
	//if (result.exception)
	//	*output_ << (result.success ? "OK" : "\033[1;33mEXCEPTION\033[0m") << std::flush;
	//else
	//	*output_ << (result.success ? "OK" : "\033[1;31mFAILURE\033[0m") << std::flush;
	*output_ << std::endl << std::flush;
	
	if (result.exception) {
		*output_ << "\tUnexpected exception occured." << endl;
		if (result.message != NULL)
			*output_ << "\tMessage: " << result.message << endl;
	} else if (!result.success) {
		if (result.assertion.size() > 0)
			*output_ << "\tAssertion " << result.assertion << " failed at " <<
			    result.file << ":" << result.line << "." << endl;
		else
			*output_ << "\tAssertion failed at " <<
				result.file << ":" << result.line << "." << endl;
		
		if (result.message != NULL)
			*output_ << "\tMessage: " << result.message << endl;
	}
}


void TextTestRunner::startDebug(const TestSuite &tests)
{
	*output_ << "Debugging " << tests.getCount() << " tests..." << std::endl;
}


void TextTestRunner::startTestDebug(Ref<TestRef> test)
{
	*output_ << test->getTestCase()->getName() <<
		"::" << test->getName() << "... " <<
		std::endl << std::flush;
}


////////////////////////////////////////////////////////////////////////////////
// TEST TEST CASE
////////////////////////////////////////////////////////////////////////////////


/**
 * Constructor.
 */
TestTestCase::TestTestCase() :
	TestCase()
{
	TEST_ADD (TestTestCase, noFailureTest);
	TEST_ADD2(TestTestCase, unconditionalFailureTest, true);
	TEST_ADD2(TestTestCase, simpleFailureTest, true);
	TEST_ADD2(TestTestCase, exceptionFailureTest, true);
	TEST_ADD (TestTestCase, equalsAssertionTest);
	TEST_ADD2(TestTestCase, equalsAssertionFailureTest, true);
}


class LocalTestCase : public TestCase {
public:
	void noFailureTest() {}
	
	LocalTestCase() : TestCase()
	{
		TEST_ADD(LocalTestCase, noFailureTest);
	}
};


void TestTestCase::noFailureTest()
{
	//LocalTestCase testCase;
	Ref<TestCase> testCase = new LocalTestCase();
	
	TestRunner runner;
	runner.run(*testCase);
	
	TEST_EQUALS(1, runner.getTestCount(), "1 test should have been executed.");
	TEST_EQUALS(0, runner.getFailureCount(), "0 failures should have occured.");
}


void TestTestCase::unconditionalFailureTest()
{
	TEST_FAIL("This always fails (gracefully).");
}


void TestTestCase::simpleFailureTest()
{
	TEST_ASSERT(false, "This is supposed to fail (gracefully).");
}


void TestTestCase::exceptionFailureTest()
{
	throw std::exception();
}


void TestTestCase::equalsAssertionTest()
{
	int a = 4;
	int b = 2 + 2;
	TEST_EQUALS(a, b, "This really should be equal.");
}


void TestTestCase::equalsAssertionFailureTest()
{
	int a = 4;
	int b = 2 + 1;
	TEST_EQUALS(a, b, "This really should not be equal.");
}


} /* namespace cppapp */


