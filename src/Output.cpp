/**
 * \file   Output.cpp
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2012-10-03
 * 
 * \brief Implementation file for the Output class.
 */

#include "Output.h"


namespace cppapp {


///////////////////////////////////////////////////////////////////////////////
// OUTPUT
///////////////////////////////////////////////////////////////////////////////


Output::~Output() { close(); }


///////////////////////////////////////////////////////////////////////////////
// FILE OUTPUT
///////////////////////////////////////////////////////////////////////////////


FileOutput::FileOutput(string fileName, ios_base::openmode mode) :
	fileName(fileName), mode(mode)
{
}


ostream* FileOutput::getStream()
{
	if (!out.is_open())
		out.open(fileName.c_str(), mode);
	return &out;
}


void FileOutput::close()
{
	if (out.is_open()) out.close();
}


} // namespace cppapp

