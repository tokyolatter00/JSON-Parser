
#pragma once

#include <stdio.h>

#define TRUE 1
#define FALSE 0

#define NO_ERROR ErrorInit()

/*
	Error Types

	MACROS:

	> ERR_INVALID_SYNTAX
	Syntax error has been found in a json source string. 

	> ERR_UNTERMINATED_STRING_LITERAL
	A string escape character "\" was used and the next character was not a valid escape character (", t, n, \)

	> ERR_ACCESS_PATH_FAILURE
	Access to a filepath has failed. 

	> ERR_TOKEN_NOT_RECOGNISED
	An unrecognised token has been found in a json source string. 

	> ERR_KEYWORD_NOT_RECOGNISED
	An unrecognised keyword has been found in a json source string. 

	> ERR_UNEXPECTED_TYPE
	An unexpected type was encountered when parsing a json source string. The parser expected
	a certain type but got another. 

	> ERR_UNEXPECTED_EOF
	An unexpected EOF was encountered when parsing a json source string. The parser did not expect
	the json source string to end where it had ended. 

	> ERR_INVALID_INT
	An invalid integer has been found in a json source string. This integer either is not a real integer
	or exceeds the memory boundaries of the integer used to store it. 

	> ERR_INVALID_FLOAT
	An invalid float has been found in a json source string. This float either is not a real integer or
	exceeds the memory boundaries of the float used to store it. 
*/

#define ERR_INVALID_SYNTAX				"invalid syntax"
#define ERR_UNTERMINATED_STRING_LIERAL	"unterminated string literal"
#define ERR_ACCESS_PATH_FAILURE			"failed to access filepath '%s'"
#define ERR_TOKEN_NOT_RECOGNISED		"'%c' is not a recognised token"
#define ERR_KEYWORD_NOT_RECOGNISED		"'%s' is not a valid JSON keyword"
#define ERR_UNEXPECTED_TYPE				"expected type '%s', got type '%s'"
#define ERR_UNEXPECTED_EOF				"unexpected eof"
#define ERR_INVALID_INT					"integer '%s' is not a valid 64 bit integer"
#define ERR_INVALID_FLOAT				"float '%s' is not a valid 64 bit float"

/*
	Error Raising

	MACROS:

	> CREATE_ERROR()
	Creates an error called "error" where it is called. The caller will have to refer to the error made as "error".

	> RAISE_FATAL_ERROR()
	Creates an error and loads it to a struct with an Error as a field. Examples of these structs are Lexers and
	Parsers.

	> ASSERT()
	Asserts a certain condition. If the condition is false then the assertion failed and it raises an error with the
	arguments passed to it. 
*/

#define CREATE_ERROR(errstr, ...)							\
	static char buffer[256];								\
	sprintf_s(buffer, 256, errstr, ##__VA_ARGS__);			\
	Error* error = ErrorInit();								\
	error->DebugStr = buffer;								\
	error->Exists = TRUE;

#define RAISE_FATAL_ERROR(obj, errstr, ...)					\
	static char buffer[256];								\
	sprintf_s(buffer, 256, errstr, ##__VA_ARGS__);			\
	obj->Error->DebugStr = buffer;							\
	obj->Error->Exists = TRUE;

#define ASSERT(obj, condition, errstr, ...)					\
	if (!(condition)) {										\
		RAISE_FATAL_ERROR(obj, errstr, ##__VA_ARGS__);		\
	}

#define ErrorDelete(error)									\
	free(error);

typedef struct {
	const char* DebugStr;
	int Exists;
} Error;

/*
	Initializing Data
*/

Error* ErrorInit();
