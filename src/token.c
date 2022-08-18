
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/token.h"
#include "include/containers.h"

/*
	Initializing Data

	FUNCTIONS:

	> TokenInit()
	Initialize a Token object
*/

Token* TokenInit(const char* value, TokenType type) {
	Token* token = calloc(1, sizeof(Token));
	token->Value = value;
	token->Type = type;

	return token;
}

/*
	Serializing Data

	FUNCTIONS:

	> TokenTypeToString()
	Turns a TokenType to a string

	> TokenToString()
	Turns a Token to a string

	NOTEs:

	These functions are used for debugging purposes
*/

const char* TokenTypeToString(TokenType type) {
	switch (type) {
		case TOKEN_LCURLY:
			return "LCURLY";
		case TOKEN_RCURLY:
			return "RCURLY";
		case TOKEN_LBRACKET:
			return "LBRACKET";
		case TOKEN_RBRACKET:
			return "RBRACKET";
		case TOKEN_COLON:
			return "COLON";
		case TOKEN_COMMA:
			return "COMMA";
		case TOKEN_QUOTE:
			return "QUOTE";
		case TOKEN_STRING:
			return "STRING";
		case TOKEN_INT:
			return "INT";
		case TOKEN_FLOAT:
			return "FLOAT";
		case TOKEN_TRUE:
			return "TRUE";
		case TOKEN_FALSE:
			return "FALSE";
		case TOKEN_NULL:
			return "NULL";
		case TOKEN_MINUS:
			return "MINUS";
		case TOKEN_EOF:
			return "EOF";
		default:
			return "UNRECOGNISED_TYPE";
	}
}

const char* TokenToString(Token* token) {
	StringBuilder* builder = StringBuilderInit();
	const char* typestr = TokenTypeToString(token->Type);
	const char* valuestr;

	if (!token->Value) {
		valuestr = "(null)";
	}
	else if (strlen(token->Value) > 128) {
		valuestr = "(trunctuated)";
	}
	else {
		valuestr = token->Value;
	}

	StringBuilderAppendChar(builder, '[');
	StringBuilderAppendString(builder, "VALUE: ");
	StringBuilderAppendString(builder, valuestr);
	StringBuilderAppendString(builder, ", TYPE: ");
	StringBuilderAppendString(builder, typestr);
	StringBuilderAppendChar(builder, ']');

	char* buffer = builder->Buffer;
	free(builder);
	return buffer;
}
