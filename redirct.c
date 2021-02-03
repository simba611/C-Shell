#include "headers.h"
#include "redirect.h"

int redirect(char *command, char *in_file, char *out_file)
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
    in_file[0] = '\0';
    out_file[0] = '\0';
    int flag_in = 0;
    int flag_out = 0;
    int iii = 0;
    for (int i = 0; i < word_count; i++)
    {
        if (com[i][0] == '<')
        {
            com[i][0] = ' ';
            flag_in = 1;
            int iii = open(com[i + 1], O_RDONLY);
            if (i + 1 < word_count && com[i + 1] != "|")
            {
                strcpy(in_file, com[i + 1]);
                com[i + 1][0] = ' ';
                com[i + 1][1] = '\0';
            }
            else
            {
                fprintf(stderr, "Give input file\n");
                return -1;
            }
        }
        else if (com[i][0] == '>')
        {
            com[i][0] = ' ';
            if (strlen(com[i]) > 1)
            {
                if (iii > 0)
                {
                    int lala = open(com[i + 1], O_APPEND | O_CREAT | O_WRONLY, 0644);
                    close(lala);
                }
                com[i][1] = ' ';
                flag_out = 2;
            }
            else
            {
                if (iii > 0)
                {
                    int lala = open(com[i + 1], O_TRUNC | O_CREAT | O_WRONLY, 0644);
                    close(lala);
                }
                flag_out = 1;
            }
            if (i + 1 < word_count && com[i + 1] != "|")
            {
                strcpy(out_file, com[i + 1]);
                com[i + 1][0] = ' ';
                com[i + 1][1] = '\0';
            }
            else
            {
                fprintf(stderr, "Give output file\n");
                return -1;
            }
        }
    }
    memset(command, 0, strlen(command));
    for (int i = 0; i < word_count - 1; i++)
    {
        strcat(command, com[i]);
        strcat(command, " ");
    }
    strcat(command, com[word_count - 1]);
    if (flag_in == 0 && flag_out == 0)
    {
        return 0;
    }
    else if (flag_in == 0 && flag_out == 1)
    {
        return 1;
    }
    else if (flag_in == 0 && flag_out == 2)
    {
        return 2;
    }
    else if (flag_in == 1 && flag_out == 0)
    {
        return 3;
    }
    else if (flag_in == 1 && flag_out == 1)
    {
        return 4;
    }
    else if (flag_in == 1 && flag_out == 2)
    {
        return 5;
    }
}