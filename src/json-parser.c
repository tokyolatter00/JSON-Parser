
#include <stdlib.h>
#include <string.h>
#include "include/json-parser.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/serialisation.h"
#include "include/file-io.h"

#define CompareStrings(str1, str2) strcmp(str1, str2) == 0

/*
	Loading Json Data

	FUNCTIONS:

	> JsonLoadString()
	Creates a JsonExpr object from a raw JSON string using a parsing algorithm
	Creates an error in the handler if there is an error in the string

	> JsonLoadFile()
	Creates a JsonExpr object from a json file using a parsing algorithm
	Creates an error in the handler if the contents of the file are invalid

	> JsonDumpString()
	Dumps a JsonExpr object to a string
	String that is returns should be freed by the function caller

	> JsonDumpFile()
	Dumps a JsonExpr object to a json file
	Creates an error in the JsonHandler if it failed to write to the file
	
	WARNING:

	Attempting to work with a JsonExpr object return from JsonLoadString() or JsonLoadFile()
	may result in a runtime error as the behaviour is undefined. Errors should be checked for
	in the handler after calling this function. If there is no error then it is safe to proceed
*/

JsonExpr* JsonLoadString(JsonHandler* handler, const char* source) {
	// Lexer

	Lexer* lexer = LexerInit(source);
	TokenArray* tokens = LexerGetResult(lexer);

	if (lexer->Error->Exists) {
		handler->Error = lexer->Error;
		
		// Free Lexer Memory

		TokenArrayDelete(tokens);
		free(lexer);

		return;
	}

	// Parser

	Parser* parser = ParserInit(tokens);
	JsonExpr* expr = ParserGetResult(parser);

	if (parser->Error->Exists) {
		handler->Error = parser->Error;
		
		// Free Lexer & Parser Memory

		TokenArrayDelete(tokens);
		free(lexer);
		free(parser);

		return;
	}

	// Free All Memory

	for (ullong i = 0; i < tokens->Length; i++) {
		Token* token = &tokens->Buffer[i];

		switch (token->Type) {
			case TOKEN_STRING:
			case TOKEN_INT:
			case TOKEN_FLOAT:
				free(token->Value);
				break;
		}
	}

	ErrorDelete(lexer->Error);
	ErrorDelete(parser->Error);
	TokenArrayDelete(tokens);
	free(lexer);
	free(parser);

	return expr;
}

JsonExpr* JsonLoadFile(JsonHandler* handler, const char* path) {
	char* source;
	Error* error = FileReadAllText(path, &source);

	if (error->Exists) {
		handler->Error = error;
		return;
	}

	JsonExpr* expr = JsonLoadString(handler, source);
	ErrorDelete(error);
	free(source);
	return expr;
}

void JsonDumpString(JsonExpr* expr, const char** dest) {
	*dest = SerialiseJsonExpr(expr);
}

void JsonDumpFile(JsonHandler* handler, JsonExpr* expr, const char* path) {
	char* text;
	JsonDumpString(expr, &text);
	Error* error = FileWriteAllText(path, text);
	free(text);

	if (error->Exists) {
		handler->Error = error;
		return;
	}

	ErrorDelete(error);
}

/*
	Creating Values

	FUNCTIONS:

	> JsonCreateString()
	Creates a JsonValue object which stores a JsonString inside of it

	> JsonCreateInt()
	Creates a JsonValue object which stores a JsonInt inside of it

	> JsonCreateFloat()
	Creates a JsonValue object which stores a JsonFloat inside of it

	> JsonCreateKeyword()
	Creates a JsonValue which stores a keyword inside of it (true, false or null)
*/

JsonValue* JsonCreateString(JsonString string) {
	JsonValue* value = calloc(1, sizeof(JsonValue));
	value->Data = calloc(1, sizeof(JsonData));
	value->Data->String = AllocJsonString(string);
	value->Type = JSON_STRING;

	return value;
}

JsonValue* JsonCreateInt(JsonInt integer) {
	JsonValue* value = calloc(1, sizeof(JsonValue));
	value->Data = calloc(1, sizeof(JsonData));
	value->Data->Int = AllocJsonInt(integer);
	value->Type = JSON_INT;

	return value;
}

JsonValue* JsonCreateFloat(JsonFloat flt) {
	JsonValue* value = calloc(1, sizeof(JsonValue));
	value->Data = calloc(1, sizeof(JsonData));
	value->Data->Float = AllocJsonFloat(flt);
	value->Type = JSON_FLOAT;

	return value;
}

