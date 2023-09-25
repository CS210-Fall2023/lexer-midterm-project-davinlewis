#include "lexer.h"

int main(int argc, char *argv[])
{
    //if FILE opens
    if (argc > 1)
    {
    FILE *fptr;
    // Open a FILE in read mode
    fptr = fopen(argv[1], "r");
    // Storage for lines of the FILE
    char myString[256];
    // Read the content and store it inside myString until end of FILE
    while(fgets(myString, 256, fptr)) 
    {
        //parses and categorizes tokens
        findType(myString, fptr);
    }

    fclose(fptr);
    }
    //if FILE fails to open
    else
    {
        printf("ERROR: NO FILE ARGUMENT SPECIFIED\n");
    }
    return 0;
}