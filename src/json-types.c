
#include <stdlib.h>
#include <string.h>
#include "include/json-types.h"

/*
	Memory Allocation

	FUNCTIONS:

	> AllocJsonString()
	Creates a dynamically allocated JsonString from another string

	> AllocJsonInt()
	Creates a dynamically allocated JsonInt from an integer

	> AllocJsonFloat()
	Creates a dynamically allocated JsonFloat from a float
*/

JsonString* AllocJsonString(JsonString string) {
	if (!string) {
		return string;
	}

	size_t len = strlen(string);
	char* output = malloc(len + 1);

	for (size_t i = 0; i < len; i++) {
		output[i] = string[i];
	}

	output[len] = '\0';
	return output;
}

JsonInt* AllocJsonInt(JsonInt integer) {
	JsonInt* output = malloc(sizeof(JsonInt));
	*output = integer;

	return output;
}

JsonFloat* AllocJsonFloat(JsonFloat flt) {
	JsonFloat* output = malloc(sizeof(JsonFloat));
	*output = flt;

	return output;
}

/*
	Initializing Data

	FUNCTIONS:

	> JsonValueInit()
	Initialize a JsonValue object

	> JsonPairInit()
	Initialize a JsonPair object
*/

JsonValue* JsonValueInit(void* data, JsonType type) {
	JsonValue* value = calloc(1, sizeof(JsonValue));
	value->Data = calloc(1, sizeof(JsonData));
	value->Type = type;

	switch (type) {
		case JSON_EXPR:
			value->Data->Expr = data;
			break;
		case JSON_LIST:
			value->Data->List = data;
			break;
		case JSON_STRING:
			value->Data->String = data;
			break;
		case JSON_INT:
			value->Data->Int = data;
			break;
		case JSON_FLOAT:
			value->Data->Float = data;
			break;
	}

	return value;
}

JsonPair* JsonPairInit(const char* key, JsonValue* value) {
	JsonPair* pair = calloc(1, sizeof(JsonPair));
	pair->Key = key;
	pair->Value = value;

	return pair;
}

/*
	Copy Data

	FUNCTIONS:

	> JsonValueCopy()
	Create a copy of a JsonValue from another JsonValue

	> JsonPairCopy()
	Create a copy of a JsonPair from another JsonPair

	> JsonListCopy()
	Create a copy of a JsonList from another JsonList

	> JsonExprCopy()
	Create a copy of a JsonExpr from another JsonExpr

	NOTES:

	These functions are used to create clones of other objects. These help ensure that data is
	not shared between objects. 
*/

JsonValue* JsonValueCopy(JsonValue* value) {
	JsonValue* copy = calloc(1, sizeof(JsonValue));
	copy->Data = calloc(1, sizeof(JsonValue));
	copy->Type = value->Type;

	switch (value->Type) {
		case JSON_EXPR:
			copy->Data->Expr = JsonExprCopy(value->Data->Expr);
			break;
		case JSON_LIST:
			copy->Data->List = JsonListCopy(value->Data->List);
			break;
		case JSON_STRING:
			copy->Data->String = AllocJsonString(value->Data->String);
			break;
		case JSON_INT:
			copy->Data->Int = AllocJsonInt(*value->Data->Int);
			break;
		case JSON_FLOAT:
			copy->Data->Float = AllocJsonFloat(*value->Data->Float);
			break;
	}

	return copy;
}

JsonPair* JsonPairCopy(JsonPair* pair) {
	JsonPair* copy = calloc(1, sizeof(JsonPair));
	copy->Key = AllocJsonString(pair->Key);
	copy->Value = JsonValueCopy(pair->Value);

	return copy;
}

JsonList* JsonListCopy(JsonList* list) {
	JsonList* copy = calloc(1, sizeof(JsonList));
	copy->Buffer = calloc(list->Length, sizeof(JsonValue));
	copy->Capacity = list->Capacity;
	copy->Length = list->Length;

	for (ullong i = 0; i < list->Length; i++) {
		JsonValue* value = JsonValueCopy(&list->Buffer[i]);
		copy->Buffer[i] = *value;
		free(value);
	}

	return copy;
}

JsonExpr* JsonExprCopy(JsonExpr* expr) {
	JsonExpr* copy = calloc(1, sizeof(JsonExpr));
	copy->Buffer = calloc(expr->Length, sizeof(JsonPair));
	copy->Capacity = expr->Capacity;
	copy->Length = expr->Length;

	for (ullong i = 0; i < expr->Length; i++) {
		JsonPair* pair = JsonPairCopy(&expr->Buffer[i]);
		copy->Buffer[i] = *pair;
		free(pair);
	}

	return copy;
}

/*
	Deleting Data

	FUNCTIONS:

	> JsonDataDelete()
	Deletes a JsonData object inside of a JsonValue object
	JsonValue is passed in because the type is required for freeing

	> JsonValueDelete()
	Deletes a JsonValue object entirely

	> JsonPairDelete()
	Deletes a JsonPair object entirely

	> JsonListDelete()
	Deletes a JsonList object entirely
	Deletes all of the elements inside of the JsonList

	> JsonExprDelete()
	Deletes a JsonExpr object entirely
	Deletes all of the pairs inside of the JsonExpr
*/

void JsonDataDelete(JsonValue* value) {
	switch (value->Type) {
		case JSON_EXPR:
			JsonExprDelete(value->Data->Expr);
			break;
		case JSON_LIST:
			JsonListDelete(value->Data->List);
			break;
		case JSON_STRING:
			free(value->Data->String);
			break;
		case JSON_INT:
			free(value->Data->Int);
			break;
		case JSON_FLOAT:
			free(value->Data->Float);
			break;
	}

	free(value->Data);
}

void JsonValueDelete(JsonValue* value) {
	JsonDataDelete(value);
	free(value);
}

void JsonPairDelete(JsonPair* pair) {
	free(pair->Key);
	JsonValueDelete(pair->Value);
}

void JsonListDelete(JsonList* list) {
	for (ullong i = 0; i < list->Length; i++) {
		JsonDataDelete(&list->Buffer[i]);
	}

	free(list->Buffer);
	free(list);
}

void JsonExprDelete(JsonExpr* expr) {
	for (ullong i = 0; i < expr->Length; i++) {
		JsonPairDelete(&expr->Buffer[i]);
	}

	free(expr->Buffer);
	free(expr);
}
