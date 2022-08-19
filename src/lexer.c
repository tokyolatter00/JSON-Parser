
#include <stdio.h> //
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "include/lexer.h"

/*
	Advancing

	FUNCTIONS:

	> Advance()
	Advance to the next char in a Lexer's source string. Also sets the previous char in the Lexer

	> PeekNext()
	Peek ahead of the current Lexer's index without advancing

	> SkipWhitespace()
	Keep advancing until a character which is not a whitespace is found

	> AdvanceWith()
	Create a token from the current lexer's char and call Advance()
*/

static void Advance(Lexer* lexer) {
	lexer->PrevChar = lexer->Char;

	lexer->Char = lexer->Length > ++lexer->Index
		? lexer->Source[lexer->Index]
		: CHAR_EMPTY;
}

static char PeekNext(Lexer* lexer, ullong offset) {
	return lexer->Length > (offset = lexer->Index + offset)
		? lexer->Source[offset]
		: CHAR_EMPTY;
}

static void SkipWhitespace(Lexer* lexer) {
	while (isspace(lexer->Char)) {
		Advance(lexer);
	}
}

static Token* AdvanceWith(Lexer* lexer, TokenType type) {
	Advance(lexer);
	return TokenInit(NULL, type);
}

/*
	String Evaluation

	MACROS:

	> COMPARE_KEYWORDS
	Returns 1 if a string is equal to a keyword

	FUNCTIONS:

	> EvaluateKeyword
	Returns the keyword that a string represents. If there is no keyword then the string is not a valid
	keyword and an error is thrown in the Lexer
*/

#define COMPARE_KEYWORD(str, keyword) strcmp(str, keyword) == 0

static TokenType EvaluateKeyword(Lexer* lexer, const char* str) {
	if (COMPARE_KEYWORD(str, KEYWORD_TRUE)) {
		return TOKEN_TRUE;
	}
	else if (COMPARE_KEYWORD(str, KEYWORD_FALSE)) {
		return TOKEN_FALSE;
	}
	else if (COMPARE_KEYWORD(str, KEYWORD_NULL)) {
		return TOKEN_NULL;
	}

	RAISE_FATAL_ERROR(
		lexer,
		ERR_KEYWORD_NOT_RECOGNISED,
		str
	);
}

/*
	Data Building

	MACROS:

	> IS_STRING
	Returns 1 if a char is a valid string type

	> IS_KEYWORD
	Returns 1 if a char is a valid keyword type

	> IS_NUMERICAL
	Returns 1 if a char is a valid numerical type

	> IS_ESCAPE_SEQ
	Returns 1 if a char is a valid escape sequence type (e.g., \n)

	> ESCAPE_STRING
	String escape \" is used to register a '"' character into the string

	> ESCAPE_TAB
	String escape \t is used to register a tab (multiple spaces) into the string

	> ESCAPE_NEWLINE
	String escape \n is used to register a newline into the string

	> ESCAPE_BACKSLASH
	String escape \\ is used to register a '\' into the string

	FUNCTIONS:

	> ScanStringSize()
	Scans the size of a string without advancing. This is used to tell the caller how many bytes should be
	allocated when trying to store the string. Supports scanning for string escape characters such as '\n'

	> ScanKeywordSize()
	Scans the size of a keyword without advancing. This is used to tell the caller how many bytes should be
	allocated when trying to store the keyword

	> ScanNumberSize()
	Scans the size of a number without advancing. This is used to tell the caller how many bytes should be
	allocated when trying to store the number

	> BuildString()
	Builds a string from characters in a Lexer's source by advancing. Supports recognition for string escape
	characters such as '\n'. 

	> BuildKeyword()
	Builds a keyword from characters in a Lexer's source by advancing

	> BuildNumber()
	Builds a number from characters in a Lexer's source by advancing
*/

#define IS_STRING(chr) chr != CHAR_QUOTE && chr != CHAR_EMPTY
#define IS_KEYWORD(chr) isalpha(chr) && chr != CHAR_EMPTY
#define IS_NUMERICAL(chr) isdigit(chr) || chr == '.' && chr != CHAR_EMPTY
#define IS_ESCAPE_SEQ(chr) chr == 't' || chr == 'n' && chr != CHAR_EMPTY

#define ESCAPE_STRING '"'
#define ESCAPE_TAB 't'
#define ESCAPE_NEWLINE 'n'
#define ESCAPE_BACKSLASH '\\'

static ullong ScanStringSize(Lexer* lexer) {
	ullong size = 0;
	ullong offset = 0;
	char chr = lexer->Char;

	while (IS_STRING(chr)) {
		if (chr == CHAR_ESCAPE) {
			chr = PeekNext(lexer, offset++);
		}

		size++;
		chr = PeekNext(lexer, offset++);
	}

	return size;
}

static ullong ScanKeywordSize(Lexer* lexer) {
	ullong size = 0;
	ullong offset = 0;
	char chr = lexer->Char;

	while (IS_KEYWORD(chr)) {
		size++;
		chr = PeekNext(lexer, offset++);
	}

	return size;
}