JsonValue* JsonCreateKeyword(JsonType keyword) {
	JsonValue* value = calloc(1, sizeof(JsonValue));
	value->Type = keyword;

	return value;
}

/*
	Accessing Data

	MACROS:

	> COMPARE_KEYS()
	Compares two keys to see if they match (technically its just a shorthand for string comparison)

	> PAIR_INDEX_NOT_FOUND
	The index of a key in a JsonExpr was not found (the JsonExpr object does not contain this key)

	> SUCCESS
	An operation was successfuly

	> FAILURE
	An operation failed

	FUNCTIONS:

	> JsonGetPairIndex()
	Returns the index of a key in a JsonExpr
	Returns PAIR_INDEX_NOT_FOUND if the key does not exist

	> JsonKeyExists()
	Returns 1 if a key exists in a JsonExpr
	Returns 0 if the key does not exist

	> JsonGetValue()

	Gets the value which belongs to a key in a JsonExpr object
	Returns SUCCESS if the value was found
	Returns FAILURE if the value was not found

	> JsonGetList()
	Gets the JsonList which belongs to a key in a JsonExpr
	Returns SUCCESS if a JsonList was found
	Returns FAILURE if the value was not found or if the value type is not JSON_LIST

	> JsonGetExpr()
	Gets the JsonExpr which belongs to a key in a JsonExpr
	Returns SUCCESS if a JsonExpr was found
	Returns FAILURE if the value was not found or if the value type is not JSON_EXPR

	WARNING:
	Attempting to work with a value assigned by JsonGetValue(), JsonGetList() or JsonGetExpr()
	without checking for an error may result in a runtime error as the behaviour is undefined.
	You should check that the return values from these functions is 1 and not 0 before using the value
*/

#define PAIR_INDEX_NOT_FOUND -1
#define SUCCESS 1
#define FAILURE 0

ullong JsonGetPairIndex(JsonExpr* expr, const char* key) {
	for (ullong i = 0; i < expr->Length; i++) {
		if (CompareStrings(expr->Buffer[i].Key, key)) {
			return i;
		}
	}

	return PAIR_INDEX_NOT_FOUND;
}

ullong JsonKeyExists(JsonExpr* expr, const char* key) {
	return JsonGetPairIndex(expr, key) != PAIR_INDEX_NOT_FOUND;
}

int JsonGetValue(JsonExpr* expr, const char* key, JsonValue** value) {
	ullong index = JsonGetPairIndex(expr, key);

	if (index == PAIR_INDEX_NOT_FOUND) {
		return FAILURE;
	}

	*value = expr->Buffer[index].Value;
	return SUCCESS;
}

int JsonGetList(JsonExpr* expr, const char* key, JsonList** list) {
	JsonValue* value;

	if (JsonGetValue(expr, key, &value)) {
		if (value->Type != JSON_LIST) {
			return FAILURE;
		}

		*list = value->Data->List;
		return SUCCESS;
	}
	
	return FAILURE;
}

int JsonGetExpr(JsonExpr* expr, const char* key, JsonExpr** expr2) {
	JsonValue* value;

	if (JsonGetValue(expr, key, &value)) {
		if (value->Type != JSON_EXPR) {
			return FAILURE;
		}

		*expr2 = value->Data->Expr;
		return SUCCESS;
	}

	return FAILURE;
}

/*
	Comparing Data

	MACROS:

	> CompareValueExprs()
	Compares two JsonValues which are both JsonExprs

	> CompareValueLists()
	Compares two JsonValues which are both JsonLists

	> CompareValueString()
	Compares two JsonValues which are both JsonStrings

	> CompareValueInt()
	Compares two JsonValues which are both JsonInts

	> CompareValueFloat()
	Compares two JsonValues which are both JsonFloats

	FUNCTIONS:

	> JsonCompareValues()
	Compares two JsonValues together
	If both JsonValues point to the same JsonValue struct in memory then they are automatically the same
	If they both share the same datatype it will compare the JsonData inside of each JsonValue

	> JsonComparePairs()
	Compares two JsonPairs together
	If both JsonPairs point to the same JsonPair struct in memory then they are autoamtically the same
	If the keys both match then it will call JsonCompareValues on both of the values

	> JsonCompareLists()
	Compares two JsonLists together
	If both JsonLists point to the same JsonList struct in memory then they are automatically the same
	If both JsonLists do not share the same length then they cannot be the same
	If every JsonValue in both of the JsonLists match at the same index then they are identical

	> JsonCompareExprs()
	Compares two JsonExprs together
	If both JsonExprs point to the same JsonExpr struct in memory then they are automatically the same
	If both JsonExprs do not share the same length then they cannot be the same
	
	NOTE:

	Unlike JsonCompareLists(), JsonCompareExprs() does not require the values to match at the same index.
	For example: {"name": "John", "age": 30} and {"age": 30, "name": "John"} would still be seen as equal.
	This does not apply in a JsonList as it is indexed by integers not keys
*/

