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
void findType(char *line, FILE *fptr)
{
    int start=0, end=0, doubleOp =0;
    char token[256], operator[2];
    //runs through entire line of document
    for(int i = 0; i < strlen(line); i++)
    {
        //adjusts starting point when start is a blank
        if (i > start && line[start] ==' ')
        {
            start = i;
        }

        

        //detects if there is a comment
        if (line[i] == '/' && line[i+1] == '*')
        {
            end = i;
            if(end-start > 1 || (end-start == 1 && isalnum(line[i-1])))
            {
                strncpy(token, line+start, end-start);
                if(isdigit(line[i-1]))
                {
                    printf("%c (numeric literal)\n", line[i-1]);
                }
                else
                {
                    sort(token);
                }
                memset(token, '\0', sizeof(token));
            }
            i = isComment(line, i, fptr);
            i = i+2;
            start = i;
            end = start;
        }

        else if(isdigit(line[i]))
        {
            end = i;
            if(end-start > 1 || (end-start == 1 && isalnum(line[i-1])))
            {
                strncpy(token, line+start, end-start);
                sort(token);
                memset(token, '\0', sizeof(token));
            }
            i = isNumber(line, i, fptr);
            start =i;
            end = start;
        }

        else if ((ispunct(line[i]) || line[i] == '<' || line[i] == '>' || line[i] == '=') && line[i] != '_')
        {
            end = i;
            if(end-start > 1 || (end-start == 1 && isalnum(line[i-1])))
            {
                strncpy(token, line+start, end-start);
                if(isdigit(line[i-1]))
                {
                    printf("%c (numeric literal)\n", line[i-1]);
                }
                else
                {
                    sort(token);
                }
                memset(token, '\0', sizeof(token));
            }

            if ((ispunct(line[i+1]) || line[i+1] == '<' || line[i+1] == '>' || line[i+1] == '=') && line[i+1] != '_')
            {
                operator[0] = line[i];
                operator[1] = line[i+1];
                for(int j = 0; j < OPERATORS_COUNT; j++) 
                {
                    //if match is found
                    if(strcmp(operator, operatorsList[j]) == 0) 
                    {
                        //prints the token follow by keyword
                        doubleOp = 1;
                    }
                }
                if(doubleOp == 1)
                {
                    printf("%s (operator)\n", operator);
                    i = i+2;
                    start = i;
                    end = start;
                }
                else
                {
                    isOperator(line, i, fptr);
                    start = i+1;
                    end = start;
                }
                doubleOp = 0;
                memset(operator, '\0', sizeof(operator));
            }

            else if(line[i] == '"')
            {
                i = isString(line, i, fptr);
                start = i+1;
                end = start;
            }

            else if(line[i] == 39)
            {
                printf("'%c' (character literal)\n", line[i+1]);
                i = i+2;
                start = i+1;
                end = start;
            }

            else
            {
                isOperator(line, i, fptr);
                start = i+1;
                end = start;
            }
            
        }
                
        //if starting point is fine, updates ending point and sorts token
        else if (i > start && (line[i] == ' ' || line[i] == '\n'))
        {
            end = i;
            strncpy(token, line+start, end-start);
            sort(token);
            start = i+1;
            end = start;
            memset(token, '\0', sizeof(token));
        }
    }
}

void isOperator(char *line, int position, FILE *fptr)
{
    int found = 0;
    char operator[1];
    operator[0] = line[position];
    for(int j = 0; j < OPERATORS_COUNT; j++) 
    {
        //if match is found
        if(strcmp(operator, operatorsList[j]) == 0) 
        {
            printf("%c (operator)\n", line[position]);
            found = 1;
        }
    }
    if(found == 0)
    {
        printf("%c (UNK)\n", line[position]);
    }
}

//prints comment after deteced
int isComment(char *line, int position, FILE *fptr)
{
    do 
    {
        //runs through entire line
        for(int i = position; i < strlen(line); i++)
        {
            //prints each character from line
            printf("%c", line[i]);
            //checks for comment end within line
            if(line[i] == '*' && line[i+1] == '/')
            {
                //when end dectected marks as comment and returns position after comment ends
                printf("%c (comment)\n", line[i+1]);
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
int isString(char *line, int position, FILE *fptr)
{
    int first = 0;
    do 
    {
        //runs through entire line
        for(int i = position; i < strlen(line); i++)
        {
            //prints each character from line
            printf("%c", line[i]);
            //checks for string end within line
            if(line[i] == '"' && first != 0)
            {
                //when end dectected marks as string and returns position after string ends
                printf(" (string)\n");
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
int isNumber(char *line, int position, FILE *fptr)
{
    do 
    {
        //runs through entire line
        for(int i = position; i < strlen(line); i++)
        {
            //checks for string end within line
            if(isdigit(line[i]))
            {
                printf("%c", line[i]);
            }
            else if(line[i] == '#' && isdigit(line[i-1]))
            {
                printf("%c", line[i]);
            }
            else if(line[i] =='.' && isdigit(line[i-1]) && isdigit(line[i+1]))
            {
                printf("%c", line[i]);
            }
            else
            {
                printf(" (numeric literal)\n");
                return i;
            }
        }
        //resets starting position for multi-lined strings
        position = 0;
        //looks through FILE
    }while(fgets(line, 256, fptr));
    return -1;
}

void sort(char *token)
{
    //compares token to keyword list
    for(int i = 0; i < KEYWORDS_COUNT; i++) {
        //if match is found
        if(strcmp(token, keywordsList[i]) == 0) 
        {
            //prints the token follow by keyword
            printf("%s (keyword)\n", token);
            return;
        }
    }
    printf("%s (identifier)\n", token);
}