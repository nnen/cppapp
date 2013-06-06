/**
 * \file   Test.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-06-04
 *
 * \brief  Header file for the Test class.
 */

#ifndef TEST_N6WIX7RO
#define TEST_N6WIX7RO


#include <exception>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <execinfo.h>
#include <stdio.h>

#include "Object.h"
#include "utils.h"


namespace cppapp {


struct TestResult;
class TestCase;


////////////////////////////////////////////////////////////////////////////////
// TEST BACKTRACE
////////////////////////////////////////////////////////////////////////////////


class TestBacktrace {
private:
	std::vector<void*> addresses_;
	std::vector<std::string> lines_;
	
	bool getLines()
	{
		std::stringstream tmpStream;
		std::vector<std::string> lines;
		
		FOR_EACH(addresses_, addr) {
			tmpStream.str("");
			tmpStream << "addr2line -fCp " << *addr << " 2> /dev/null";
			
			FILE *pipe = popen(tmpStream.str().c_str(), "r");
			if (!pipe) {
				tmpStream.str("");
				tmpStream << "[" << *addr << "]";
				lines.push_back(tmpStream.str());
				continue;
			}
			
			char buffer[128];
			tmpStream.str("");
			while (!feof(pipe)) {
				if (fgets(buffer, 128, pipe) != NULL) {
					for (char *p = buffer; (p < buffer + 128) && *p != '\n'; p++)
						tmpStream << *p;
				}
			}
			pclose(pipe);
			
			lines.push_back(tmpStream.str());
		}
		
		lines_.swap(lines);
		return true;
	}
	
public:
	typedef std::vector<std::string>::const_iterator Iterator;
	
	int size() const { return (int)addresses_.size(); }
	
	Iterator begin() const { return lines_.begin(); }
	Iterator end() const { return lines_.end(); }
	
	std::string operator[](int index) { return lines_[index]; }
	
	void get()
	{
		void *buffer[30];
		size_t size;
		
		size = backtrace(buffer, 30);
		
		vector<void*> result;
		for (int i = 0; i < (int)size; i++) {
			result.push_back(buffer[i]);
		}
		
		addresses_.swap(result);
		
		getLines();
	}
};


////////////////////////////////////////////////////////////////////////////////
// TEST REF
////////////////////////////////////////////////////////////////////////////////


class TestRef : public Object {
protected:
	const char *name_;
public:
	TestRef(const char *name) : name_(name) {}
	
	const char* getName() const { return name_; }
	virtual Ref<TestCase> getTestCase() = 0;
	
	virtual void setUp() = 0;
	virtual void tearDown() = 0;
	virtual void executeMethod() = 0;
	
	virtual TestResult run();
};


template<class T>
class TestRef_ : public TestRef {
private:
	Ref<T> instance_;
	void (T::*method_)();

public:
	TestRef_(const char *name) : TestRef(name), instance_(NULL), method_(NULL)
	{ }
	
	TestRef_(const char *name, Ref<T> instance, void (T::*method)()) :
		TestRef(name), instance_(instance), method_(method)
	{ }
	
	virtual Ref<TestCase> getTestCase() { return instance_; }
	
	virtual void setUp()
	{
		instance_->reset();
		instance_->setUp();
	}
	
	virtual void tearDown()
	{
		instance_->tearDown();
	}
	
	virtual void executeMethod()
	{
		//(instance_->*method_)();
		((instance_.getPtr())->*method_)();
	}
};


////////////////////////////////////////////////////////////////////////////////
// TEST RESULT
////////////////////////////////////////////////////////////////////////////////


struct TestResult {
	bool          success;
	bool          exception;
	
	bool          hasLocation;
	const char   *file;
	int           line;
	
	const char   *assertion;
	const char   *message;
	
	TestBacktrace backtrace;
	
	TestResult() :
		success(true), exception(false),
		hasLocation(false), file(""), line(0),
		assertion(""), message("")
	{}
	
	TestResult(bool success, const char *file, int line) :
		success(success), exception(false),
		hasLocation(true), file(file), line(line),
		assertion(""), message("")
	{}
	
	TestResult(bool success, const char *file, int line,
			 const char *assertion, const char *message) :
		success(success), exception(false),
		hasLocation(true), file(file), line(line),
		assertion(assertion), message(message)
	{}
	
	TestResult(std::exception &e);
	
	TestResult(std::exception &e,
			 const TestBacktrace &backtrace);
};


////////////////////////////////////////////////////////////////////////////////
// TEST RESULTS
////////////////////////////////////////////////////////////////////////////////


class TestResults : public Object {
private:
	std::vector<TestResult> results_;

public:
	typedef std::vector<TestResult>::iterator Iterator;
	typedef std::vector<TestResult>::const_iterator ConstIterator;
	
