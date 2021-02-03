#include "headers.h"

int in_history(char *com, char *home)
{
    int no_of_lines = 0;
    char path[1024];
    sprintf(path, "%s/history_file", home);
    // printf("path: %s\n", path);
    char line[100000] = {'\0'};
    int fd = open(path, O_RDWR | O_CREAT, 0666);
    if(fd<0)
    {
        fprintf(stderr, "Failed to write to history\n");
        return -1;
    }
    lseek(fd, 0, SEEK_SET);
    // printf("fd : %d\n", fd);
    int read_check = read(fd, line, 100000);
    if(read_check<0)
    {
        fprintf(stderr, "Failed to write to history\n");
        return -1;
    }
    for (int i = 0; i < strlen(line); i++)
    {
        if (line[i] == '\n')
        {
            no_of_lines++;
        }
    }
    char copy[1024];
    sprintf(copy, "%s\n", com);
    if (no_of_lines < 20)
    {
        int wr_check = write(fd, copy, strlen(copy));
    }
    else
    {
        memset(line, 0, strlen(line));
        lseek(fd, 0, SEEK_SET);
        int r_check = read(fd, line, 100000);
        close(fd);
        // for (int i = 0; i < strlen(line); i++)
        // {
        //     printf("%c", line[i]);
        // }
        int fd1 = open(path, O_RDWR | O_TRUNC);
        char remove_line[100000];
        int char_check = 0;
        while (line[char_check] != '\n')
        {
            char_check++;
        }
        char_check++;
        int ii = 0;
        for (ii = char_check; ii <= strlen(line); ii++)
        {
            remove_line[ii - char_check] = line[ii];
        }
        strcat(remove_line, copy);
        // printf("copy: %s", copy);
        // printf("%s", remove_line);
        write(fd1, remove_line, strlen(remove_line));
        close(fd1);
    }
    return 0;
}

int history(char *command, char *home)
{
    char list[1024][1024];
    int word_count = 0;
    int char_count = 0;
    for (int i = 0; i < strlen(command); i++)
    {
        if (command[i] != ' ' && command[i]!='\t' && command[i] != '\0')
        {
            while (command[i] != ' ' && command[i]!='\t' && command[i] != '\0')
            {
                list[word_count][char_count] = command[i];
                i++;
                char_count++;
            }
            list[word_count][char_count] = '\0';
            char_count = 0;
            word_count++;
        }
    }
    char path[1024];
    sprintf(path, "%s/history_file", home);
    int skip_line = 0;
    if (word_count == 1)
    {
        int no_lines = 0;
        char my_history[1000000];
        FILE *fp = fopen(path, "r");
        int trav_line = 0;
        char print_line[1024];
        while (fgets(print_line, sizeof(print_line), fp))
        {
            no_lines++;
        }
        skip_line = no_lines - 10;
        if (skip_line < 0)
        {
            skip_line = 0;
        }
        fclose(fp);
        fp = fopen(path, "r");
        while (fgets(print_line, sizeof(print_line), fp))
        {
            trav_line++;
            if (trav_line > skip_line)
            {
                printf("%s", print_line);
            }
        }
    }
    else if (word_count == 2)
    {
        if(chint(list[1])<0)
        {
            error_code=1;
            fprintf(stderr, "Invalid argument\n");
            return -1;
        }
        error_code=0;
        int num = atoi(list[1]);
        int no_lines = 0;
        char my_history[1000000];
        FILE *fp = fopen(path, "r");
        int trav_line = 0;
        char print_line[1024];
        while (fgets(print_line, sizeof(print_line), fp))
        {
            no_lines++;
        }
        skip_line = no_lines - num;
        if (skip_line < 0)
        {
            skip_line = 0;
        }
        fclose(fp);
        fp = fopen(path, "r");
        while (fgets(print_line, sizeof(print_line), fp))
        {
            trav_line++;
            if (trav_line > skip_line)
            {
                printf("%s", print_line);
            }
        }
    }
    else 
    {
        error_code=1;
        fprintf(stderr, "Invalid number of arguments\n");
        return -1;
    }
    return 0;
    // printf("%d\n", word_count);
}