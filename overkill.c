#include "headers.h"
#include "overkill.h"
#include "other.h"

int overkill(char *command)
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
    if (word_count != 1)
    {
        error_code=1;
        fprintf(stderr, "overkill: Invalid number of arguments\n");
        return -1;
    }
    else
    {
        struct bg_prc *a = malloc(sizeof(struct bg_prc));
        a = initial;
        if (initial->size == 0)
        {
            return 1;
        }
        else
        {
            while (1)
            {
                if(kill(a->pid, SIGKILL)!=0)
                {
                    error_code=1;
                }
                signal(SIGCHLD, func);
                if(a->next==NULL)
                {
                    break;
                }
                else
                {
                    a=a->next;   
                }
            }
            return 1;
        }
    }
}