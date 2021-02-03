#include "headers.h"
#include "kjob.h"
#include "chint.h"
#include "other.h"

int kjob(char *command)
{
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
    if(word_count!=3)
    {
        error_code=1;
        fprintf(stderr, "kjobs: Incorrect number of arguments\n");
        return -1;
    }
    else
    {
        if(chint(com[1])==-1 ||chint(com[2])==-1)
        {
            error_code=1;
            fprintf(stderr, "kjobs: Invalid argument, expected an integer\n");
            return -1;
        }
        else
        {
            int j_num = atoi(com[1]);
            int s_num = atoi(com[2]);
            struct bg_prc* a = malloc(sizeof(struct bg_prc));
            a=initial;
            if(j_num>a->size)
            {
                error_code=1;
                fprintf(stderr, "kjob: Job number doesn't exist\n");
                return -1;
            }
            else
            {
                int trav=1;
                while(trav<j_num)
                {
                    a=a->next;
                    trav++;
                }
                int pi_num=a->pid;
                signal(SIGCHLD, func);
                if(kill(pi_num, s_num)==0)
                {
                    error_code=0;
                }
                else
                {
                    error_code=1;
                }
            }
        }
    }
    return 1;
}