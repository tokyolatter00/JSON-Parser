
// Make comments more similar

#include "../src/include/json-parser.h"

/*
	> F001
	Loads a JsonExpr from a string and print it to the console

	INFO:

	This function is an example of how to load a JsonExpr object from a string
	This function exits with 0 memory leaks

	RAW OUTPUT:

	--> {"name": "John", "age": 30, "hobbies": ["Swimming", "Cycling"]}
*/

void F001() {
	// Initialize

	JsonHandler* handler = JsonCreateHandler();
	const char* source = "{\"name\": \"John\", \"age\": 30, \"hobbies\": [\"Swimming\", \"Cycling\"]}";

	// Load Expr & Error Checking

	JsonExpr* expr = JsonLoadString(handler, source);

	if (handler->Error->Exists) {
		printf("Error: %s\n", handler->Error->DebugStr);
		JsonDeleteHandler(handler);
		return;
	}

	// Print Expr to Console

	char* str;
	JsonDumpString(expr, &str);
	printf("--> %s\n", str);

	// Memory Cleanup

	JsonDeleteHandler(handler);
	JsonDeleteExpr(expr);
	free(str);
}

/*
	> F002
	Create Json Expr from scratch and print it to the console

	INFO:

	This function is an example of how to create a JsonExpr object from scratch
	This function exits with 0 memory leaks

	RAW OUTPUT:

	--> {"name": "John", "age": 30}
*/

void F002() {
	// Initialize

	JsonExpr* expr = JsonCreateExpr();

	// Attach to Expr

	JsonSetString(expr, "name", "John");
	JsonSetInt(expr, "age", 30);

	// Print Expr to Console

	char* str;
	JsonDumpString(expr, &str);
	printf("--> %s\n", str);

	// Memory Cleanup

	JsonDeleteExpr(expr);
	free(str);
}

/*
	> F003
	Add a JsonList to a JsonExpr and print it to the console

	INFO:
	
	This function is an example of how to attach a JsonList to a JsonExpr with a key to index it
	This function exits with 0 memory leaks

	RAW OUTPUT:

	--> {"list-example": ["Swimming", "Cycling"]}
*/

void F003() {
	// Initialize

	JsonExpr* expr = JsonCreateExpr();

	// Create Hobbies

	JsonList* hobbies = JsonCreateList();
	JsonAppendString(hobbies, "Swimming");
	JsonAppendString(hobbies, "Cycling");
	
	// Attach to Expr

	JsonSetList(expr, "list-example", hobbies);

	// Print Expr to Console

	char* str;
	JsonDumpString(expr, &str);
	printf("--> %s\n", str);

	// Memory Cleanup

	JsonDeleteExpr(expr);
	free(str);
}

/*
	> F004
	Add a JsonExpr to a JsonExpr and print it to the console

	INFO:

	This function is an example of how to attach a JsonExpr to a JsonExpr with a key to index it
	This function exits with 0 memory leaks

	RAW OUTPUT:

	--> {"expr-example": {"John": "Head Manager", "Mike": "Manager", "Jack": "Toilet Cleaner"}}
*/

void F004() {
	// Initialize

	JsonExpr* expr = JsonCreateExpr();

	// Create Grades

	JsonExpr* employees = JsonCreateExpr();
	JsonSetString(employees, "John", "Head Manager");
	JsonSetString(employees, "Mike", "Manager");
	JsonSetString(employees, "Jack", "Toilet Cleaner");

	// Attach to Expr

	JsonSetExpr(expr, "expr-example", employees);

	// Print Expr to Console

	char* str;
	JsonDumpString(expr, &str);
	printf("--> %s\n", str);

	// Memory Cleanup

	JsonDeleteExpr(expr);
	free(str);
}

/*
	> F005
	Remove an element from a JsonList and print it to the console

	INFO:

	This function is an example of how to remove an element from a JsonList with an integer to index it
	This function exits with 0 memory leaks

	RAW OUTPUT:

	--> {"list-example": [1000, 3000, 4000]}
*/

void F005() {
	// Initialize

	JsonExpr* expr = JsonCreateExpr();

	// Create List

	JsonList* list = JsonCreateList();
	JsonAppendInt(list, 1000);
	JsonAppendInt(list, 2000); // This will be removed
	JsonAppendInt(list, 3000);
	JsonAppendInt(list, 4000);
	
	// Remove Element

	JsonRemoveElement(list, 1);

	// Attach to Expr

	JsonSetList(expr, "list-example", list);

	// Print Expr to Console

	char* str;
	JsonDumpString(expr, &str);
	printf("--> %s\n", str);

	// Memory Cleanup

	JsonDeleteExpr(expr);
	free(str);
}