	Iterator      begin()       { return results_.begin(); }
	ConstIterator begin() const { return results_.begin(); }
	Iterator      end()       { return results_.end(); }
	ConstIterator end() const { return results_.end(); }
	
	void add(const TestResult &result) { results_.push_back(result); }
	void add(const TestResults &results);
};


////////////////////////////////////////////////////////////////////////////////
// TEST SUITE
////////////////////////////////////////////////////////////////////////////////


class TestSuite : public Object {
private:
	std::vector<Ref<TestRef> >  tests_;
	
	//static TestSuite defaultSuite_;

public:
	typedef std::vector<Ref<TestRef> >::iterator Iterator;
	typedef std::vector<Ref<TestRef> >::const_iterator ConstIterator;
	
	virtual ~TestSuite();
	
	virtual void add(Ref<TestRef> test);
	virtual void add(const TestSuite &suite);
	
	virtual int getCount() const { return tests_.size(); }
	
	virtual Iterator begin()            { return tests_.begin(); }
	virtual ConstIterator begin() const { return tests_.begin(); }
	virtual Iterator end()              { return tests_.end(); }
	virtual ConstIterator end() const   { return tests_.end(); }
	
	void run();
	
	static TestSuite& getDefaultSuite(); // { return defaultSuite_; }
	static void runDefault();
};


////////////////////////////////////////////////////////////////////////////////
// TEST SUITE REGISTRATION
////////////////////////////////////////////////////////////////////////////////


#define RUN_SUITE(name) TestSuiteRegistration<name> testSuite_##name(#name);


template<class T>
struct TestSuiteRegistration {
	TestSuiteRegistration(const char *name)
	{
		Ref<T> suite = new T(name);
		TestSuite::getDefaultSuite().add(*suite);
	}
};


////////////////////////////////////////////////////////////////////////////////
// TEST CASE
////////////////////////////////////////////////////////////////////////////////


#define TEST_ADD(suite, method) registerTest(&suite::method, #method);

#define TEST_ASSERT(expr, message) \
	assert_((expr), __FILE__, __LINE__, #expr, message);

#define TEST_EQUALS(expected, got, message) { \
	VAR(expectedValue_, (expected)); \
	VAR(gotValue_,      (got)); \
	std::stringstream tmpStream_; \
	tmpStream_ << #expected " == " #got; \
	tmpStream_ << "  (" << expectedValue_ << " == " << gotValue_ << ")"; \
	assert_(expectedValue_ == gotValue_, \
	        __FILE__, __LINE__, \
	        tmpStream_.str().c_str(), message); \
}

#define TEST_FAIL(message) \
	assert_(false, __FILE__, __LINE__, NULL, message);


class TestCase : public TestSuite {
private:
	const char *name_;

protected:
	TestCase(const char *name) : name_(name)
	{}
	
	template<class T>
	void registerTest(void (T::*method)(), const char *name)
	{
		add(new TestRef_<T>(name, (T*)this, method));
	}
	
	void assert_(bool value,
			   const char *file, int line,
			   const char *assertion, const char *message);
	
public:
	const char* getName() const { return name_; }
	
	virtual void setUp() {}
	virtual void tearDown() {}
	
	void reset();
};


////////////////////////////////////////////////////////////////////////////////
// TEST RUNNER
////////////////////////////////////////////////////////////////////////////////


class TestRunner {
private:
	int testCount_;
	int failureCount_;
	
protected:
	virtual void startTests(const TestSuite &tests) {}
	virtual void finishTests(const TestSuite &tests) {}
	virtual void startTest(Ref<TestRef> test) {}
	virtual void addTestResult(Ref<TestRef> test, TestResult result) {}

public:
	int getTestCount() const { return testCount_; }
	int getFailureCount() const { return failureCount_; }
	
	virtual void run(const TestSuite &tests);
};


class TextTestRunner : public TestRunner {
private:
	std::ostream *output_;

protected:
	virtual void startTests(const TestSuite &tests);
	virtual void finishTests(const TestSuite &tests);
	
	virtual void startTest(Ref<TestRef> test);
	virtual void addTestResult(Ref<TestRef> test, TestResult result);

public:
	TextTestRunner(std::ostream *output) : output_(output) {}
	TextTestRunner() : output_(&std::cout) {}
	virtual ~TextTestRunner() { output_ = NULL; }
};


////////////////////////////////////////////////////////////////////////////////
// TEST TEST CASE
////////////////////////////////////////////////////////////////////////////////


class TestTestCase : public TestCase {
public:
	TestTestCase(const char *name);
	
	void noFailureTest();
	void unconditionalFailureTest();
	void simpleFailureTest();
	void exceptionFailureTest();
	void equalsAssertionTest();
	void equalsAssertionFailureTest();
};


} /* namespace cppapp */


#endif /* end of include guard: TEST_N6WIX7RO */

