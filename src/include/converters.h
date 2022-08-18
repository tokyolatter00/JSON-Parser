
/*
	converters.h
	- Contains features for converting between datatypes
*/

#pragma once

#include "json-types.h"

#define SUCCESS 1
#define FAILURE 0

int StringToInt(const char* str, JsonInt** integer);
int StringToFloat(const char* str, JsonFloat** flt);
