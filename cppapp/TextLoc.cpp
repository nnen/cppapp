/**
 * \file   TextLoc.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-12-19
 * 
 * \brief  Implementation file for the TextLoc class.
 */

#include "TextLoc.h"


namespace cppapp {


std::ostream& operator<<(std::ostream &out, const TextLoc &loc)
{
	out << loc.fileName << ":" << loc.line;
	return out;
}


} // namespace cppapp


