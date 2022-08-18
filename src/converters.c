
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "include/converters.h"

int StringToInt(const char* str, JsonInt** integer) {
	char* extra;
	JsonInt ret = strtoll(str, &extra, 10);

	if ((ret == LLONG_MAX || ret == LLONG_MIN) && errno == ERANGE) {
		return FAILURE;
	}

	size_t i = 0;
	for (size_t i = 0; i < strlen(extra); i++) {
		if (!isspace(extra[i])) {
			return FAILURE;
		}
	}

	*integer = AllocJsonInt(ret);
	return SUCCESS;
}

int StringToFloat(const char* str, JsonFloat** flt) {
	char* extra;
	JsonFloat ret = strtold(str, &extra);

	size_t i = 0;
	for (size_t i = 0; i < strlen(extra); i++) {
		if (!isspace(extra[i])) {
			return FAILURE;
		}
	}

	*flt = AllocJsonFloat(ret);
	return SUCCESS;
}
