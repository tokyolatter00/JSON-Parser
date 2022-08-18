
#include "../src/include/json-parser.h"

/*
	In this example we will create a JSON expression from a string
	We will add some values to this expression as well as remove some existing ones
	We will print the expression to the screen after dumping it to a string
*/

void ExampleTwo() {
	/* Initialize */

	const char* source = "{\"name\": \"John\", \"age\": 32, \"personality\": [\"Kind\", \"Helpful\"]}";
	JsonHandler* handler = JsonCreateHandler();

	/* Parse String */

	JsonExpr* expr = JsonLoadString(handler, source);

	if (handler->Error->Exists) {
		// Error occured while parsing the string
		printf("Error: %s\n", handler->Error->DebugStr);
		return;
	}

	/* Change Age */

	JsonSetInt(expr, "age", 33); // Change the age from 32 to 33 (it was his birthday yesterday :D)

	/* Change Personality Values */

	JsonList* personality;

	if (JsonGetList(expr, "personality", &personality)) {
		JsonRemoveElement(personality, 1);
		JsonAppendString(personality, "Thoughtful");
	}

	/* Add Job Expression */

	JsonExpr* job = JsonCreateExpr();
	JsonSetString(job, "name", "Librarian");
	JsonSetString(job, "position", "Manager");
	JsonSetInt(job, "weekly hours", 40);

	JsonSetExpr(expr, "job", job);

	/* Print To Console */

	char* str;
	JsonDumpString(expr, &str);
	printf(">>> %s\n", str);

	/* Free Memory & Exit */

	JsonDeleteHandler(handler);
	JsonDeleteExpr(expr);
	free(str);
}
