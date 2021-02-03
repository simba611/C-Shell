#include "updir.h"
#include "headers.h"
#include "cd.h"
#include "funcpwd.h"

int cd(char *token_copy, char *home, int prev_set, char *prev_dir)
{
    int count = 1;
    char *com_copy;
    char token[1024];
    int flag = 0;
    int flag1 = 0;
    strcpy(token, token_copy);
    for (int i = 0; i < strlen(token); i++)
    {
        if ((token[i] == ' ' || token[i] == '\t') && flag == 0)
        {
            flag1 = 1;
            continue;
        }
        flag = 1;
        if (i > 0 && (token[i] != ' ' && token[i] != '\t') && (token[i - 1] == ' ' || token[i - 1] == '\t'))
        {
            count++;
        }
    }
    if (flag1 == 1)
    {
        count--;
    }
    if (count == 1)
    {
        error_code = 0;
        chdir(home);
        return 0;
    }
    else if (count == 2)
    {
        char com[1024];
        int j = 0;
        int f = 0, f1 = 0, first = 0;
        for (int i = 0; i < strlen(token); i++)
        {
            if ((token[i] == ' ' || token[i] == '\t') && f == 0)
            {
                f1 = 1;
                continue;
            }
            f = 1;
            if (f1 == 0)
            {
                if (i > 0 && (token[i] != ' ' && token[i] != '\t') && (token[i - 1] == ' ' || token[i - 1] == '\t'))
                {
                    int k = i;
                    while ((token[k] != ' ' && token[k] != '\t') && k < strlen(token))
                    {
                        com[j] = token[k];
                        j++;
                        k++;
                    }
                }
            }
            else
            {
                if (i > 0 && (token[i] != ' ' && token[i] != '\t') && (token[i - 1] == ' ' || token[i - 1] == '\t'))
                {
                    if (first == 0)
                    {
                        first = 1;
                    }
                    else
                    {
                        int k = i;
                        while ((token[k] != ' ' && token[k] != '\t') && k < strlen(token))
                        {
                            com[j] = token[k];
                            j++;
                            k++;
                        }
                    }
                }
            }
        }
        com[j] = '\0';
        // printf("com: %s\n", com);
        char rel_path[1024];
        if (com[0] == '~')
        {
            for (int i = 0; i < strlen(home); i++)
            {
                rel_path[i] = home[i];
                // printf("%c", rel_path[i]);
            }
            // printf("*");
            for (int i = 1; i < strlen(com) + 1; i++)
            {
                rel_path[strlen(home) + i - 1] = com[i];
            }
            strcpy(com, rel_path);
        }
        // printf("path: %s\n", com);
        struct stat st;
        int exist = stat(com, &st);
        // if(strcmp(com,"~")==0)
        // {
        //     chdir(home);
        //     return 1;
        // }
        if (strcmp(com, "-") == 0)
        {
            if (prev_set == 0)
            {
                funcpwd();
                error_code = 1;
                fprintf(stderr, "No previous directory\n");
                return -1;
            }
            else
            {
                error_code = 0;
                printf("%s\n", prev_dir);
                chdir(prev_dir);
                return 1;
            }
        }
        if (exist != 0)
        {
            error_code = 1;
            fprintf(stderr, "Directory/file doesn't exist\n");
            return -1;
        }
        else
        {
            if (!S_ISDIR(st.st_mode))
            {
                error_code = 1;
                fprintf(stderr, "Not a directory\n");
                return -1;
            }
            else
            {
                if (!(st.st_mode & S_IXUSR))
                {
                    error_code = 1;
                    fprintf(stderr, "Permission Denied\n");
                    return -1;
                }
                else
                {
                    error_code = 0;
                    chdir(com);
                    return 1;
                }
            }
        }
    }
    else if (count > 2)
    {
        error_code = 1;
        fprintf(stderr, "Too many arguments\n");
        return -1;
    }
}