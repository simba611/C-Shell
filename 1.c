#include "headers.h"
#include "updir.h"
#include "cd.h"
#include "funcpwd.h"
#include "extract1.h"
#include "echo.h"
#include "ls.h"
#include "pinfo.h"
#include "nightswatch.h"
#include "history.h"
#include "setenv.h"
#include "unset.h"
#include "jobs.h"
#include "kjob.h"
#include "fg.h"
#include "bg.h"
#include "overkill.h"
#include "redirect.h"
#include "signal.h"

int main()
{
    error_code=0;
    quit_global = malloc(1024 * sizeof(char));
    strcpy(quit_global, "quit");
    child_pid_global = (int *)malloc(sizeof(int));
    *child_pid_global = -1;
    signal(SIGINT, ctrl);
    signal(SIGTSTP, ctrl);
    initial = malloc(sizeof(struct bg_prc));
    initial->size = 0;
    initial->next = NULL;
    char home_dir[PATH_MAX];
    getcwd(home_dir, PATH_MAX);
    char prev_directory[PATH_MAX];
    int prev_directory_set = 0;
    int time = 0;
    int stdin_fd = dup(STDIN_FILENO);
    int stdout_fd = dup(STDOUT_FILENO);
    while (1)
    {
        char hostname[HOST_NAME_MAX];
        char username[LOGIN_NAME_MAX];
        char cwd[PATH_MAX];
        if (time == 0)
        {
            cwd[0] = '~';
            cwd[1] = '\0';
            time = 1;
        }
        gethostname(hostname, HOST_NAME_MAX);
        getlogin_r(username, LOGIN_NAME_MAX);
        if(error_code==0)
        {
            printf(":')<%s@%s:%s>", username, hostname, cwd);
        }
        else
        {
            printf(":'(<%s@%s:%s>", username, hostname, cwd);
        }
        char *command;
        command = malloc(1024 * sizeof(char));
        size_t bufsize = 1024;
        int quit_check = getline(&command, &bufsize, stdin);
        if (quit_check == -1)
        {
            printf("\n");
            return 0;
        }
        command[strlen(command) - 1] = '\0';
        char histoo[1096];
        strcpy(histoo, command);
        char final_histoo[1096];
        int how_many = 0;
        while (histoo[how_many] == ' ')
        {
            how_many++;
        }
        // printf("how: %d\n", how_many);
        for (int jj = how_many; jj < strlen(histoo); jj++)
        {
            final_histoo[jj - how_many] = histoo[jj];
        }
        // printf("hist: ****%s****", final_histoo);
        if (strcmp(final_histoo, "") != 0)
        {
            in_history(final_histoo, home_dir);
        }
        memset(histoo, 0, strlen(histoo));
        memset(final_histoo, 0, strlen(final_histoo));
        char *token;
        token = strtok(command, ";");
        while (token != NULL)
        {
            int no_pipe = 0;
            for (int i = 0; i < strlen(token); i++)
            {
                if (token[i] == '|')
                {
                    no_pipe++;
                }
            }
            char pipe_tok[4096];
            strcpy(pipe_tok, token);
            char *tok_pipe_array[1024];
            for (int i = 0; i < 1024; i++)
            {
                tok_pipe_array[i] = malloc(4096 * sizeof(char));
            }
            int word_count = 0;
            int char_count = 0;
            for (int i = 0; i < strlen(pipe_tok); i++)
            {
                if (pipe_tok[i] != '|' && pipe_tok[i] != '\0')
                {
                    while (pipe_tok[i] != '|' && command[i] != '\0')
                    {
                        tok_pipe_array[word_count][char_count] = pipe_tok[i];
                        i++;
                        char_count++;
                    }
                    tok_pipe_array[word_count][char_count] = '\0';
                    word_count++;
                    char_count = 0;
                }
            }

            for (int ii = 0; ii < word_count; ii++)
            {
                int pipe_wr[2];
                pipe(pipe_wr);
                if (ii != word_count - 1)
                {
                    dup2(pipe_wr[1], STDOUT_FILENO);
                    close(pipe_wr[1]);
                }
                else
                {
                    dup2(stdout_fd, STDOUT_FILENO);
                }
                char *in_file = malloc(1024 * sizeof(char));
                char *out_file = malloc(1024 * sizeof(char));
                int io_flag = redirect(tok_pipe_array[ii], in_file, out_file);
                int fd, fd1, input_not = 0;
                if (io_flag == 3 || io_flag == 4 || io_flag == 5)
                {
                    fd = open(in_file, O_RDONLY);
                    if (fd < 0)
                    {
                        fprintf(stderr, "Input file doesn't exist\n");
                        input_not = 1;
                    }
                    else
                    {
                        dup2(fd, STDIN_FILENO);
                        close(fd);
                    }
                }
                if (input_not == 0)
                {
                    if (io_flag == 1 || io_flag == 4)
                    {
                        fd1 = open(out_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
                        dup2(fd1, STDOUT_FILENO);
                        close(fd1);
                    }
                    else if (io_flag == 2 || io_flag == 5)
                    {
                        fd1 = open(out_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
                        dup2(fd1, STDOUT_FILENO);
                        close(fd1);
                    }
                    char com[1024];
                    char tok_command[1024];
                    char tok_command2[1024];
                    strcpy(tok_command, tok_pipe_array[ii]);
                    strcpy(tok_command2, tok_pipe_array[ii]);
                    for (int i = strlen(tok_command2) - 1; i >= 0; i--)
                    {
                        if (tok_command2[i] != ' ' && tok_command2[i] != '\t')
                        {
                            tok_command2[i + 1] = '\0';
                            break;
                        }
                    }
                    extract_first(com, tok_command2);
                    if (strcmp(com, "cd") == 0)
                    {
                        // in_history(tok_command2, home_dir);
                        char temp_prev[1024];
                        getcwd(temp_prev, 1024);
                        int cd_success = cd(tok_command, home_dir, prev_directory_set, prev_directory);
                        if (cd_success != -1)
                        {
                            strcpy(prev_directory, temp_prev);
                            prev_directory_set = 1;
                        }
                        int update_result = update_dir(home_dir, cwd);
                    }
                    else if (strcmp(com, "pwd") == 0)
                    {
                        // in_history(tok_command2, home_dir);
                        int check_pwd = funcpwd();
                    }
                    else if (strcmp(com, "echo") == 0)
                    {
                        // in_history(tok_command2, home_dir);
                        int echo_success = echo(tok_command2);
                    }
                    else if (strcmp(com, "ls") == 0)
                    {
                        // in_history(tok_command2, home_dir);
                        int check_ls = ls(tok_command2, home_dir);
                    }
                    else if (strcmp(com, "pinfo") == 0)
                    {
                        // in_history(tok_command2, home_dir);
                        int check_pinfo = pinfo(tok_command2);
                    }
                    else if (strcmp(com, "") == 0)
                    {
                    }
                    else if (strcmp(com, "nightswatch") == 0)
                    {
                        // in_history(tok_command2, home_dir);
                        int check_nights = nightwatch(tok_command2);
                    }
                    else if (strcmp(com, "history") == 0)
                    {
                        // in_history(tok_command2, home_dir);
                        int success_history = history(tok_command2, home_dir);
                    }
                    else if (strcmp(com, "setenv") == 0)
                    {
                        int set_suc = sen(tok_command2);
                    }
                    else if (strcmp(com, "unsetenv") == 0)
                    {
                        int unse_check = unsen(tok_command2);
                    }
                    else if (strcmp(com, "jobs") == 0)
                    {
                        int job_check = jobs(tok_command2);
                    }
                    else if (strcmp(com, "kjob") == 0)
                    {
                        int kjob_check = kjob(tok_command2);
                    }
                    else if (strcmp(com, "fg") == 0)
                    {
                        int fg_check = fg(tok_command2);
                    }
                    else if (strcmp(com, "bg") == 0)
                    {
                        int bg_check = bg(tok_command2);
                    }
                    else if (strcmp(com, "overkill") == 0)
                    {
                        int over_check = overkill(tok_command2);
                    }
                    else if (strcmp(com, "quit") == 0)
                    {
                        return 0;
                    }
                    else
                    {
                        // in_history(tok_command2, home_dir);
                        if (tok_command2[strlen(tok_command2) - 1] == '&')
                        {
                            tok_command2[strlen(tok_command2) - 1] = '\0';
                            other(tok_command2, 1);
                        }
                        else
                        {
                            other(tok_command2, 0);
                        }
                    }
                }
                dup2(pipe_wr[0], STDIN_FILENO);
                close(pipe_wr[0]);
                // printf("pipe_com: %s\n", tok_pipe_array[ii]);
            }
            dup2(stdout_fd, STDOUT_FILENO);
            dup2(stdin_fd, STDIN_FILENO);
            token = strtok(NULL, ";");
        }
    }
    return 0;
}