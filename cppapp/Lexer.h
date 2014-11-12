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
#include "Input.h"


namespace cppapp {


/**
 * \brief Simple stream lexer to make parsing easier.
 */
class Lexer {
private:
	Ref<Input>    inputObj_;
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
	
	void input(Ref<Input> in);
	void input(std::string str);
	
	TextLoc getLocation() const { return loc_; }
	
	int peek();
	int read();
	
	bool read(int expected, bool skipSpace = true);
	bool read(const char *expected, bool skipSpace = true);
	
	void skipWhitespace();
	bool skipWhitespace(int expected);
};


} // namespace cppapp


#endif /* end of include guard: LEXER_1WKANT5X */

