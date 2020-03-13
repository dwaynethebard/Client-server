
#include <time.h>
#include <string.h>
#ifndef _functions_H_
#define _functions_H_

char* getTime();
void inputs(int inputs);
void OpenReadFile(char *filename );
void convertToNum(char *six, int *temp,int change );
void convertTochar(char* encrypted_string, int* temp,int change,int group_start );
void fixchar(char* loc, char* fix);
#endif
