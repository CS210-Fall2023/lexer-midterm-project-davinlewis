/**
 * @file   lexer.h
 * @author Davin Lewis
 * @brief  Header file for program accepts a file then lexes file into a new file with same named plus the extension ".lexer"
 * @date   09/24/2023
 *
 */

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