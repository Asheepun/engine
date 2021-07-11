#ifndef EDIT_FILES_H_
#define EDIT_FILES_H_

#include "engine/array.h"

typedef struct FileData{
	Array lines;
}FileData;

void FileData_init(FileData *);

void FileData_addLine(FileData *, char *);

FileData readFile(char *);

void writeFile(FileData);

#endif
