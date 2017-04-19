/*
Name:Darren Ytsma
student number:301171125
SFU username:dytsma
Lecture section: CPMT 300 D100
instructor: Brian Booth
TA: Scott Kristjanson
*/

#include <time.h>
#include <string.h>
#ifndef _FUNctions_H_
#define _FUNctions_H_

char* getTime();
void inputs(int inputs);
void OpenReadFile(char *filename );
void convertToNum(char *six, int *temp,int change );
void convertTochar(char* encrypted_string, int* temp,int change,int group_start );
void fixchar(char* loc, char* fix);
#endif