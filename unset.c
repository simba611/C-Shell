#include "headers.h"
#include "unset.h"

int unsen(char *command)
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
    if(word_count!=2)
    {
        error_code=1;
        fprintf(stderr, "unsetenv: Invalid number of arguments\n");
        return -1;
    }
    else
    {
       int uc = unsetenv(com[1]);   
       if(uc<0)
       {
           error_code=1;
           fprintf(stderr, "unsetenv: Error occured\n");
           return -1;
       }
       else
       {
           return 1;
       }
    }
}
