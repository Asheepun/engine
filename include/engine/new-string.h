#ifndef NEW_STRING_H_
#define NEW_STRING_H_

#include "engine/array.h"

typedef struct String{
	char characters[1024];
	int length;
}String;

void String_init(String *, char *);

#endif
