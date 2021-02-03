#include "headers.h"

long long int po10(int o)
{
    long long int h = 1;
    for (int i = 0; i < o; i++)
    {
        h = h * 10;
    }
    return h;
}

int pinfo(char *command)
{
    error_code=0;
    int pid;
    char token[50][PATH_MAX];
    int char_no = 0;
    int word_no = 0;
    for (int i = 0; i < strlen(command); i++)
    {
        if (command[i] != ' ' && command[i]!='\t' && command[i] != '\0')
        {
            while (command[i] != ' ' && command[i]!='\t' && command[i] != '\0')
            {
                token[word_no][char_no] = command[i];
                char_no++;
                i++;
            }
            token[word_no][char_no] = '\0';
            word_no++;
            char_no = 0;
        }
    }

    if (word_no > 2)
    {
        error_code=1;
        fprintf(stderr, "Too many arguments\n");
        for (int i = 0; i < word_no; i++)
        {
            memset(token[i], 0, strlen(token[i]));
        }
        return -1;
    }
    else if (word_no == 1)
    {
        pid = getpid();
    }
    else if (word_no == 2)
    {
        pid = atoi(token[1]);
    }
    char proc[1024];
    sprintf(proc, "/proc/%d/stat", pid);
    // printf("proc: %s\n", proc);
    FILE *fp = fopen(proc, "r");
    char info[1024];
    if (fp == NULL)
    {
        error_code=1;
        fprintf(stderr, "Process does not exist\n");
    }
    else
    {
        fscanf(fp, "%[^\n]s", info);
        fclose(fp);
        // printf("%s\n", info);
        // if(info[strlen(info)]=='\0')
        // {
        //     printf("works\n");
        // }
        char stat_stuff[100][4096];
        int arg_no = 0;
        int no_no = 0;
        int trav = 0;
        // printf("dfhghc\n");
        while (info[trav] != '\0')
        {
            // printf("sdf");
            if (info[trav] != ' ' && info[trav] != '\0')
            {
                while (info[trav] != ' ' && info[trav] != '\0')
                {
                    stat_stuff[arg_no][no_no] = info[trav];
                    trav++;
                    no_no++;
                }
                stat_stuff[arg_no][no_no] = '\0';
                arg_no++;
                no_no = 0;
            }
            trav++;
        }
        printf("pid: %d\n", pid);
        printf("Process Status: %s\n", stat_stuff[2]);
        printf("Memory: %s\n", stat_stuff[22]);
        char exe_path[1024];
        char exe_path_link[1024];
        sprintf(exe_path,"/proc/%d/exe", pid);
        int check_read=readlink(exe_path, exe_path_link, 1024);
        if(check_read<0)
        {
            fprintf(stderr, "Executable path does not exist\n");
        }
        else
        {
            printf("Executable Path: %s\n", exe_path_link);   
        }
    }
    memset(info, 0, strlen(info));
    for (int i = 0; i < word_no; i++)
    {
        memset(token[i], 0, strlen(token[i]));
    }
}
