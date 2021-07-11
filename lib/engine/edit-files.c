#include "engine/edit-files.h"
#include "engine/array.h"

#include "string.h"
#include "stdlib.h"

void FileData_init(FileData *fileData_p){
	
	Array_init(&fileData_p->lines, sizeof(char *));

}

void FileData_addLine(FileData *fileData_p, char *characters){

	char **line_p = Array_addItem(&fileData_p->lines);

	*line_p = malloc(sizeof(char) * 255);

	memset(*line_p, *"\0", 255);

	memcpy(*line_p, characters, strlen(characters));

}

FileData readFile(char *path){

}

void writeFile(FileData fileData){

}

void printFileData(FileData fileData){

}
