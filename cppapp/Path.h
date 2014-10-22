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
#include <sys/stat.h>
#include <errno.h>


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
	Path() :
		value_(".")
	{}
	/**
	 * \brief Copy constructor.
	 */
	Path(const Path& other) : 
		value_(other.value_)
	{}
	Path(const std::string &value) :
		value_(value)
	{}
	Path(const char *value) :
		value_(value)
	{}
	/**
	 * \brief Destructor.
	 */
	~Path() {}
	
	const std::string& toString() const { return value_; }
	
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


struct FileInfo {
	Path        path;
	struct stat data;
	int         status;
	int         error;
	
	FileInfo();
	FileInfo(const Path &path);
	FileInfo(const std::string &path);
	FileInfo(const char *path);
	
	inline bool exists() const { return status == 0; }
	
	inline static bool exists(const std::string &path) {
		FileInfo info(path);
		return info.exists();
	}
};


} // namespace cppapp


#endif /* end of include guard: PATH_TMZPR80T */

