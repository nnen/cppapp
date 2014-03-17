/**
 * \file   src/string_utils.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-04-29
 *
 * \brief  
 */

#ifndef STRING_UTILS_GXVJSRP9
#define STRING_UTILS_GXVJSRP9


#include <string> 
#include <vector> 
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <cstring>
#include <locale>


namespace cppapp {


template<class T>
inline size_t length(T value)
{
	return value.size();
}


template<>
inline size_t length<const char *>(const char *value)
{
	return strlen(value);
}


/**
 * \brief Contains utility methods for manipulating strings.
 */
struct Strings {
	
	/**
	 * \brief Returns length of a STL string.
	 *
	 * \returns length of a STL string
	 */
	static inline size_t length(const std::string &s)
	{
		return s.size();
	}
	
	/**
	 * \brief Returns length of a C-string.
	 *
	 * \returns length of a C-string
	 */
	static inline size_t length(const char *s) 
	{
		return strlen(s);
	}
	
	/**
	 * \brief Trim whitespace from the left end of a string.
	 */
	static inline std::string &ltrim(std::string &s) 
	{
        	s.erase(
			s.begin(),
			std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace)))
		);
		return s;
	}
	
	/**
	 * \brief Trim whitespace from the right end of a string.
	 */
	static inline std::string &rtrim(std::string &s) 
	{
		s.erase(
			std::find_if(
				s.rbegin(),
				s.rend(),
				std::not1(std::ptr_fun<int, int>(std::isspace))
			).base(),
			s.end()
		);
		return s;
	}
	
	/**
	 * \brief Trims whitespace from both ends of a string.
	 */
	static inline std::string &trim(std::string &s) 
	{
		return ltrim(rtrim(s));
	}
	
	/**
	 * \brief Converts a string to lowercase.
	 */
	static inline std::string toLower(const std::string &s) 
	{
		std::string result = s;
		std::transform(s.begin(), s.end(), result.begin(), ::tolower);
		return result;
	}
	
	/**
	 * \brief Converts a string to uppercase.
	 */
	static inline std::string toUpper(const std::string &s) 
	{
		std::string result = s;
		std::transform(s.begin(), s.end(), result.begin(), ::toupper);
		return result;
	}
	
	/**
	 * \brief Splits a string into parts by a separator.
	 *
	 * \param value     the string to be split
	 * \param separator string to split the \c value by
	 * \param output    pointer to a vector to which the
	 *                  resulting string parts will be stored
	 *
	 * \code
	 * std::vector<std::string> output;
	 * split("a,b,c", ",", &output);
	 * // vector contents: ["a", "b", "c"]
	 *
	 * split("abc", ",", &output);
	 * // vector contents: ["abc"]
	 * \endcode
	 */
	static inline void split(std::string value, std::string separator, std::vector<std::string> *output)
	{
		size_t pos = 0;
		
		while (pos < value.length()) {
			size_t found = value.find(separator, pos);
			
			if (found == std::string::npos) {
				output->push_back(value.substr(pos, value.length() - pos));
				return;
			}
			
			output->push_back(value.substr(pos, found - pos));
			pos = found + separator.length();
		}
	}
	
	/**
	 * \brief Returns \c true if \c needle occurs within \c haystack.
	 *
	 * \param haystack string to be searched for occurences of \c needle
	 * \param needle   string to search for in \c haystack
	 * \returns \c true if \c needle occurs in \c haystack, \c false otherwise
	 */
	template<class T>
	static inline bool contains(const std::string &haystack, T needle)
	{
		size_t pos = haystack.find(needle);
		return (pos != std::string::npos);
	}
	
	/**
	 * \brief Returns \c true if a string \c value ends with \c suffix.
	 *
	 * \param value  string whose end will be compared to \c suffix
	 * \param suffix string to which is compared to the end of \c value
	 * \returns \c true if the suffix of \c value is equal to \c suffix, \c false otherwise
	 */
	template<class T>
	static inline bool endsWith(const std::string &value, T suffix)
	{
		int l = length(suffix);
		return (value.compare(value.size() - l, l, suffix) == 0);
	}

};


} // namespace cppapp


#endif /* end of include guard: STRING_UTILS_GXVJSRP9 */

