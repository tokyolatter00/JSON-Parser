
/*
	> lexer.h
	Header file for defining a JSON Lexer and functions which interact with it
	Documentation about the below functions can be found in lexer.c
*/

#pragma once

#include "containers.h"
#include "error.h"

typedef struct {
	const char* Source;
	ullong Length;
	ullong Index;
	char Char;
	char PrevChar;
	Error* Error;
} Lexer;

/*
	Initializing Data
*/

Lexer* LexerInit(const char* source);

/*
	Get Results
*/

TokenArray* LexerGetResult(Lexer* lexer);
