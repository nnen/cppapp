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


// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}


// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}


// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}


static inline std::string toLower(const std::string &s) {
	std::string result = s;
	std::transform(s.begin(), s.end(), result.begin(), ::tolower);
	return result;
}


static inline std::string toUpper(const std::string &s) {
	std::string result = s;
	std::transform(s.begin(), s.end(), result.begin(), ::toupper);
	return result;
}


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


struct Strings {
	
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
	
	template<class T>
	static inline bool contains(const std::string &haystack, T needle)
	{
		size_t pos = haystack.find(needle);
		return (pos != std::string::npos);
	}
	
	template<class T>
	static inline bool endsWith(const std::string &value, T suffix)
	{
		int l = length(suffix);
		return (value.compare(value.size() - l, l, suffix) == 0);
	}

};


#endif /* end of include guard: STRING_UTILS_GXVJSRP9 */

