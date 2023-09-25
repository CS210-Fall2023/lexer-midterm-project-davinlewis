#ifndef LEXER_H
#define LEXER_H

//libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//functions
void findType(char *line, FILE *fptr);
int isComment(char *line, int position, FILE *fptr);
int isString(char *line, int position, FILE *fptr);
int isNumber(char *line, int position, FILE *fptr);
void isOperator(char *line, int position, FILE *fptr);
void sort(char* token);

#endif       