/*
	> F006
	Remove a pair from a JsonExpr and print it to the console

	INFO:

	This function is an example of how to remove a pair from a JsonExpr with a key to index it
	This function exits with 0 memory leaks

	RAW OUTPUT:

	--> {"one-thousand": 1000, "three-thousand": 3000, "four-thousand": 4000}
*/

void F006() {
	// Initialize

	JsonExpr* expr = JsonCreateExpr();

	// Attach to Expr

	JsonSetInt(expr, "one-thousand", 1000);
	JsonSetInt(expr, "two-thousand", 2000); // This will be removed
	JsonSetInt(expr, "three-thousand", 3000);
	JsonSetInt(expr, "four-thousand", 4000);

	// Remove Pair

	JsonRemoveKey(expr, "two-thousand");

	// Print Expr to Console

	char* str;
	JsonDumpString(expr, &str);
	printf("--> %s\n", str);

	// Memory Cleanup

	JsonDeleteExpr(expr);
	free(str);
}

/*
	> F007
	Merge two JsonList objects into one and print it to the console

	INFO:

	This function will merge two JsonList objects into one, by combining both of their elements
	This function exits with 0 memory leaks
	
	RAW OUTPUT:

	--> {"list-example": [1000, 2000, 3000, 4000]}
*/

void F007() {
	// Create Json Expr

	JsonExpr* expr = JsonCreateExpr();

	// Create First List

	JsonList* list = JsonCreateList();
	JsonAppendInt(list, 1000);
	JsonAppendInt(list, 2000);

	// Create Second List

	JsonList* list2 = JsonCreateList();
	JsonAppendInt(list2, 3000);
	JsonAppendInt(list2, 4000);

	// Combine both Lists

	JsonAddRange(list, list2, FALSE);

	// Attach to Expr

	JsonSetList(expr, "list-example", list);

	// Print Expr to Console

	char* str;
	JsonDumpString(expr, &str);
	printf("--> %s\n", str);

	// Memory Cleanup

	JsonDeleteExpr(expr);
	free(str);
}

/*
	> F008
	Merge two JsonExpr objects into one and print it to the console

	INFO:

	This function will merge two JsonExpr objects into one, by combining both of their pairs
	This function exits with 0 memory leaks

	RAW OUTPUT:

	--> {"name": "John", "age": 20, "dob": "16th March 1998"}
*/

void F008() {
	// Create First Expr

	JsonExpr* expr = JsonCreateExpr();
	JsonSetString(expr, "name", "John");
	JsonSetInt(expr, "age", 20);

	// Create Second Expr

	JsonExpr* expr2 = JsonCreateExpr();
	JsonSetString(expr2, "dob", "16th March 1998");

	// Combine both Exprs

	JsonMerge(expr, expr2, FALSE);

	// Print Expr to Console

	char* str;
	JsonDumpString(expr, &str);
	printf("--> %s\n", str);

	// Memory Cleanup

	JsonDeleteExpr(expr);
	free(str);
}

/*
	> F009
	Dump a JsonExpr object to a file

	INFO:

	This function will create a JsonExpr object and dump it to a file
	This function exits with 0 memory leaks

	RAW OUTPUT (IN FILE):

	{"name": "John", "age": 20}
*/

void F009() {
	// Initialize

	const char* filepath = "file.json";

	// Create Handler

	JsonHandler* handler = JsonCreateHandler();

	// Create Json Expr

	JsonExpr* expr = JsonCreateExpr();
	JsonSetString(expr, "name", "John");
	JsonSetInt(expr, "age", 20);

	// Write To File

	JsonDumpFile(handler, expr, filepath);

	if (handler->Error->Exists) {
		printf("Error: %s\n", handler->Error->DebugStr);
		JsonDeleteHandler(handler);
		return;
	}
	else {
		printf("Successfully wrote to file: %s\n", filepath);
	}

	// Memory Cleanup

	JsonDeleteHandler(handler);
	JsonDeleteExpr(expr);
}

/*
	> F010
	Load a JsonExpr object from a file

	INFO:
	This function will load a JsonExpr object from a file
	This function exits with 0 memory leaks
*/

void F010() {
	// Initialize

	const char* filepath = "file.json";

	// Create Handler

	JsonHandler* handler = JsonCreateHandler();

	// Load Expr & Error Checking

	JsonExpr* expr = JsonLoadFile(handler, filepath);

	if (handler->Error->Exists) {
		printf("Error: %s\n", handler->Error->DebugStr);
		JsonDeleteHandler(handler);
		return;
	}

	// Print Expr to Console

	char* str;
	JsonDumpString(expr, &str);
	printf("--> %s\n", str);

	// Memory Cleanup

	JsonDeleteExpr(expr);
	free(str);
}
