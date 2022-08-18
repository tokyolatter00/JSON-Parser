
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/serialisation.h"

/*
	Serialising JSON Datatypes

	FUNCTIONS:

	> SerialiseJsonString()
	Converts a JsonString to a serialised string.

	> SerialiseJsonInt()
	Converts a JsonInt to a serialised int.

	> SerialiseJsonFloat()
	Converts a JsonFloat to a serialised float.

	> SerialiseKeyword()
	Converts a keyword (JSON_TRUE, JSON_FALSE or JSON_NULL) to a serialised string

	> SerialiseJsonValue()
	Converts a JsonValue to a serialised value by serialising the JsonData inside of it
	
	> SerialiseJsonPair()
	Converts a JsonPair to a serialised pair.

	> SerialiseJsonList()
	Converts a JsonList to a serialised list.

	> SerialiseJsonExpr()
	Converts a JsonExpr to a serialised expr.
*/

const char* SerialiseJsonString(JsonString string) {
	StringBuilder* builder = StringBuilderInit();

	StringBuilderAppendChar(builder, '\"');
	StringBuilderAppendString(builder, string);
	StringBuilderAppendChar(builder, '\"');

	char* buffer = builder->Buffer;
	free(builder);
	return buffer;
}

const char* SerialiseJsonInt(JsonInt* integer) {
	StringBuilder* builder = StringBuilderInit();
	StringBuilderAppendLLONG(builder, *integer);

	char* buffer = builder->Buffer;
	free(builder);
	return buffer;
}

const char* SerialiseJsonFloat(JsonFloat* flt) {
	StringBuilder* builder = StringBuilderInit();
	StringBuilderAppendLDOUBLE(builder, *flt);

	char* buffer = builder->Buffer;
	free(builder);
	return buffer;
}

const char* SerialiseKeyword(JsonType keyword) {
	StringBuilder* builder = StringBuilderInit();
	
	switch (keyword) {
		case JSON_TRUE:
			StringBuilderAppendString(builder, KEYWORD_TRUE);
			break;
		case JSON_FALSE:
			StringBuilderAppendString(builder, KEYWORD_FALSE);
			break;
		case JSON_NULL:
			StringBuilderAppendString(builder, KEYWORD_NULL);
			break;
	}

	char* buffer = builder->Buffer;
	free(builder);
	return buffer;
}

const char* SerialiseJsonValue(JsonValue* value) {
	switch (value->Type) {
		case JSON_EXPR:
			return SerialiseJsonExpr(value->Data->Expr);
		case JSON_LIST:
			return SerialiseJsonList(value->Data->List);
		case JSON_STRING:
			return SerialiseJsonString(value->Data->String);
		case JSON_INT:
			return SerialiseJsonInt(value->Data->Int);
		case JSON_FLOAT:
			return SerialiseJsonFloat(value->Data->Float);
		case JSON_TRUE:
		case JSON_FALSE:
		case JSON_NULL:
			return SerialiseKeyword(value->Type);
	}
}

const char* SerialiseJsonPair(JsonPair* pair) {
	StringBuilder* builder = StringBuilderInit();
	const char* valuestr = SerialiseJsonValue(pair->Value);

	StringBuilderAppendChar(builder, '\"');
	StringBuilderAppendString(builder, pair->Key);
	StringBuilderAppendString(builder, "\": ");
	StringBuilderAppendString(builder, valuestr);

	char* buffer = builder->Buffer;
	free(builder);
	free(valuestr);
	return buffer;
}

const char* SerialiseJsonList(JsonList* list) {
	StringBuilder* builder = StringBuilderInit();
	StringBuilderAppendChar(builder, '[');

	for (ullong i = 0; i < list->Length; i++) {
		const char* valuestr = SerialiseJsonValue(&list->Buffer[i]);
		StringBuilderAppendString(builder, valuestr);

		if (i != list->Length - 1) {
			StringBuilderAppendString(builder, ", ");
		}

		free(valuestr);
	}

	StringBuilderAppendChar(builder, ']');

	char* buffer = builder->Buffer;
	free(builder);
	return buffer;
}

const char* SerialiseJsonExpr(JsonExpr* expr) {
	StringBuilder* builder = StringBuilderInit();
	StringBuilderAppendChar(builder, '{');

	for (ullong i = 0; i < expr->Length; i++) {
		const char* pairstr = SerialiseJsonPair(&expr->Buffer[i]);
		StringBuilderAppendString(builder, pairstr);

		if (i != expr->Length - 1) {
			StringBuilderAppendString(builder, ", ");
		}

		free(pairstr);
	}

	StringBuilderAppendChar(builder, '}');

	char* buffer = builder->Buffer;
	free(builder);
	return buffer;
}
