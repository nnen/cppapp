/**
 * \file   Output.h
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2012-10-03
 *
 * \brief Header file for the Output class.
 */

#ifndef OUTPUT_CEN4W49F
#define OUTPUT_CEN4W49F


#include <string>
#include <iostream>
#include <fstream>

#include "Object.h"


using namespace std;


namespace cppapp {


class Output : public Object {
public:
	virtual ~Output();
	
	virtual string getName() = 0;
	virtual ostream* getStream() = 0;
	virtual void close() {}
	
	inline operator ostream*() { return getStream(); }
	inline operator ostream&() { return *getStream(); }
};


class StandardOutput : public Output {
public:
	StandardOutput() {}
	
	virtual string getName() { return "<stdout>"; }
	virtual ostream* getStream() { return &std::cout; }
};


class StandardErrorOutput : public Output {
public:
	StandardErrorOutput() {}
	
	virtual string getName() { return "<stderr>"; }
	virtual ostream* getStream() { return &std::cerr; }
};


class FileOutput : public Output {
private:
	string             fileName;
	ios_base::openmode mode;
	ofstream           out;

public:
	FileOutput(string fileName,
	           ios_base::openmode mode = ios_base::out | ios_base::binary);
	
	virtual string getName() { return fileName; }
	virtual ostream* getStream();
	virtual void close();
};


} // namespace cppapp


#endif /* end of include guard: OUTPUT_CEN4W49F */

