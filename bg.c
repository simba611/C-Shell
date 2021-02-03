#include "headers.h"
#include "bg.h"

int bg(char *command)
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
    if(word_count!=2)
    {
        fprintf(stderr, "bg: Invalid number of arguments\n");
        error_code=1;
        return -1;
    }
    else
    {
        if(chint(com[1])==-1 || atoi(com[1])>initial->size)
        {
            fprintf(stderr, "bg: Enter valid job number\n");
            error_code=1;
        }
        else
        {
            int job_num=atoi(com[1]);
            int trv=1;
            struct bg_prc* a = malloc(sizeof(struct bg_prc));
            a=initial;
            while(trv<job_num)
            {
                a=a->next;
                trv++;
            }  
            if(kill(a->pid, SIGCONT)==0)
            {
                error_code=0;
            }
            else
            {
                fprintf(stderr, "kill: kill could not execute\n");
                error_code=1;   
            }
            return 1;
        }
    }
}