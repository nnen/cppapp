/**
 * \file   Input.h
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2012-03-12
 *
 * \brief Header file for the Input class.
 */

#ifndef INPUT_NM268LPU
#define INPUT_NM268LPU

#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>

#include "Object.h"


using namespace std;


namespace cppapp {


string pathWithoutExtension(const string &path);
string pathWithExtension(const string &path, const string &extension);


/**
 * Represents an abstract input.
 */
class Input : public Object {
private:
	Input(const Input& other);
	
public:
	Input() {}
	virtual ~Input() { close(); }
	
	virtual string getName() = 0;
	
	virtual istream* getStream() = 0;
	
	virtual void close() {}
	
	inline istream* operator->() { return getStream(); }
	
	
	virtual string getFileNameWithExt(const string &extension)
	{
		string result = "output.";
		result += extension;
		return result;
	}
};


/**
 * Represents standard input.
 */
class StandardInput : public Input {
private:

public:
	StandardInput() {}
	
	virtual string getName() { return "<stdin>"; }
	virtual istream* getStream() { return &std::cin; }
};


/**
 * Represents an input read from a file.
 */
class FileInput : public Input {
private:
	string   fileName_;
	ifstream stream_;
	
public:
	FileInput(string fileName);
	virtual ~FileInput() { }
	
	virtual string getName() { return fileName_; }
	
	virtual istream* getStream() { return &stream_; }
	
	virtual void close();
	
	bool exists();
	
	bool hasExtension(string extension);
	static bool hasExtension(string fileName, string extension);
	
	
	virtual string getFileNameWithExt(const string &extension)
	{
		return pathWithExtension(fileName_, extension);
	}
};


} // namespace cppapp


#endif /* end of include guard: INPUT_NM268LPU */

