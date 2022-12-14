
# JSON-Parser

## About
This is a JSON parser written in C, this can be used in C or even C++ projects and has lots of functions to work with JSON data. This JSON library was made for convenience and takes care of alot of the processes which go into handling and visualising JSON data. 

### Features
- Read a JSON Object from a string
- Read a JSON Object from a file
- Write a JSON Object to a string
- Write a JSON Object to a file
- Efficient and fast parsing
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

// The above code prints out '--> {"name": "John", "age": 20, "job": "Librarian", "hours-per-week": 25}' to the screen,
// which is what we expected
```

Read JSON Object from a file

```c
#include <stdio.h>
#include "src/include/json-parser.h"

int main(void) {
	// Initialize

	const char* filepath = "examples/example.json";
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

// The above code prints out the contents of the file `examples/example.json`. This is a JSON file which has randomised data,
// with a size of almost 4MB to show this JSON parser is able to read in very large amounts of JSON data.
```

Write Json Object to File

```c
#include <stdio.h>
#include "src/include/json-parser.h"

int main(void) {
	// Initialize

	const char* filepath = "examples/output.json";
	JsonHandler* handler = JsonCreateHandler();
	JsonExpr* expr = JsonCreateExpr();

	// Attach Values

	JsonSetString(expr, "animal-name", "Polar Bear");
	JsonSetString(expr, "average-weight", "775 lbs to 1500 lbs");
	JsonSetString(expr, "average-height", "6 feet to 8 feet");

	// Save to File

	JsonDumpFile(handler, expr, filepath);

	if (handler->Error->Exists) {
		printf("Error: %s\n", handler->Error->DebugStr);
		JsonDeleteHandler(handler);
		return;
	}

	printf("Successfully saved to %s\n", filepath);

	// Memory Cleanup

	JsonDeleteHandler(handler);
	JsonDeleteExpr(expr);
}

// The above code creates the file `examples/output.json` and writes:
// '{"animal-name": "Polar Bear", "average-weight": "775 lbs to 1500 lbs", "average-height": "6 feet to 8 feet"}'
// to it, which is what we expected
```

The examples above all exit with 0 memory leaks as the correct functions were used to delete objects that were allocated. For more examples you can take a look at `examples/example-1.c`, this C file contains some functions which contain examples of different ways to use and interact with this library

