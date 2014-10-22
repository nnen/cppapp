/**
 * \file   PathTest.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-06-26
 *
 * \brief  Header file for the PathTest class.
 */

#ifndef PATHTEST_LCLNOC97
#define PATHTEST_LCLNOC97


#include <cppapp/cppapp.h>
using namespace cppapp;


/**
 * \todo Write documentation for class PathTest.
 */
class PathTest : public TestCase {
public:
	/**
	 * Constructor.
	 */
	PathTest()
	{
		TEST_ADD(PathTest, testJoin);
	}
	
	void testJoin()
	{
		std::string path = Path::join("a", "b");
		TEST_EQUALS(std::string("a/b"), path,
				  "Simple path joining doesn't work.");
		
		path = Path::join("first", "second");
		TEST_EQUALS(std::string("first/second"), path,
				  "Simple path joining doesn't work.");
		
		path = Path::join("a/", "b");
		TEST_EQUALS(std::string("a/b"), path,
				  "Separator-ended path is not handled properly.");
		
		path = Path::join("first/", "second");
		TEST_EQUALS(std::string("first/second"), path,
				  "Separator-ended path is not handled properly.");
		
		path = Path::join("a", "/b");
		TEST_EQUALS(std::string("/b"), path,
				  "Absolute path is not handled properly.");
		
		path = Path::join("a/", "/b");
		TEST_EQUALS(std::string("/b"), path,
				  "Absolute path is not handled properly.");
		
		path = Path::join("/a/", "/b");
		TEST_EQUALS(std::string("/b"), path,
				  "Absolute path is not handled properly.");
	}
};

RUN_SUITE(PathTest);


#endif /* end of include guard: PATHTEST_LCLNOC97 */

