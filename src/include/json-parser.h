
/*
	> json-parser.h
	Header file for defining functions of the main Json Parser module
	Documentation about the below functions can be found in json-parser.c
*/

#pragma once

#include <stdlib.h>
#include "json-handler.h"
#include "json-types.h"
#include "error.h"

#define JsonCreateHandler JsonHandlerInit
#define JsonDeleteHandler JsonHandlerDelete
#define JsonDeleteExpr JsonExprDelete
#define JsonDeleteValue JsonValueDelete

/*
	Loading Json Data
*/

JsonExpr* JsonLoadString(JsonHandler* handler, const char* source);
JsonExpr* JsonLoadFile(JsonHandler* handler, const char* path);

/*
	Dumping Json Data
*/

void JsonDumpString(JsonExpr* expr, const char** dest);
void JsonDumpFile(JsonHandler* handler, JsonExpr* expr, const char* path);

/*
	Creating Values
*/

#define JsonCreateValue JsonValueInit
#define JsonCreateExpr JsonExprInit
#define JsonCreateList JsonListInit

JsonValue* JsonCreateString(JsonString string);
JsonValue* JsonCreateInt(JsonInt integer);
JsonValue* JsonCreateFloat(JsonFloat flt);
JsonValue* JsonCreateKeyword(JsonType keyword);

/*
	Accessing Data
*/

ullong JsonGetPairIndex(JsonExpr* expr, const char* key);
ullong JsonKeyExists(JsonExpr* expr, const char* key);
int JsonGetValue(JsonExpr* expr, const char* key, JsonValue** value);
int JsonGetList(JsonExpr* expr, const char* key, JsonList** list);
int JsonGetExpr(JsonExpr* expr, const char* key, JsonExpr** expr2);

/*
	Comparing Data
*/

int JsonCompareValues(JsonValue* value1, JsonValue* value2);
int JsonComparePairs(JsonPair* pair1, JsonPair* pair2);
int JsonCompareLists(JsonList* list1, JsonList* list2);
int JsonCompareExprs(JsonExpr* expr1, JsonExpr* expr2);

/*
	Modifying List
*/

void JsonAppend(JsonList* list, JsonValue* value);
void JsonAppendString(JsonList* list, JsonString string);
void JsonAppendInt(JsonList* list, JsonInt integer);
void JsonAppendFloat(JsonList* list, JsonFloat flt);
void JsonAppendTrue(JsonList* list);
void JsonAppendTrue(JsonList* list);
void JsonAppendNull(JsonList* list);
void JsonAppendList(JsonList* list, JsonList* list2);
void JsonAppendExpr(JsonList* list, JsonExpr* expr);
void JsonAddRange(JsonList* list, JsonList* list2, int reuse_list);

int JsonRemoveElement(JsonList* list, ullong index);

/*
	Modifying Expr
*/

void JsonSet(JsonExpr* expr, const char* key, JsonValue* value);
void JsonSetString(JsonExpr* expr, const char* key, JsonString* string);
void JsonSetInt(JsonExpr* expr, const char* key, JsonInt integer);
void JsonSetFloat(JsonExpr* expr, const char* key, JsonFloat flt);
void JsonSetTrue(JsonExpr* expr, const char* key);
void JsonSetFalse(JsonExpr* expr, const char* key);
void JsonSetNull(JsonExpr* expr, const char* key);
void JsonSetList(JsonExpr* expr, const char* key, JsonList* list);
void JsonSetExpr(JsonExpr* expr, const char* key, JsonExpr* expr2);
void JsonMerge(JsonExpr* expr, JsonExpr* expr2, int reuse_expr);

int JsonRemoveKey(JsonExpr* expr, const char* key);
