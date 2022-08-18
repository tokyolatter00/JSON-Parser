
/*
	> parser.h
	Header file for defining a JSON Parser and functions which interact with it
	Documentation about the below functions can be found in parser.c
*/

#pragma once

#include "containers.h"
#include "error.h"

typedef struct {
	TokenArray* Tokens;
	Token* Token;
	ullong Index;
	Error* Error;
} Parser;


/*
	Initializing Data
*/

Parser* ParserInit(TokenArray* tokens);

/*
	Get Results
*/

JsonExpr* ParserGetResult(Parser* parser);
