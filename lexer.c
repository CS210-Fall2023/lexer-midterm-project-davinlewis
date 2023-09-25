/**
 * @file   lexer.c
 * @author Davin Lewis
 * @brief  Functions for a program accepts a file then lexes file into a new file with same named plus the extension ".lexer"
 * @date   09/24/2023
 *
 */

#include "lexer.h"
//for comparing to keywordlist
const int KEYWORDS_COUNT = 37;
char* keywordsList[] = { "accessor", "and", "array", "begin", "bool", "case", "character", "constant", "else", "elsif", "end", "exit", "function",
    "if", "in", "integer", "interface", "is", "loop", "module", "mutator", "natural", "null", "of", "or", "other", "out",
    "positive", "procedure", "range", "return", "struct", "subtype", "then", "type", "when", "while" };
//for comparing to operators list
const int OPERATORS_COUNT = 27;
char *operatorsList[] = { ".",   "<",   ">",   "(",   ")",   "+",   "-",  "*",   "/",   "|",   "&",   ";",   ",",   ":",
    "[",   "]", "=",   ":=",   "..",   "<<",   ">>",   "<>",   "<=",   ">=",   "**",   "!=",   "=>"};


//reads through lines, creating tokens, sending to functions for sorting
void findType(char *line, FILE *fptr, FILE *write)
{
    int start=0, end=0, doubleOp =0;
    //temp strings
    char token[256], operator[2];
    memset(token, '\0', sizeof(token));
    //runs through entire line of document
    for(int i = 0; i < strlen(line); i++)
    {
        //adjusts starting point when start is a blank
        if (i > start && (line[start] ==' ' || line[start] == '\n'))
        {
            start = i;
        }

        //detects if there is a comment
        if ((line[i] == '/' && line[i+1] == '*') || (line[i] == '*' && line[i-1] == '/'))
        {
            end = i;
            if(end-start > 1 || (end-start == 1 && isalnum(line[i-1])))
            {
                strncpy(token, line+start, end-start);
                if(isdigit(line[i-1]))
                {
                    fprintf(write, "%c (numeric literal)\n", line[i-1]);
                }
                else
                {
                    sort(token, write);
                }
                memset(token, '\0', sizeof(token));
            }
            i = isComment(line, i, fptr, write);
            i = i+2;
            start = i;
            end = start;
        }

        //detects if there's a number 
        else if(isdigit(line[i]) && !isalpha(line[i-1]))
        {
            i = isNumber(line, i, fptr, write);
            start =i;
            end = start;
        }

        //detects if there's punctuation
        if ((ispunct(line[i]) || line[i] == '<' || line[i] == '>' || line[i] == '=') && line[i] != '_')
        {
            end = i;
            //creates token for anything that came before the the symbol and sorts it
            if(end-start > 1 || (end-start == 1 && isalnum(line[i-1])))
            {
                strncpy(token, line+start, end-start);
                if(isdigit(line[i-1]) && !isalpha(line[i-2]))
                {
                    fprintf(write, "%c (numeric literal)\n", line[i-1]);
                }
                else
                {
                    sort(token, write);
                }
                memset(token, '\0', sizeof(token));
            }
            //detects if the symbol means a string
            if(line[i] == '"')
            {
                i = isString(line, i, fptr, write);
                start = i+1;
                end = start;
            }
            //detects if the symbol means a character literal
            else if(line[i] == 39)
            {
                fprintf(write, "'%c' (character literal)\n", line[i+1]);
                i = i+2;
                start = i+1;
                end = start;
            }
            //detects if there's 2 symbols in a row
            else if ((ispunct(line[i+1]) || line[i+1] == '<' || line[i+1] == '>' || line[i+1] == '=') && line[i+1] != '_')
            {
                operator[0] = line[i];
                operator[1] = line[i+1];
                //checks if the 2 integers are a double operator or 2 operators next to each other
                for(int j = 0; j < OPERATORS_COUNT; j++) 
                {
                    //if match is found
                    if(strcmp(operator, operatorsList[j]) == 0) 
                    {
                        //prints the token follow by keyword
                        doubleOp = 1;
                    }
                }
                //if a double operator
                if(doubleOp == 1)
                {
                    fprintf(write, "%s (operator)\n", operator);
                    i = i+2;
                    start = i;
                    end = start;
                }
                //if 2 seperate operators
                else
                {
                    isOperator(line, i, fptr, write);
                    start = i+1;
                    end = start;
                }
                doubleOp = 0;
                memset(operator, '\0', sizeof(operator));
            }
            //any symbols that don't have special requirements get checked if they're an operator
            else
            {
                isOperator(line, i, fptr, write);
                start = i+1;
                end = start;
            }
            
        }
                
        //if starting point is fine, updates ending point and sorts token
        else if (i > start && (line[i] == ' ' || line[i] == '\n'))
        {
            //creates token a identifies it
            end = i;
            strncpy(token, line+start, end-start);
            sort(token, write);
            start = i;
            end = start;
            memset(token, '\0', sizeof(token));
        }
    }
}

