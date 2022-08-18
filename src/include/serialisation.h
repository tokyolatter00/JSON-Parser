
/*
	> serialisation.h
	Header file for defining functions which serialise JSON datatypes
	Documentation about the below functions can be found in serialisation.c
*/

#pragma once

#include "json-types.h"

/*
	Serialising JSON Datatypes
*/

const char* SerialiseJsonValue(JsonValue* value);
const char* SerialiseJsonPair(JsonPair* pair);
const char* SerialiseJsonList(JsonList* list);
const char* SerialiseJsonExpr(JsonExpr* expr);
