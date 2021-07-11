#include "engine/array.h"

#include "stdbool.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

static size_t availableID = 0;

static Array tmpArrayPointers;

void Array_init(Array *array_p,  unsigned int itemSize){

	//array_p->items = malloc(maxLength * itemSize);
	array_p->maxLength = 16;
	array_p->items = malloc(array_p->maxLength * itemSize);
	array_p->itemSize = itemSize;
	array_p->length = 0;

}

void Array_free(Array *array_p){
	free(array_p->items);
}

void EntityHeader_init(EntityHeader *entityHeader_p){

	entityHeader_p->ID = availableID;
	availableID++;

}

void *Array_addItem(Array *array_p){
	
	array_p->length++;

	if(array_p->length > array_p->maxLength){
		array_p->maxLength += 16;
		array_p->items = realloc(array_p->items, (array_p->maxLength) * array_p->itemSize);
	}

	return Array_getItemPointerByIndex(array_p, array_p->length - 1);

}

void *Array_getItemPointerByIndex(Array *array_p, unsigned int index){

	return (void *)((char *)array_p->items + index * array_p->itemSize);

}

void *Array_getItemPointerByID(Array *array_p, size_t ID){

	for(int i = 0; i < array_p->length; i++){

		void *itemPointer = Array_getItemPointerByIndex(array_p, i);

		if(((EntityHeader *)itemPointer)->ID == ID){
			return itemPointer;
		}

	}

	//printf("***\n");
	//printf("Array_getItemPointerByID\n");
	//printf("COULD NOT FIND ARRAY ITEM WITH ID: %i\n", ID);
	//printf("***\n");

	return NULL;

}

unsigned int Array_getItemIndexByID(Array *array_p, size_t ID){

	for(int i = 0; i < array_p->length; i++){

		void *itemPointer = Array_getItemPointerByIndex(array_p, i);

		if(((EntityHeader *)itemPointer)->ID == ID){
			return i;
		}

	}

	printf("***\n");
	printf("Array_getItemIndexByID\n");
	printf("COULD NOT FIND ARRAY ITEM WITH ID: %i\n", ID);
	printf("***\n");

}

void Array_removeItemByIndex(Array *array_p, unsigned int index){

	if(index >= array_p->length){
		printf("***\n");
		printf("TRIED REMOVING ITEM AT AN INDEX BIGGER THAN THE ARRAY LENGTH");
		printf("array pointer: %p\n", array_p);
		printf("index: %i\n", index);
		printf("array length: %i\n", array_p->length);
		printf("***\n");
		return;
	}
	
	memcpy(
		(char *)array_p->items + index * array_p->itemSize,
		(char *)array_p->items + (index + 1) * array_p->itemSize,
		(array_p->length - index) * array_p->itemSize
	);

	array_p->length--;
}

void Array_removeItemByID(Array *array_p, size_t ID){

	unsigned int index = Array_getItemIndexByID(array_p, ID);

	Array_removeItemByIndex(array_p, index);

}

void Array_clear(Array *array_p){

	while(array_p->length > 0){
		Array_removeItemByIndex(array_p, 0);
	}

}

void initTmpArrays(){
	Array_init(&tmpArrayPointers, sizeof(Array *));
}

void Array_addToTmpArrays(Array *array_p){
	Array **tmpArrayPointer_p = Array_addItem(&tmpArrayPointers);
	*tmpArrayPointer_p = array_p;
}

void freeTmpArrays(){
	while(tmpArrayPointers.length > 0){
		Array *tmpArrayPointer_p = Array_getItemPointerByIndex(&tmpArrayPointers, 0);
		Array_free(tmpArrayPointer_p);
	}
}

/*
void String_init(String *string_p, char *text){

	string_p->characters = malloc(sizeof(text));

	for(int i = 0; i < strlen(text); i++){
		string_p->characters[i] = text[i];
	}

}

void String_setText(String *string_p, char *text){

	free(string_p->characters);

	string_p->characters = malloc(sizeof(text));

	for(int i = 0; i < strlen(text); i++){
		string_p->characters[i] = text[i];
	}

}

void String_concatText(String *string_p, char *text){

}

void String_concatString(String *string_p, String concatString){

}

void String_concatInt(String *string_p, int number){

}

void String_free(String *string_p){

	free(string_p->characters);

}
*/

bool compareFloatToFloat(float a, float b){
	return fabs(a - b) < 0.000001;
}

void IndexSafeArray_init(IndexSafeArray *indexSafeArray_p, unsigned int itemSize, unsigned int maxLength){

	indexSafeArray_p->itemSize = itemSize;
	indexSafeArray_p->indexSize = itemSize + sizeof(bool);
	indexSafeArray_p->length = 0;
	//indexSafeArray_p->spaces = 0;
	indexSafeArray_p->maxLength = maxLength;
	
	indexSafeArray_p->items = malloc(maxLength * indexSafeArray_p->indexSize);

	for(int i = 0; i < maxLength; i++){
		bool *flag = indexSafeArray_p->items + i * indexSafeArray_p->indexSize;
		*flag = false;
	}

}

unsigned int IndexSafeArray_addItem(IndexSafeArray *indexSafeArray_p){

	for(int i = 0; i < indexSafeArray_p->maxLength; i++){

		bool *flag = indexSafeArray_p->items + i * indexSafeArray_p->indexSize;

		if(!*flag){

			*flag = true;
			indexSafeArray_p->length++;

			return i;
			//return (void *)flag + sizeof(bool);

		}
		
	}

	printf("ARRAY IS FULL! COULD NOT ADD ITEM!\n");
	printf("ArrayPointer: %p\n", indexSafeArray_p);

}

void *IndexSafeArray_getItemPointer(IndexSafeArray *indexSafeArray_p, unsigned int index){

	bool *flag = indexSafeArray_p->items + index * indexSafeArray_p->indexSize;

	if(*flag){
		return (void *)flag + sizeof(bool);
	}

	return NULL;

}

void IndexSafeArray_removeItem(IndexSafeArray *indexSafeArray_p, unsigned int index){

	bool *flag = indexSafeArray_p->items + index * indexSafeArray_p->indexSize;

	*flag = false;
	
}

void IndexSafeArray_clear(IndexSafeArray *indexSafeArray_p){
	for(int i = 0; i < indexSafeArray_p->length; i++){
		IndexSafeArray_removeItem(indexSafeArray_p, i);
	}
}

void IndexSafeArray_free(IndexSafeArray *indexSafeArray_p){
	free(indexSafeArray_p->items);
	indexSafeArray_p->items = NULL;
}
