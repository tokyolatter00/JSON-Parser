
/*
	> file-io.h
	Header file for defining functions for interacting with the computer's filesystem
	Documentation about the below functions can be found in file-io.c
*/

#pragma once

#include "error.h"

#define SUCCESS 1
#define FAILURE 0

/*
	Reading From File
*/

Error* FileReadAllText(const char* path, const char** text);

/*
	Writing To File
*/

Error* FileWriteAllText(const char* path, const char* text);
