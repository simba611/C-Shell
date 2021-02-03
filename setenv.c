#include "headers.h"
#include "setenv.h"

int sen(char *command)
{
    error_code=0;
    char com[1024][1024];
    int word_count = 0;
    int char_count = 0;
    for (int i = 0; i < strlen(command); i++)
    {
        if (command[i] != ' ' && command[i] != '\t' && command[i] != '\0')
        {
            while (command[i] != ' ' && command[i] != '\t' && command[i] != '\0')
            {
                com[word_count][char_count] = command[i];
                i++;
                char_count++;
            }
            com[word_count][char_count] = '\0';
            word_count++;
            char_count = 0;
        }
    }
    if(word_count<2||word_count>3)
    {
        error_code=1;
        fprintf(stderr, "setenv: Invalid number of arguments\n");
        return -1;
    }
    else if(word_count==2)
    {
        int che=setenv(com[1], "", 1);
        if(che!=0)
        {
            error_code=1;
            fprintf(stderr, "setenv: Error occured\n");
            return -1;
        }
    }
    else 
    {
        int c=setenv(com[1], com[2], 1);
        if(c!=0)
        {
            error_code=1;
            fprintf(stderr, "setenv: Error occured\n");
            return -1;
        }
    }
    return 1;
}