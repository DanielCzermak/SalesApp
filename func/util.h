#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void clr();
void stdinClear();
void CheckDefaultFiles();
bool DoesFileExist(char*);
bool DoesFolderExist(char*);
void CreateFolder(char*);
bool isFileEmpty(char*);
int GetFileLength(char*);
char *ReadDynStr();
char *ReadLimDynStr(int);
bool HasDigits(char*);
bool HasOnlyDigits(char*);

#endif // UTIL_H_INCLUDED
