/**
 * \file   Path.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2014-01-12
 * 
 * \brief  Implementation file for the Path class.
 */

#include "Path.h"


namespace cppapp {


const char Path::SEPARATOR = '/';


bool Path::isAbsolute(const std::string &path)
{
	if (path.size() < 1) return false;
	return path[0] == SEPARATOR;
}


/**
 * \fn std::string pathBasename(const std::string &path)
 *
 * \note The function uses the basename function from the libgen.h header.
 * For more information, see <tt>man 3 basename</tt>.
 */
std::string Path::basename(const std::string &path)
{
	char *p = new char[path.size() + 1];
	path.copy(p, path.size());
	p[path.size()] = 0;
	
	char *bn = ::basename(p);
	std::string result(bn);
	delete [] p;
	
	return result;
}


std::string Path::join(const std::string &a, const std::string &b)
{
	if (isAbsolute(b))
		return b;
	
	bool addSeparator = false;
	if (a.size() > 1 && (a[a.size() - 1] != SEPARATOR))
		addSeparator = true;
	
	std::ostringstream ss;
	ss << a;
	ss.put(SEPARATOR);
	ss << b;
	
	return ss.str();
}


} // namespace cppapp


