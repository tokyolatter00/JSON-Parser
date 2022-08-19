
# JSON-Parser

## About
This is a JSON parser written in C, this can be used in C or even C++ projects and has lots of functions to work with JSON data. This JSON library was made for convenience and takes care of alot of the processes which go into handling and visualising JSON data. 

### Features
- Read a JSON Object from a string
- Read a JSON Object from a file
- Write a JSON Object to a string
- Write a JSON Object to a file
- Clean and efficient memory management

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

	// Add Some Values

	JsonSetString(expr, "job", "Librarian");
	JsonSetInt(expr, "hours-per-week", 25);

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
	free(str);
}
```

The above code prints out `--> {"name": "John", "age": 20, "job": "Librarian", "hours-per-week": 25}` to the screen, which is what we expected

Read JSON Object from a file

```c
#include <stdio.h>
#include "src/include/json-parser.h"

int main(void) {
	// Initialize

	const char* filepath = "exaamples/example.json";
	JsonHandler* handler = JsonCreateHandler();

	// Load Json Object

	JsonExpr* expr = JsonLoadFile(handler, filepath);

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
	free(str);
}
```

The examples above all exit with 0 memory leaks as the correct functions were used to delete objects that were allocated. For more examples you can take a look at `examples/example-1.c`, this C file contains some functions which contain examples of different ways to use and interact with this library

