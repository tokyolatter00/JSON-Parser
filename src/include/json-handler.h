
#pragma once

#include "error.h"

#define JsonHandlerDelete(handler)			\
	ErrorDelete(handler->Error);			\
	free(handler);

typedef struct {
	Error* Error;
} JsonHandler;

JsonHandler* JsonHandlerInit();
