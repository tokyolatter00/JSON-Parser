
/*
	> json-types.h
	Header file for defining JSON datatypes and functions which interact with them
	Documentation about the below functions can be found in json-types.c
*/

#pragma once

#include "containers.h"

#define JsonListInit JsonValueArrayInit
#define JsonExprInit JsonPairArrayInit
#define JsonTrue JsonCreateKeyword(JSON_TRUE)
#define JsonFalse JsonCreateKeyword(JSON_FALSE)
#define JsonNull JsonCreateKeyword(JSON_NULL)

typedef const char* JsonString;
typedef long long JsonInt;
typedef long double JsonFloat;
typedef struct JsonValueArray_t JsonList;
typedef struct JsonPairArray_t JsonExpr;

typedef enum {
	JSON_EXPR,
	JSON_LIST,
	JSON_STRING,
	JSON_INT,
	JSON_FLOAT,
	JSON_TRUE,
	JSON_FALSE,
	JSON_NULL
} JsonType;

typedef union {
	JsonExpr* Expr;
	JsonList* List;
	JsonString String;
	JsonInt* Int;
	JsonFloat* Float;
} JsonData;

typedef struct JsonValue_t {
	JsonType Type;
	JsonData* Data;
} JsonValue;

typedef struct JsonPair_t {
	const char* Key;
	JsonValue* Value;
} JsonPair;

/*
	Memory Allocation
*/

JsonString* AllocJsonString(JsonString string);
JsonInt* AllocJsonInt(JsonInt integer);
JsonFloat* AllocJsonFloat(JsonFloat flt);

/*
	Initializing Data
*/

JsonValue* JsonValueInit(void* data, JsonType type);
JsonPair* JsonPairInit(const char* key, JsonValue* value);

/*
	Copying Data
*/

JsonValue* JsonValueCopy(JsonValue* value);
JsonPair* JsonPairCopy(JsonPair* pair);
JsonList* JsonListCopy(JsonList* list);
JsonExpr* JsonExprCopy(JsonExpr* expr);

/*
	Deleting Data
*/

void JsonDataDelete(JsonValue* value);
void JsonValueDelete(JsonValue* value);
void JsonPairDelete(JsonPair* pair);
void JsonListDelete(JsonList* list);
void JsonExprDelete(JsonExpr* expr);
