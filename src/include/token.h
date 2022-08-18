
#pragma once

// Characters

#define CHAR_LCURLY '{'
#define CHAR_RCURLY '}'
#define CHAR_LBRACKET '['
#define CHAR_RBRACKET ']'
#define CHAR_COLON ':'
#define CHAR_COMMA ','
#define CHAR_QUOTE '"'
#define CHAR_MINUS '-'

// Escape Sequences

#define CHAR_ESCAPE '\\'
#define CHAR_ESCAPE_QUOTE '\"'
#define CHAR_ESCAPE_TAB '\t'
#define CHAR_ESCAPE_NEWLINE '\n'
#define CHAR_EMPTY '\0'

// Keywords

#define KEYWORD_TRUE "true"
#define KEYWORD_FALSE "false"
#define KEYWORD_NULL "null"

typedef enum {
	TOKEN_LCURLY,
	TOKEN_RCURLY,
	TOKEN_LBRACKET,
	TOKEN_RBRACKET,
	TOKEN_COLON,
	TOKEN_COMMA,
	TOKEN_QUOTE,
	TOKEN_STRING,
	TOKEN_INT,
	TOKEN_FLOAT,
	TOKEN_TRUE,
	TOKEN_FALSE,
	TOKEN_NULL,
	TOKEN_MINUS,
	TOKEN_EOF
} TokenType;

typedef struct {
	const char* Value;
	TokenType Type;
} Token;

/*
	Initializing Data
*/

Token* TokenInit(const char* value, TokenType type);

/*
	Serialising Data
*/

const char* TokenTypeToString(TokenType type);
const char* TokenToString(Token* token);
