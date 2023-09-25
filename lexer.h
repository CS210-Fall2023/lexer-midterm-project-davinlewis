#ifndef LEXER_H
#define LEXER_H

//libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//functions
void findType(char *line, FILE *fptr, FILE *write);
int isComment(char *line, int position, FILE *fptr, FILE *write);
int isString(char *line, int position, FILE *fptr, FILE *write);
int isNumber(char *line, int position, FILE *fptr, FILE *write);
void isOperator(char *line, int position, FILE *fptr, FILE *write);
void sort(char* token, FILE *write);

#endif       