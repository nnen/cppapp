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
#include <algorithm> 
#include <functional> 
#include <cctype>
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


#endif /* end of include guard: STRING_UTILS_GXVJSRP9 */

