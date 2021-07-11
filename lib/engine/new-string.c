#include "engine/new-string.h"
#include "engine/array.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void String_init(String *string_p, char *characters){

	printf("CHECK, %i\n", strlen(characters));
	
	memset(&string_p->characters, *"\0", 1024);

	printf("again\n");

	memcpy(&string_p->characters, characters, strlen(characters));

	printf("booob\n");

	string_p->length = strlen(characters);

	printf("chacka\n");

}

void String_appendString(String *string_p, String newString){

	memcpy(&string_p->characters + string_p->length, &newString.characters, newString.length);

}