static ullong ScanNumberSize(Lexer* lexer) {
	ullong size = 0;
	ullong offset = 0;
	char chr = lexer->Char;

	while (IS_NUMERICAL(chr)) {
		size++;
		chr = PeekNext(lexer, offset++);
	}

	return size;
}

static Token* BuildString(Lexer* lexer) {
	ullong size = ScanStringSize(lexer);
	char* value = malloc(size + 1);
	ullong index = 0;
	
	while (IS_STRING(lexer->Char)) {
		if (lexer->Char == CHAR_ESCAPE) {
			switch (PeekNext(lexer, 1)) {
				case ESCAPE_STRING:
					value[index++] = CHAR_ESCAPE_QUOTE;
					Advance(lexer);
					break;
				case ESCAPE_TAB:
					value[index++] = CHAR_ESCAPE_TAB;
					Advance(lexer);
					break;
				case ESCAPE_NEWLINE:
					value[index++] = CHAR_ESCAPE_NEWLINE;
					Advance(lexer);
					break;
				case ESCAPE_BACKSLASH:
					value[index++] = CHAR_ESCAPE;
					Advance(lexer);
					break;
				default:
					{
						RAISE_FATAL_ERROR(
							lexer,
							ERR_UNTERMINATED_STRING_LIERAL
						);
					}
					break;
			}
		}
		else {
			value[index++] = lexer->Char;
		}

		Advance(lexer);
	}

	value[index] = '\0';
	return TokenInit(value, TOKEN_STRING);
}

static Token* BuildKeyword(Lexer* lexer) {
	ullong size = ScanKeywordSize(lexer);
	char* value = malloc(size + 1);
	int index = 0;
	
	while (IS_KEYWORD(lexer->Char)) {
		value[index++] = lexer->Char;
		Advance(lexer);
	}

	value[index] = '\0';
	TokenType type = EvaluateKeyword(lexer, value);
	free(value);

	return TokenInit(NULL, type);
}

static Token* BuildNumber(Lexer* lexer) {
	ullong size = ScanNumberSize(lexer);
	char* value = malloc(size + 1);
	int index = 0;
	int decimals = 0;

	while (IS_NUMERICAL(lexer->Char)) {
		value[index++] = lexer->Char;
		Advance(lexer);

		if (lexer->Char == '.') {
			decimals++;
		}
	}

	value[index] = '\0';
	return TokenInit(value, decimals ? TOKEN_FLOAT : TOKEN_INT);
}

/*
	Reading Tokens

	FUNCTIONS:

	> GetNextToken()
	Gets the next token from the current character in a Lexer's source string. Data can be built off of these characters,
	such as (e.g., strings, keywords and numbers) if it is appropriate to do so. 
*/

Token* GetNextToken(Lexer* lexer) {
	SkipWhitespace(lexer);

	switch (lexer->Char) {
		case CHAR_LCURLY:
			return AdvanceWith(lexer, TOKEN_LCURLY);
		case CHAR_RCURLY:
			return AdvanceWith(lexer, TOKEN_RCURLY);
		case CHAR_LBRACKET:
			return AdvanceWith(lexer, TOKEN_LBRACKET);
		case CHAR_RBRACKET:
			return AdvanceWith(lexer, TOKEN_RBRACKET);
		case CHAR_COLON:
			return AdvanceWith(lexer, TOKEN_COLON);
		case CHAR_COMMA:
			return AdvanceWith(lexer, TOKEN_COMMA);
		case CHAR_QUOTE:
			return AdvanceWith(lexer, TOKEN_QUOTE);
		case CHAR_MINUS:
			return AdvanceWith(lexer, TOKEN_MINUS);
		case CHAR_EMPTY:
			return TokenInit(NULL, TOKEN_EOF);
	}

	if (lexer->PrevChar == CHAR_QUOTE) {
		return BuildString(lexer);
	}
	else if (IS_KEYWORD(lexer->Char)) {
		return BuildKeyword(lexer);
	}
	else if (IS_NUMERICAL(lexer->Char)) {
		return BuildNumber(lexer);
	}

	RAISE_FATAL_ERROR(lexer, ERR_TOKEN_NOT_RECOGNISED, lexer->Char);
}

/*
	Initializing Data

	FUNCTIONS:

	> LexerInit()
	Initialize a Lexer object
*/

Lexer* LexerInit(const char* source) {
	Lexer* lexer = calloc(1, sizeof(Lexer));
	lexer->Source = source;
	lexer->Length = strlen(source);
	lexer->Index = 0;
	lexer->Char = source[0];
	lexer->Error = ErrorInit();

	return lexer;
}

/*
	Get Results

	FUNCTIONS:

	> LexerGetResult()
	Use a Lexer object to generate a TokenArray object
*/

TokenArray* LexerGetResult(Lexer* lexer) {
	TokenArray* tokens = TokenArrayInit();
	TokenType type;
	
	do {
		Token* token = GetNextToken(lexer);

		if (lexer->Error->Exists) {
			return tokens;
		}

		type = token->Type;
		TokenArrayAppend(tokens, *token);
		free(token);
	} while (type != TOKEN_EOF);

	return tokens;
}
