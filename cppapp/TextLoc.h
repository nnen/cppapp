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

#endif /* end of include guard: TEXTLOC_ON5JQBQJ */

