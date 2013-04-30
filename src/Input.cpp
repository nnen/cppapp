/**
 * \file   Input.cpp
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2012-03-12
 * 
 * \brief Implementation file for the Input class.
 */

#include "Input.h"


namespace cppapp {


string pathWithoutExtension(const string &path)
{
	int dotIndex = path.find_last_of(".");
	return path.substr(0, dotIndex);
}


string pathWithExtension(const string &path, const string &extension)
{
	string result = pathWithoutExtension(path);
	result += ".";
	result += extension;
	return result;
}


///////////////////////////////////////////////////////////////////////////////
// FILE INPUT
///////////////////////////////////////////////////////////////////////////////


/**
 * Constructor.
 */
FileInput::FileInput(string fileName) :
	fileName_(fileName),
	stream_(fileName.c_str()) 
{
}


/**
 * Close file input.
 */
void FileInput::close()
{
	if (!stream_.is_open()) return;
	stream_.close();
}


bool FileInput::exists()
{
	struct stat fileInfo;
	return (stat(fileName_.c_str(), &fileInfo) == 0);
}


bool FileInput::hasExtension(string extension)
{
	return FileInput::hasExtension(fileName_, extension);
}


bool FileInput::hasExtension(string fileName, string extension)
{
	if (extension.size() < 1) return false;
	if (extension[0] != '.') extension = '.' + extension;
	if (extension.size() > fileName.size()) return false;
	
	return (fileName.substr(fileName.size() - extension.size(), extension.size()) == extension);
}


} // namespace cppapp

