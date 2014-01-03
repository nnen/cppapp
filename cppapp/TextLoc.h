/**
 * \file   TextLoc.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-12-19
 *
 * \brief  Header file for the TextLoc class.
 */

#ifndef TEXTLOC_ON5JQBQJ
#define TEXTLOC_ON5JQBQJ

#include <string>
#include <ostream>


namespace cppapp {


/**
 * \todo Write documentation for class TextLoc.
 */
struct TextLoc {
	std::string fileName;
	int         line;
	int         column;
	
	TextLoc() :
		fileName(), line(1), column(0)
	{}
	
	TextLoc(std::string fileName) :
		fileName(fileName), line(1), column(0)
	{}
	
	TextLoc(std::string fileName, int line) :
		fileName(fileName), line(line), column(0)
	{}
	
	TextLoc(std::string fileName, int line, int column) :
		fileName(fileName), line(line), column(column)
	{}
	
	TextLoc newLine() { return TextLoc(fileName, line + 1, 0); }
	
	TextLoc operator+(int rhs)
	{ 
		return TextLoc(fileName, line, column + 1);
	}
};


std::ostream& operator<<(std::ostream &out, const TextLoc &loc);


#define CPPAPP_TEXT_LOC TextLoc(__FILE__, __LINE__)


} // namespace cppapp


#endif /* end of include guard: TEXTLOC_ON5JQBQJ */

