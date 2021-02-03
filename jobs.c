#include "headers.h"
#include "jobs.h"

char getstatus(int pid)
{
    char path[1024];
    sprintf(path,"/proc/%d/status", pid);
    FILE* fd = fopen(path, "r");
    if(fd==NULL)
    {
        fprintf(stderr, "jobs: Error: could not open %s", path);
        return '1';
    }
    else
    {
        int linecount=0;
        char line[1024];
        while(linecount<3)
        {
            fgets(line,sizeof(line), fd);
            linecount++;
        }
        int trav=0;
        while(line[trav]!='\t')
        {
            trav++;
        }
        fclose(fd);
        return line[trav+1];
    }
}

int jobs(char *command)
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
    if(word_count>1)
    {
        error_code=1;
        fprintf(stderr, "jobs: Invalid number of arguments\n");
        return -1;
    }
    struct bg_prc *a = malloc(sizeof(struct bg_prc));
    a = initial;
    // printf("list_size: %d\n", a->size);
    if (a->size == 0)
    {
        error_code=0;
        // printf("haha\n");
        return 0;
    }
    else
    {
        int i = 1;
        while (1)
        {
            printf("[%d] ", i);
            char not_used_yet;
            if((not_used_yet= getstatus(a->pid))=='T')
            {
                printf("Stopped ");
            }
            else if(not_used_yet != '1')
            {
                printf("Running ");   
            }
            for (int j = 0; j < a->w_c; j++)
            {
                printf("%s ", a->whole[j]);
            }
            printf("[%d]\n", a->pid);
            i++;
            if (a->next == NULL)
            {
                break;
            }
            else
            {
                a = a->next;
            }
        }
    }
    error_code=0;
    return 1;
}