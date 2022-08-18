
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/containers.h"

/*
	String Builder

	FUNCTIONS:

	> StringBuilerAllocMore()
	Allocates more data for a StringBuilder's buffer. Data sizes double each time (2, 4, 8, 16, 32, 64, etc.).
	An extra byte is allocated ontop of the capacity. For the null terminator. The previous buffer is then freed to
	prevent memory leaks and a new buffer with the copied data is set. 

	> StringBuilderInit()
	Initialize a StringBuilder object

	> StringBuilderAppendChar()
	Appends a char to a StringBuilder's buffer. If the length of the StringBuilder begins to exceed its capacity
	the StringBuilder will allocate more memory to adjust. 

	> StringBuilderAppendString()
	Appends a string to a StringBuilder's buffer. To keep it simple the StringBuilder just pushes each char in the
	string to StringBuilderAppendChar()

	> StringBuilderAppendLLONG()
	Appends a long long to a StringBuilder's buffer. The long long is first converted to a string using sprintf_s()
	and is then passed to StringBuilderAppendString()

	> StringBuilderAppendLDOUBLE()
	Appends a long double to a StringBuilder's buffer. The long double is first converted to a string using sprintf_s()
	and is then passed to StringBuilderAppendString()
*/

void StringBuilderAllocMore(StringBuilder* builder) {
	size_t capacity = builder->Capacity > 0 ? builder->Capacity * 2 : 2;
	char* buffer = malloc(capacity + 1);

	for (size_t i = 0; i < builder->Capacity; i++) {
		buffer[i] = builder->Buffer[i];
	}

	free(builder->Buffer);
	builder->Buffer = buffer;
	builder->Capacity = capacity;
}

StringBuilder* StringBuilderInit() {
	StringBuilder* builder = calloc(1, sizeof(StringBuilder));
	builder->Buffer = NULL;
	builder->Length = 0;
	builder->Capacity = 0;

	return builder;
}

void StringBuilderAppendChar(StringBuilder* builder, char chr) {
	if (builder->Length >= builder->Capacity) {
		StringBuilderAllocMore(builder);
	}

	builder->Buffer[builder->Length++] = chr;
	builder->Buffer[builder->Length] = '\0';
}

void StringBuilderAppendString(StringBuilder* builder, const char* str) {
	size_t len = strlen(str);

	for (size_t i = 0; i < len; i++) {
		StringBuilderAppendChar(builder, str[i]);
	}
}

void StringBuilderAppendLLONG(StringBuilder* builder, llong integer) {
	static char buffer[32];
	sprintf_s(buffer, 32, "%lld", integer);
	StringBuilderAppendString(builder, buffer);
}

void StringBuilderAppendLDOUBLE(StringBuilder* builder, ldouble flt) {
	static char buffer[32];
	sprintf_s(buffer, 32, "%Lf", flt);
	StringBuilderAppendString(builder, buffer);
}

/*
	Token Array

	FUNCTIONS:

	> TokenArrayAllocMore()
	Allocates more data for a TokenArray's buffer. Data sizes double each time (2, 4, 8, 16, 32, 64, etc.).
	The previous buffer is then freed to prevent memory leaks and a new buffer with the copied data is set.

	> TokenArrayInit()
	Initialize a TokenArray object

	> TokenArrayAppend()
	Appends a Token to a TokenArray's buffer. If the length of the TokenAray begins to exceed its capacity the TokenArray
	will allocate more memory to adjust. 
*/

void TokenArrayAllocMore(TokenArray* arr) {
	ullong capacity = arr->Capacity > 0 ? arr->Capacity * 2 : 2;
	Token* buffer = malloc(sizeof(Token) * capacity);

	for (ullong i = 0; i < arr->Capacity; i++) {
		buffer[i] = arr->Buffer[i];
	}

	free(arr->Buffer);
	arr->Buffer = buffer;
	arr->Capacity = capacity;
}

TokenArray* TokenArrayInit() {
	TokenArray* arr = calloc(1, sizeof(TokenArray));
	arr->Buffer = NULL;
	arr->Length = 0;
	arr->Capacity = 0;

	return arr;
}

