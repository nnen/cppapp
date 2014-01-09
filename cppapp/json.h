/**
 * \file   json.h
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2013-12-04
 *
 * \brief  Header file for the json class.
 */

#ifndef JSON_2HV25IX3
#define JSON_2HV25IX3


#include <string>
#include <vector>
#include <map>
#include <istream>
#include <sstream>

#include "Object.h"
#include "Lexer.h"
#include "DynObject.h"
#include "utils.h"


namespace cppapp {


//// JSONParser /////////////////////////////////////////////////////


/**
 * \brief JSON parser for an extended JSON syntax.
 *
 * The sytax extensions include:
 * \li any number of commas allowed after each array item and key-value
 *     pair in an object including the last one (<tt>[1, 2, ]</tt>),
 * \li C++-style line comments (<tt>// a comment</tt>),
 * \li capital letters allowed in literals (<tt>True</tt>).
 */
class JSONParser {
private:
	Lexer lexer;
	
	Ref<DynError> returnError(const char *fn, int line);
	
	void skipWhitespace();
	
	bool readObject(Ref<DynObject> *result);
	
	bool readDict(Ref<DynObject> *result);
	bool readKeyValue(Ref<DynString> *key, Ref<DynObject> *value);
	
	bool readList(Ref<DynObject> *result);
	bool readString(Ref<DynObject> *result);
	bool readNumber(Ref<DynObject> *result);
	bool readBool(Ref<DynObject> *result);
	bool readNull(Ref<DynObject> *result);

public:
	/**
	 * \brief Parse a single JSON value and return a reference to it.
	 *
	 * \param input input stream to read from
	 * \param fileName name of the file the input comes from, this
	 *                 value is stored with the parsed values as
	 *                 metadata
	 */
	Ref<DynObject> parse(std::istream *input, std::string fileName);
	
	static bool strToBool(Lexer *lexer, double *result);
	static bool strToDouble(Lexer *lexer, double *result);
};


} // namespace cppapp


#endif /* end of include guard: JSON_2HV25IX3 */

