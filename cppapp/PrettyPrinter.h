/**
 * \file   PrettyPrinter.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2014-01-04
 *
 * \brief  Header file for the PrettyPrinter class.
 */

#ifndef PRETTYPRINTER_6ZJFEZ1C
#define PRETTYPRINTER_6ZJFEZ1C


#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdarg>

#include "Object.h"
#include "Output.h"
#include "string_utils.h"


namespace cppapp {


/**
 * \todo Write documentation for class PrettyPrinter.
 */
class PrettyPrinter : public Object {
private:
	Ref<Output>              output_;
	
	std::vector<std::string> indent_;
	bool                     atNewLine_;
	int                      currentIndent_;
	
	void print(const std::string &value, bool isLine);
	
	/**
	 * Copy constructor.
	 */
	PrettyPrinter(const PrettyPrinter& other);

public:
	/**
	 * Constructor.
	 */
	PrettyPrinter();
	/**
	 * Constructor.
	 */
	PrettyPrinter(Ref<Output> output);
	
	/**
	 * Destructor.
	 */
	virtual ~PrettyPrinter() {}
	
	void indent(const char *value);
	void indent(const std::string &value);
	void indent();
	void indentCurrent();
	void unindent();
	
	void printIndent();
	void print(const std::string &value);
	void printf(const char *format, ...);
};


} // namespace cppapp


#endif /* end of include guard: PRETTYPRINTER_6ZJFEZ1C */