#define CompareValueExprs(value1, value2)							\
	JsonCompareExprs(value1->Data->Expr, value2->Data->Expr)

#define CompareValueLists(value1, value2)							\
	JsonCompareLists(value1->Data->List, value2->Data->List)

#define CompareValueString(value1, value2)							\
	CompareStrings(value1->Data->String, value2->Data->String)

#define CompareValueInt(value1, value2)								\
	*value1->Data->Int == *value2->Data->Int

#define CompareValueFloat(value1, value2)							\
	*value1->Data->Float == *value2->Data->Float

#define TRUE 1
#define FALSE 0

int JsonCompareValues(JsonValue* value1, JsonValue* value2) {
	if (value1 == value2) {
		return TRUE;
	}

	switch (value1->Type) {
		case JSON_EXPR:
			return value2->Type == JSON_EXPR
				? CompareValueExprs(value1, value2)
				: FALSE;
		case JSON_LIST:
			return value2->Type == JSON_LIST
				? CompareValueLists(value1, value2)
				: FALSE;
		case JSON_STRING:
			return value2->Type == JSON_STRING
				? CompareValueString(value1, value2)
				: FALSE;
		case JSON_INT:
			return value2->Type == JSON_INT
				? CompareValueInt(value1, value2)
				: FALSE;
		case JSON_FLOAT:
			return value2->Type == JSON_FLOAT
				? CompareValueFloat(value1, value2)
				: FALSE;
		case JSON_TRUE:
		case JSON_FALSE:
		case JSON_NULL:
			return value1->Type == value2->Type;
	}
}

int JsonComparePairs(JsonPair* pair1, JsonPair* pair2) {
	if (pair1 == pair2) {
		return TRUE;
	}
	
	if (!CompareStrings(pair1->Key, pair2->Key)) {
		return FALSE;
	}

	return JsonCompareValues(pair1->Value, pair2->Value);
}

int JsonCompareLists(JsonList* list1, JsonList* list2) {
	if (list1 == list2) {
		return TRUE;
	}

	if (list1->Length != list2->Length) {
		return FALSE;
	}

	for (ullong i = 0; i < list1->Length; i++) {
		JsonValue* value1 = &list1->Buffer[i];
		JsonValue* value2 = &list2->Buffer[i];

		if (!JsonCompareValues(value1, value2)) {
			return FALSE;
		}
	}

	return TRUE;
}

int JsonCompareExprs(JsonExpr* expr1, JsonExpr* expr2) {
	if (expr1 == expr2) {
		return TRUE;
	}

	if (expr1->Length != expr2->Length) {
		return FALSE;
	}

	for (ullong i = 0; i < expr1->Length; i++) {
		JsonPair* pair1 = &expr1->Buffer[i];
		JsonValue* value1 = pair1->Value;
		JsonValue* value2;

		if (JsonGetValue(expr2, pair1->Key, &value2)) {
			if (!JsonCompareValues(value1, value2)) {
				return FALSE;
			}
		}
		else {
			return FALSE;
		}
	}

	return TRUE;
}

/*
	Modifying List

	FUNCTIONS:

	> JsonAppend()
	Adds a JsonValue to a JsonList
	Used for when you want to append a JsonValue to a JsonList

	> JsonAppendString()
	Creates a JsonValue from a JsonString and adds it to a JsonList
	Used for when you want to append a string to a JsonList

	> JsonAppendInt()
	Creates a JsonValue from a JsonInt and adds it to a JsonList
	Used for when you want to append an int to a JsonList

	> JsonAppendFloat()
	Creates a JsonValue from a JsonFloat and adds it to a JsonList
	Used for when you want to append a float to a JsonList

	> JsonAppendTrue
	Creates a JsonValue which equals JSON_TRUE (true keyword)
	Used for when you want to add true to a JsonList

	> JsonAppendFalse
	Creates a JsonValue which equals JSON_FALSE (false keyword)
	Used for when you want to add false to a JsonList

	> JsonAppendNull
	Creates a JsonValue which equals JSON_NULL (null keyword)
	Used for when you want to add null to a JsonList

	> JsonAppendList()
	Appends a JsonList to a JsonList
	If you are adding a JsonList to itself a copy will be made of the JsonExpr to avoid errors, this may result in
	a very small memory leak as the memory deallocator has lost a reference to the data. This can be fixed
	by manually freeing the list
	
	> JsonAppendExpr()
	Appends a JsonExpr to a JsonList
	
	> JsonAddRange()
	Adds the values of a JsonList to another
	Used for combining two JsonLists
	example: [1, 2, 3] MERGED WITH [4, 5, 6] IS [1, 2, 3, 4, 5, 6]
	Will delete the second list if argument 'reuse_list' is true

	> JsonRemoveElement()
	Removes an element in a JsonList at a specific index. Indexes are always 0 based. 
	Will return a failure if the index is out of bounds
*/

