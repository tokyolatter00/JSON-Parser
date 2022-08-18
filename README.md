
# JSON-Parser

## About
This is a JSON parser written in C, this can be used in C/C++ projects and has lots of functions to work with JSON data. This JSON library was made for convenience and handles lots of the complicated processes which go into handling and visualising JSON data. 

### Features
- Read a JSON Object from a string
- Read a JSON Object from a file
- Write a JSON Object to a string
- Write a JSON Object to a file

### Examples

Read JSON Object from a string

```c
#include <stdio.h>
#include "src/include/json-parser.h"

int main(void) {
   	// Initialize

	const char* source = "{\"name\": \"John\", \"age\": 20}";
	JsonHandler* handler = JsonCreateHandler();

	// Load Json Object

	JsonExpr* expr = JsonLoadString(handler, source);

	// Check For Errors

	if (handler->Error->Exists) {
		printf("Error: %s\n", handler->Error->DebugStr);
		JsonDeleteHandler(handler);
		return;
	}

	// Print to Console

	char* str;
	JsonDumpString(expr, &str);
	printf("--> %s\n", str);

	// Memory Cleanup

	JsonDeleteHandler(handler);
	JsonDeleteExpr(expr);
}
```
