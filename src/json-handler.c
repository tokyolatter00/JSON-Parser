
#include <stdlib.h>
#include "include/json-handler.h"

JsonHandler* JsonHandlerInit() {
	JsonHandler* handler = calloc(1, sizeof(JsonHandler));
	handler->Error = ErrorInit();

	return handler;
}