void JsonAppend(JsonList* list, JsonValue* value) {
	JsonValueArrayAppend(list, *value);
}

void JsonAppendString(JsonList* list, JsonString string) {
	JsonValue* value = JsonCreateString(string);
	JsonAppend(list, value);
	free(value);
}

void JsonAppendInt(JsonList* list, JsonInt integer) {
	JsonValue* value = JsonCreateInt(integer);
	JsonAppend(list, value);
	free(value);
}

void JsonAppendFloat(JsonList* list, JsonFloat flt) {
	JsonValue* value = JsonCreateFloat(flt);
	JsonAppend(list, value);
	free(value);
}

void JsonAppendTrue(JsonList* list) {
	JsonValue* value = JsonTrue;
	JsonAppend(list, value);
	free(value);
}

void JsonAppendFalse(JsonList* list) {
	JsonValue* value = JsonFalse;
	JsonAppend(list, value);
	free(value);
}

void JsonAppendNull(JsonList* list) {
	JsonValue* value = JsonNull;
	JsonAppend(list, value);
	free(value);
}

void JsonAppendList(JsonList* list, JsonList* list2) {
	if (list == list2) {
		JsonList* copy = JsonListCopy(list);
		JsonValue* value = JsonValueInit(copy, JSON_LIST);
		JsonValueArrayAppend(list, *value);
	}
	else {
		JsonValue* value = JsonValueInit(list2, JSON_LIST);
		JsonValueArrayAppend(list, *value);
	}
}

void JsonAppendExpr(JsonList* list, JsonExpr* expr) {
	JsonValue* value = JsonValueInit(expr, JSON_EXPR);
	JsonValueArrayAppend(list, *value);
}

void JsonAddRange(JsonList* list, JsonList* list2, int reuse_list) {
	for (ullong i = 0; i < list2->Length; i++) {
		JsonValue* value = &list2->Buffer[i];
		JsonValue* copy = JsonValueCopy(value);

		JsonValueArrayAppend(list, *copy);
		free(copy);
	}

	if (!reuse_list) {
		JsonListDelete(list2);
	}
}

int JsonRemoveElement(JsonList* list, ullong index) {
	if (index > list->Length || index < 0) {
		return FAILURE;
	}

	JsonDataDelete(&list->Buffer[index]);
	JsonValueArrayRemove(list, index);
	return SUCCESS;
}

/*
	Modifying Expr

	FUNCTIONS:

	> JsonSet()
	Adds a JsonValue to a JsonExpr with a key to index it
	If the key exists then it will delete the previous value and override it
	Used for when you want a key to equal a value

	> JsonSetString()
	Creates a JsonValue from a JsonString and adds it to a JsonExpr with a key to index it
	Used for when you want to add a string to a JsonExpr

	> JsonSetInt()
	Creates a JsonValue from a JsonInt and adds it to a JsonExpr with a key to index it
	Used for when you want to add an int to a JsonExpr

	> JsonSetFloat()
	Creates a JsonValue from a JsonFloat and adds it to a JsonExpr with a key to index it
	Used for when you want to add a float to a JsonExpr

	> JsonSetTrue()
	Creates a JsonValue which equals JSON_TRUE (true keyword) and adds it to a JsonExpr with a key to index it
	Used for when you want to set the value of a key to true in a JsonExpr

	> JsonSetFalse()
	Creates a JsonValue which equals JSON_FALSE (false keyword) and adds it to a JsonExpr with a key to index it
	Used for when you want to set the value of a key to false in a JsonExpr

	> JsonSetNull()
	Creates a JsonValue which equals JSON_NULL (null keyword) and adds it to a JsonExpr with a key to index it
	Used for when you want to set the value of a key to null in a JsonExpr

	> JsonSetList()
	Adds a JsonList to a JsonExpr with a key to index it
	If the key exists it will delete the previous value and override it

	> JsonSetExpr()
	Adds a JsonExpr to a JsonExpr with a key to index it
	If the key exists it will delete the previous value to override it
	If you are adding a JsonExpr to itself a copy will be made of the JsonExpr to avoid errors, this may result
	in a very small memory leak as the memory deallocator has lost a reference to the data. This can be fixed by
	manually freeing the list. 

	> JsonMerge()
	Adds the pairs of a JsonExpr to another
	Used for combining two JsonExprs
	If argument "reuse_expr" is correct then the function will delete the JsonExpr which is being copied from. 
	example: {"animal": "Polar Bear"} MERGED WITH {"location": "Snowy Areas"} IS {"animal": "PolarBear", "location": "Snowy Areas"}

	> JsonRemoveKey()
	Removes a pair in a JsonExpr with the key it is associated with
	Will return a failure if the key is not in the JsonExpr
*/

