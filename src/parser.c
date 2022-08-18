
#include <stdlib.h>
#include "include/parser.h"
#include "include/converters.h"

static JsonList* ParseList(Parser* parser);
static JsonExpr* ParseExpr(Parser* parser);

/*
	> Advancing

	MACROS:

	> ADVANCE_NO_ARGS
	Used for skipping a TokenType check in Advance()

	FUNCTIONS:

	> Advance()
	Advance to the next token in a Parser's TokenArray. Also asserts the type of the current TokenType to ensure
	that unexpected TokenTypes do not occur (for example, if the caller is expecting the current TokenType to be
	a TOKEN_QUOTE but it is a TOKEN_COMMA it will raise an error in the Parser. ADVANCE_NO_ARGS can be passed in
	when the function caller wants this check to be skipped
*/

#define ADVANCE_NO_ARGS -1

static void Advance(Parser* parser, TokenType type) {
	if (type != ADVANCE_NO_ARGS) {
		ASSERT(
			parser,
			parser->Token->Type == type,
			ERR_UNEXPECTED_TYPE,
			TokenTypeToString(parser->Token->Type),
			TokenTypeToString(type)
		);
	}

	parser->Token = parser->Tokens->Length > ++parser->Index
		? &parser->Tokens->Buffer[parser->Index]
		: &parser->Tokens->Buffer[parser->Tokens->Length - 1];
}

/*
	Parsing

	FUNCTIONS:

	> ParseString()
	Create a JsonString from Tokens in a Parser's TokenArray. Strings must be wrapped in quotes. Supports empty strings. 

	> ParseValue()
	Create a JsonValue from Tokens in a Parser's TokenArray. 

	> ParsePair()
	Create a JsonPair from Tokens in a Parser's TokenArray

	> ParseList()
	Create a JsonList from Tokens in a Parser's TokenArray

	> ParseExpr()
	Create a JsonExpr from Tokens in a Parser's TokenArray
*/

static JsonString ParseString(Parser* parser) {
	Advance(parser, TOKEN_QUOTE);

	if (parser->Token->Type == TOKEN_QUOTE) {
		Advance(parser, TOKEN_QUOTE);
		return AllocJsonString("");
	}

	ASSERT(
		parser,
		parser->Token->Type == TOKEN_STRING,
		ERR_INVALID_SYNTAX
	);

	JsonString string = AllocJsonString(parser->Token->Value);
	Advance(parser, TOKEN_STRING);
	Advance(parser, TOKEN_QUOTE);

	return string;
}

static JsonValue* ParseValue(Parser* parser) {
	if (parser->Token->Type == TOKEN_LCURLY) {
		JsonExpr* expr = ParseExpr(parser);
		return JsonValueInit(expr, JSON_EXPR);
	}
	else if (parser->Token->Type == TOKEN_LBRACKET) {
		JsonList* list = ParseList(parser);
		return JsonValueInit(list, JSON_LIST);
	}
	else if (parser->Token->Type == TOKEN_QUOTE) {
		JsonString string = ParseString(parser);
		return JsonValueInit(string, JSON_STRING);
	}
	else if (parser->Token->Type == TOKEN_INT) {
		JsonInt* integer;

		ASSERT(
			parser,
			StringToInt(parser->Token->Value, &integer),
			ERR_INVALID_INT, parser->Token->Value
		);

		Advance(parser, TOKEN_INT);
		return JsonValueInit(integer, JSON_INT);
	}
	else if (parser->Token->Type == TOKEN_FLOAT) {
		JsonFloat* flt;

		ASSERT(
			parser,
			StringToFloat(parser->Token->Value, &flt),
			ERR_INVALID_FLOAT, parser->Token->Value
		);

		Advance(parser, TOKEN_FLOAT);
		return JsonValueInit(flt, JSON_FLOAT);
	}
	else if (parser->Token->Type == TOKEN_TRUE) {
		Advance(parser, TOKEN_TRUE);
		return JsonValueInit(NULL, JSON_TRUE);
	}
	else if (parser->Token->Type == TOKEN_FALSE) {
		Advance(parser, TOKEN_FALSE);
		return JsonValueInit(NULL, JSON_FALSE);
	}
	else if (parser->Token->Type == TOKEN_NULL) {
		Advance(parser, TOKEN_NULL);
		return JsonValueInit(NULL, JSON_NULL);
	}
	else if (parser->Token->Type == TOKEN_MINUS) {
		Advance(parser, TOKEN_MINUS);
		JsonValue* value = ParseValue(parser);

		switch (value->Type) {
			case JSON_INT:
				*value->Data->Int *= -1;
				return value;
			case JSON_FLOAT:
				*value->Data->Float *= -1;
				return value;
		}

		RAISE_FATAL_ERROR(parser, ERR_INVALID_SYNTAX);
	}
}

static JsonPair* ParsePair(Parser* parser) {
	JsonString key = ParseString(parser);
	Advance(parser, TOKEN_COLON);
	JsonValue* value = ParseValue(parser);

	return JsonPairInit(key, value);
}

static JsonList* ParseList(Parser* parser) {
	JsonList* list = JsonListInit();
	Advance(parser, TOKEN_LBRACKET);

	if (parser->Token->Type == TOKEN_RBRACKET) {
		Advance(parser, TOKEN_RBRACKET);
		return list;
	}

	JsonValue* value = ParseValue(parser);
	JsonValueArrayAppend(list, *value);
	free(value);

	while (parser->Token->Type == TOKEN_COMMA) {
		Advance(parser, TOKEN_COMMA);
		JsonValue* value = ParseValue(parser);
		JsonValueArrayAppend(list, *value);
		free(value);
	}

	Advance(parser, TOKEN_RBRACKET);
	return list;
}

static JsonExpr* ParseExpr(Parser* parser) {
	JsonExpr* expr = JsonExprInit();
	Advance(parser, TOKEN_LCURLY);

	if (parser->Token->Type == TOKEN_RCURLY) {
		Advance(parser, TOKEN_RCURLY);
		return expr;
	}

	JsonPair* pair = ParsePair(parser);
	JsonPairArrayAppend(expr, *pair);
	free(pair);

	while (parser->Token->Type == TOKEN_COMMA) {
		Advance(parser, TOKEN_COMMA);
		JsonPair* pair = ParsePair(parser);
		JsonPairArrayAppend(expr, *pair);
		free(pair);
	}

	Advance(parser, TOKEN_RCURLY);
	return expr;
}

/*
	Initializing Data

	FUNCTIONS:

	> ParserInit()
	Initialize a Parser object
*/

Parser* ParserInit(TokenArray* tokens) {
	Parser* parser = calloc(1, sizeof(Parser));
	parser->Tokens = tokens;
	parser->Token = &tokens->Buffer[0];
	parser->Index = 0;
	parser->Error = ErrorInit();

	return parser;
}

/*
	Get Results

	FUNCTIONS:

	> ParserGetResult()
	Use a Parser object to generate a JsonExpr object
*/

JsonExpr* ParserGetResult(Parser* parser) {
	JsonExpr* expr = ParseExpr(parser);

	ASSERT(
		parser,
		parser->Token->Type == TOKEN_EOF,
		ERR_UNEXPECTED_EOF
	);

	return expr;
}
