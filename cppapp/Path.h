/**
 * \file   Path.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2014-01-12
 *
 * \brief  Header file for the Path class.
 */

#ifndef PATH_TMZPR80T
#define PATH_TMZPR80T


#include <string>
#include <sstream>
#include <libgen.h>


namespace cppapp {


/**
 * \todo Write documentation for class Path.
 */
class Path {
public:
	static const char SEPARATOR;

private:
	std::string value_;
	
public:
	/**
	 * \brief Constructor.
	 */
	Path();
	/**
	 * \brief Copy constructor.
	 */
	Path(const Path& other) : 
		value_(other.value_)
	{}
	/**
	 * \brief Destructor.
	 */
	virtual ~Path();
	
	/**
	 * \brief Returns \c true if the given path is absolute, \c false otherwise.
	 */
	static bool isAbsolute(const std::string &path);
	
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
	 */
	static std::string basename(const std::string &path);
	
	/**
	 * \brief Returns two paths joined together.
	 *
	 * \param a first path
	 * \param b second path
	 * \returns paths \c a + \c b joined together
	 */
	static std::string join(const std::string &a, const std::string &b);

};


} // namespace cppapp


#endif /* end of include guard: PATH_TMZPR80T */