//checks whether a symbol is an operator or unknown
void isOperator(char *line, int position, FILE *fptr, FILE *write)
{
    int found = 0;
    char operator[1];
    operator[0] = line[position];
    //runs through list of operators to verify status
    for(int j = 0; j < OPERATORS_COUNT; j++) 
    {
        //if match is found
        if(strcmp(operator, operatorsList[j]) == 0) 
        {
            fprintf(write, "%c (operator)\n", line[position]);
            found = 1;
        }
    }
    //if symbol isn't an operator it's unknown
    if(found == 0)
    {
        fprintf(write, "%c (UNK)\n", line[position]);
    }
}

//prints comment after deteced
int isComment(char *line, int position, FILE *fptr, FILE *write)
{
    //debugs in the case of starting / not being detected
    if (line[position] == '*')
    {
        fprintf(write, "%c", line[position-1]);
    }
    do 
    {
        //runs through entire line
        for(int i = position; i < strlen(line); i++)
        {
            //prints each character from line
            fprintf(write, "%c", line[i]);
            //checks for comment end within line
            if(line[i] == '*' && line[i+1] == '/')
            {
                //when end dectected marks as comment and returns position after comment ends
                fprintf(write, "%c (comment)\n", line[i+1]);
                return i++;
            }
        }
        //resets starting position for multi-lined comments
        position = 0;
        //looks through FILE
    }while(fgets(line, 256, fptr));
    return -1;
}

//prints string after deteced
int isString(char *line, int position, FILE *fptr, FILE *write)
{
    int first = 0;
    do 
    {
        //runs through entire line
        for(int i = position; i < strlen(line); i++)
        {
            //prints each character from line
            fprintf(write, "%c", line[i]);
            //checks for string end within line
            if(line[i] == '"' && first != 0)
            {
                //when end dectected marks as string and returns position after string ends
                fprintf(write, " (string)\n");
                return i;
            }
            first++;
        }
        //resets starting position for multi-lined strings
        position = 0;
        //looks through FILE
    }while(fgets(line, 256, fptr));
    return -1;
}

//prints string after deteced
int isNumber(char *line, int position, FILE *fptr, FILE *write)
{
    //debugs in the case of missing a number beforehand
    if (isdigit(line[position-1]))
            {
                fprintf(write, "%c", line[position-1]);
            }
    do 
    {
        //runs through entire line
        for(int i = position; i < strlen(line); i++)
        {
            //checks for string end within line
            if(isdigit(line[i]))
            {
                fprintf(write, "%c", line[i]);
            }
            else if(line[i] == '#' && isdigit(line[i-1]))
            {
                fprintf(write, "%c", line[i]);
            }
            else if(line[i] =='.' && isdigit(line[i-1]) && isdigit(line[i+1]))
            {
                fprintf(write, "%c", line[i]);
            }
            else
            {
                fprintf(write, " (numeric literal)\n");
                return i;
            }
        }
        //resets starting position for multi-lined strings
        position = 0;
        //looks through FILE
    }while(fgets(line, 256, fptr));
    return -1;
}

//sorts words as either keywords or identifiers
void sort(char *token, FILE *write)
{
    //compares token to keyword list
    for(int i = 0; i < KEYWORDS_COUNT; i++) {
        //if match is found
        if(strcmp(token, keywordsList[i]) == 0) 
        {
            //prints the token follow by keyword
            fprintf(write, "%s (keyword)\n", token);
            return;
        }
    }
    fprintf(write, "%s (identifier)\n", token);
}