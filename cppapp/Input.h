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


/**
 * \brief Returns the basename part of a file path.
 *
 * For example:
 * \code{.cpp}
 * pathBasename("/path/to/a/file"); // returns "file"
 * \endcode
 *
 * \param   path a file path
 * \returns      basename part of a path
 *
 * \deprecated Use \ref Path::basename().
 */
string pathBasename(const string &path);
/**
 * \brief Returns the entire path without the file extension.
 *
 * If the path does not contain extension, the path itself is returned.
 *
 * For example:
 * \code{.cpp}
 * pathWithoutExtension("/path/to/a.file"); // returns "/path/to/a"
 * pathWithoutExtension("/path/to/file"); // returns "/path/to/file"
 * \endcode
 *
 * \param   path a file path
 * \returns      path without the file extension
 */
string pathWithoutExtension(const string &path);
/**
 * \brief Returns the entire path with the extension substituted for \p extension.
 *
 * For example:
 * \code{.cpp}
 * pathWithExtension("/path/to/a.file", "xxx"); // returns "/path/to/a.xxx"
 * \endcode
 *
 * \param path      a file path
 * \param extension an extension to be used without dot
 * \returns         \p path with the extension substituted for \p extension
 */
string pathWithExtension(const string &path, const string &extension);


/**
 * \brief Represents an abstract input.
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
 * \brief Represents standard input.
 */
class StandardInput : public Input {
private:

public:
	StandardInput() {}
	
	virtual string getName() { return "<stdin>"; }
	virtual istream* getStream() { return &std::cin; }
};


/**
 * \brief Represents an input read from a file.
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


/**
 * \brief Represents an input read from a string.
 */
class StreamInput : public Input {
private:
	std::string   name_;
	std::istream *stream_;
	bool          ownsStream_;

public:
	StreamInput(std::string name, std::istream *stream, bool ownsStream);
	StreamInput(std::string name, std::istream *stream);
	StreamInput(std::string name, std::istream &stream);
	StreamInput(std::string name, std::string str);
	StreamInput(std::string str);
	virtual ~StreamInput();
	
	virtual std::string getName() { return name_; }
	
	virtual istream* getStream() { return stream_; }
	
	virtual void close();
};


} // namespace cppapp


#endif /* end of include guard: INPUT_NM268LPU */