void TokenArrayAppend(TokenArray* arr, Token token) {
	if (arr->Length >= arr->Capacity) {
		TokenArrayAllocMore(arr);
	}

	arr->Buffer[arr->Length++] = token;
}

/*
	JsonValue Array

	FUNCTIONS:

	> JsonValueArrayAllocMore()
	Allocates more data for JsonValueArray's buffer. Data sizes double each time (2, 4, 8, 16, 32, 64, etc.).
	The previous buffer is then freed to prevent memory leaks and a new buffer with the copied data is set.

	> JsonValueArrayInit()
	Initialize a JsonValueArray object

	> JsonValueArrayAppend()
	Appends a JsonValue to a JsonValueArray's buffer. If the length of the JsonValueArray begins to exceed its capacity
	the JsonValueArray will allocate more memory to adjust
*/

void JsonValueArrayAllocMore(JsonValueArray* arr) {
	ullong capacity = arr->Capacity > 0 ? arr->Capacity * 2 : 2;
	JsonValue* buffer = malloc(sizeof(JsonValue) * capacity);

	for (ullong i = 0; i < arr->Capacity; i++) {
		buffer[i] = arr->Buffer[i];
	}

	free(arr->Buffer);
	arr->Buffer = buffer;
	arr->Capacity = capacity;
}

JsonValueArray* JsonValueArrayInit() {
	JsonValueArray* arr = calloc(1, sizeof(JsonValueArray));
	arr->Buffer = NULL;
	arr->Length = 0;
	arr->Capacity = 0;

	return arr;
}

void JsonValueArrayAppend(JsonValueArray* arr, JsonValue value) {
	if (arr->Length >= arr->Capacity) {
		JsonValueArrayAllocMore(arr);
	}

	arr->Buffer[arr->Length++] = value;
}

void JsonValueArrayRemove(JsonValueArray* arr, ullong index) {
	JsonValue* buffer = malloc(sizeof(JsonValue) * arr->Capacity);

	for (ullong i = 0, x = 0; i < arr->Length; i++) {
		if (i != index) {
			buffer[x++] = arr->Buffer[i];
		}
	}

	free(arr->Buffer);
	arr->Buffer = buffer;
	arr->Length--;
}

/*
	JsonPair Array

	FUNCTIONS:

	> JsonPairArrayAllocMore()
	Allocates more data for a JsonPairArray's buffer. Data sizes double each time (2, 4, 8, 16, 32, 64, etc.).
	The previous buffer is then freed to prefvent memory leaks and a new buffer with the copied data i sset.

	> JsonPairArrayInit()
	Initialize a JsonPairArray object

	> JsonPairArrayAllocMore()
	Appends a JsonPair to a JsonPairArray's buffer. If the length of the JsonPairArray begins to exceed its capacity the
	JsonPairArray will allocate more memory to adjust. 
*/

void JsonPairArrayAllocMore(JsonPairArray* arr) {
	ullong capacity = arr->Capacity > 0 ? arr->Capacity * 2 : 2;
	JsonPair* buffer = malloc(sizeof(JsonPair) * capacity);

	for (ullong i = 0; i < arr->Capacity; i++) {
		buffer[i] = arr->Buffer[i];
	}

	free(arr->Buffer);
	arr->Buffer = buffer;
	arr->Capacity = capacity;
}

JsonPairArray* JsonPairArrayInit() {
	JsonPairArray* arr = calloc(1, sizeof(JsonPairArray));
	arr->Buffer = NULL;
	arr->Length = 0;
	arr->Capacity = 0;

	return arr;
}

void JsonPairArrayAppend(JsonPairArray* arr, JsonPair pair) {
	if (arr->Length >= arr->Capacity) {
		JsonPairArrayAllocMore(arr);
	}

	arr->Buffer[arr->Length++] = pair;
}

void JsonPairArrayRemove(JsonPairArray* arr, ullong index) {
	JsonPair* buffer = malloc(sizeof(JsonPair) * arr->Capacity);

	for (ullong i = 0, x = 0; i < arr->Length; i++) {
		if (i != index) {
			buffer[x++] = arr->Buffer[i];
		}
	}

	free(arr->Buffer);
	arr->Buffer = buffer;
	arr->Length--;
}
