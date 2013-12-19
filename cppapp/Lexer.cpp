/**
 * \file   Lexer.cpp
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2013-12-06
 * 
 * \brief  Implementation file for the Lexer class.
 */

#include "Lexer.h"


namespace cppapp {


Lexer::Lexer() :
	input_(NULL)
{
}


void Lexer::input(std::istream *in, std::string fileName)
{
	input_ = in;
	loc_   = TextLoc(fileName);
	buffer_.clear();
}


bool Lexer::bufferChars(int length)
{
	if ((int)buffer_.size() >= length)
		return true;

	while ((int)buffer_.size() < length) {
		if (!input_->good())
			return false;
		
		int c = input_->get();
		buffer_.push_back(c);
	}

	return true;
}


int Lexer::peek()
{
	if (!bufferChars(1))
		return -1;
	
	return buffer_[0];
}


int Lexer::read()
{
	int result = peek();
	if (result == -1)
		return result;
	
	if (result == '\n')
		loc_ = loc_.newLine();
	else
		loc_ = loc_ + 1;
	
	std::vector<char> newBuffer;
	newBuffer.insert(newBuffer.begin(), buffer_.begin() + 1, buffer_.end());
	buffer_.swap(newBuffer);
	
	return result;
}


bool Lexer::read(int expected, bool skipSpace)
{
	if (skipSpace) skipWhitespace();
	
	if (peek() != expected)
		return false;
	read();
	return true;
}


bool Lexer::read(const char *expected, bool skipSpace)
{
	if (skipSpace) skipWhitespace();
	
	int length = strlen(expected);
	
	if (!bufferChars(length))
		return false;
	
	for (int i = 0; i < length; i++) {
		if (buffer_[i] != expected[i])
			return false;
	}

	for (int i = 0; i < length; i++)
		read();
	
	return true;
}


void Lexer::skipWhitespace()
{
	while (isspace(peek())) read();
}


} // namespace cppapp


