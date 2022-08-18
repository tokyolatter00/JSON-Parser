
#pragma once

#include "token.h"
#include "json-types.h"
#include "types.h"

/*
	String Builder

	FUNCTIONS:

	> 
*/

typedef struct {
	char* Buffer;
	ullong Length;
	ullong Capacity;
} StringBuilder;

StringBuilder* StringBuilderInit();
void StringBuilderAppendChar(StringBuilder* builder, char chr);
void StringBuilderAppendString(StringBuilder* builder, const char* str);
void StringBuilderAppendLLONG(StringBuilder* builder, llong integer);
void StringBuilderAppendLDOUBLE(StringBuilder* builder, ldouble flt);

/*
	Token Array
*/

#define TokenArrayDelete(arr)					\
	free(arr->Buffer);							\
	free(arr);

typedef struct {
	Token* Buffer;
	ullong Length;
	ullong Capacity;
} TokenArray;

TokenArray* TokenArrayInit();
void TokenArrayAppend(TokenArray* arr, Token token);

/*
	JsonValue Array
*/

typedef struct JsonValueArray_t {
	struct JsonValue_t* Buffer;
	ullong Length;
	ullong Capacity;
} JsonValueArray;

JsonValueArray* JsonValueArrayInit();
void JsonValueArrayAppend(JsonValueArray* arr, struct JsonValue_t value);
void JsonValueArrayRemove(JsonValueArray* arr, ullong index);

/*
	JsonPair Array
*/

typedef struct JsonPair_t;

typedef struct JsonPairArray_t {
	struct JsonPair_t* Buffer;
	ullong Length;
	ullong Capacity;
} JsonPairArray;

JsonPairArray* JsonPairArrayInit();
void JsonPairArrayAppend(JsonPairArray* arr, struct JsonPair_t pair);
void JsonPairArrayRemove(JsonPairArray* arr, ullong index);
