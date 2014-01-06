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
#include "Input.h"
#include "DynObject.h"
#include "utils.h"


namespace cppapp {


//// JSONParser /////////////////////////////////////////////////////

class JSONParser {
private:
	Lexer lexer;
	
	Ref<DynError> returnError(TextLoc location);
	Ref<DynError> returnError(const char *fn, int line);
	
	bool readObject(Ref<DynObject> *result);
	
	bool readDict(Ref<DynObject> *result);
	bool readKeyValue(Ref<DynString> *key, Ref<DynObject> *value);
	
	bool readList(Ref<DynObject> *result);
	bool readString(Ref<DynObject> *result);
	bool readNumber(Ref<DynObject> *result);
	bool readBool(Ref<DynObject> *result);

public:
	Ref<DynObject> parse(std::istream *input, std::string fileName);
	Ref<DynObject> parse(Ref<Input> input);
	
	static bool strToBool(Lexer *lexer, double *result);
	static bool strToDouble(Lexer *lexer, double *result);
};


} // namespace cppapp


#endif /* end of include guard: JSON_2HV25IX3 */

