
#include <stdlib.h>
#include "include/error.h"

/*
	Initializing Data

	FUNCTIONS:

	> ErrorInit()
	Initialize an Error object
*/

Error* ErrorInit() {
	Error* error = calloc(1, sizeof(Error));
	error->DebugStr = NULL;
	error->Exists = FALSE;

	return error;
}