void JsonSet(JsonExpr* expr, const char* key, JsonValue* value) {
	ullong index = JsonGetPairIndex(expr, key);

	if (index == PAIR_INDEX_NOT_FOUND) {
		JsonPair* pair = JsonPairInit(AllocJsonString(key), value);
		JsonPairArrayAppend(expr, *pair);
		free(pair);
	}
	else {
		JsonValueDelete(expr->Buffer[index].Value);
		expr->Buffer[index].Value = value;
	}
}

void JsonSetString(JsonExpr* expr, const char* key, JsonString* string) {
	JsonValue* value = JsonCreateString(string);
	JsonSet(expr, key, value);
}

void JsonSetInt(JsonExpr* expr, const char* key, JsonInt integer) {
	JsonValue* value = JsonCreateInt(integer);
	JsonSet(expr, key, value);
}

void JsonSetFloat(JsonExpr* expr, const char* key, JsonFloat flt) {
	JsonValue* value = JsonCreateFloat(flt);
	JsonSet(expr, key, value);
}

void JsonSetTrue(JsonExpr* expr, const char* key) {
	JsonSet(expr, key, JsonTrue);
}

void JsonSetFalse(JsonExpr* expr, const char* key) {
	JsonSet(expr, key, JsonFalse);
}

void JsonSetNull(JsonExpr* expr, const char* key) {
	JsonSet(expr, key, JsonNull);
}

void JsonSetList(JsonExpr* expr, const char* key, JsonList* list) {
	ullong index = JsonGetPairIndex(expr, key);

	if (index == PAIR_INDEX_NOT_FOUND) {
		JsonString* _key = AllocJsonString(key);
		JsonValue* value = JsonValueInit(list, JSON_LIST);
		JsonPair* pair = JsonPairInit(_key, value);

		JsonPairArrayAppend(expr, *pair);
		free(pair);
	}
	else {
		JsonValueDelete(expr->Buffer[index].Value);
		expr->Buffer[index].Value->Data->List = list;
	}
}

void JsonSetExpr(JsonExpr* expr, const char* key, JsonExpr* expr2) {
	ullong index = JsonGetPairIndex(expr, key);

	if (index == PAIR_INDEX_NOT_FOUND) {
		JsonString* _key = AllocJsonString(key);
		JsonPair* pair;
		
		if (expr == expr2) {
			JsonExpr* copy = JsonExprCopy(expr2);
			JsonValue* value = JsonValueInit(copy, JSON_EXPR);
			pair = JsonPairInit(_key, value);
		}
		else {
			JsonValue* value = JsonValueInit(expr2, JSON_EXPR);
			pair = JsonPairInit(_key, value);
		}
		
		JsonPairArrayAppend(expr, *pair);
		free(pair);
	}
	else {
		JsonValueDelete(expr->Buffer[index].Value);
		expr->Buffer[index].Value->Data->Expr = expr2;
	}
}

void JsonMerge(JsonExpr* expr, JsonExpr* expr2, int reuse_expr) {
	for (ullong i = 0; i < expr2->Length; i++) {
		JsonPair* pair = &expr2->Buffer[i];
		JsonPair* copy = JsonPairCopy(pair);

		JsonPairArrayAppend(expr, *copy);
		free(copy);
	}

	if (!reuse_expr) {
		JsonExprDelete(expr2);
	}
}

int JsonRemoveKey(JsonExpr* expr, const char* key) {
	ullong index = JsonGetPairIndex(expr, key);

	if (index == PAIR_INDEX_NOT_FOUND) {
		return FAILURE;
	}

	JsonPairDelete(&expr->Buffer[index]);
	JsonPairArrayRemove(expr, index);
	return SUCCESS;
}
