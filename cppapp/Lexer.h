/**
 * \file   Lexer.h
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2013-12-06
 *
 * \brief  Header file for the Lexer class.
 */

#ifndef LEXER_1WKANT5X
#define LEXER_1WKANT5X


#include <cstring>

#include <istream>
#include <string>
#include <vector>

#include "TextLoc.h"


namespace cppapp {


/**
 * \todo Write documentation for class Lexer.
 */
class Lexer {
private:
	std::istream *input_;
	
	TextLoc           loc_;
	std::vector<char> buffer_;
	
	bool bufferChars(int length);
	
	/**
	 * Copy constructor.
	 */
	Lexer(const Lexer& other);

public:
	/**
	 * Constructor.
	 */
	Lexer();

	void input(std::istream *in, std::string fileName);
	
	TextLoc getLocation() const { return loc_; }
	
	int peek();
	int read();
	
	bool read(int expected, bool skipSpace = true);
	bool read(const char *expected, bool skipSpace = true);
	
	void skipWhitespace();
};


} // namespace cppapp


#endif /* end of include guard: LEXER_1WKANT5X */

