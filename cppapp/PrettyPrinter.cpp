/**
 * \file   PrettyPrinter.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2014-01-04
 * 
 * \brief  Implementation file for the PrettyPrinter class.
 */

#include "PrettyPrinter.h"


namespace cppapp {


void PrettyPrinter::print(const std::string &value, bool isLine)
{
	if (atNewLine_)
		printIndent();
	*(output_->getStream()) << value;
	
	if (isLine) {
		*(output_->getStream()) << std::endl;
		currentIndent_ = 0;
	} else {
		currentIndent_ += value.size();
	}
	atNewLine_ = isLine;
}


PrettyPrinter::PrettyPrinter() :
	output_(new StandardOutput()),
	atNewLine_(true)
{
	
}


PrettyPrinter::PrettyPrinter(Ref<Output> output) :
	output_(output),
	atNewLine_(true)
{
	
}


void PrettyPrinter::indent(const char *value)
{
	indent_.push_back(value);
}


void PrettyPrinter::indent(const std::string &value)
{
	indent_.push_back(value);
}


void PrettyPrinter::indent()
{
	indent("    ");
}


void PrettyPrinter::indentCurrent()
{
	std::stringstream s;
	for (int i = 0; i < currentIndent_; i++)
		s << " ";
	indent(s.str());
}


void PrettyPrinter::unindent()
{
	if (indent_.size() > 0)
		indent_.pop_back();
}


void PrettyPrinter::printIndent()
{
	for (size_t i = 0; i < indent_.size(); i++) {
		*(output_->getStream()) << indent_[i];
	}
}


void PrettyPrinter::print(const std::string &value)
{
	std::vector<std::string> lines;
	Strings::split(value, "\n", &lines);
	
	for (int i = 0; i < (lines.size() - 1); i++) {
		print(lines[i], true);
	}
	
	const std::string &last = lines[lines.size() - 1];
	print(last, Strings::endsWith(value, "\n"));
}


void PrettyPrinter::printf(const char *format, ...)
{
	int  frmtLen = strlen(format);
	char buffer[frmtLen + 1024];
	
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	
	print(buffer);
}


} // namespace cppapp


