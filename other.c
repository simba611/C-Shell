#include "headers.h"

void add_bg(char *na, int pi, int w_count, char wo[1024][1024])
{
    if (initial->size == 0)
    {
        initial->size = 1;
        for (int i = 0; i <= strlen(na); i++)
        {
            initial->name[i] = na[i];
        }
        for (int i = 0; i < w_count; i++)
        {
            for (int j = 0; j <= strlen(wo[i]); j++)
            {
                initial->whole[i][j] = wo[i][j];
            }
        }
        initial->pid = pi;
        initial->w_c = w_count;
        // initial->active = 1;
    }
    else
    {
        struct bg_prc *a = malloc(sizeof(struct bg_prc));
        a = initial;
        initial->size++;
        while (a->next != NULL)
        {
            a = a->next;
        }
        if (a->next == NULL)
        {
            if (a->pid != pi)
            {
                a->next = malloc(sizeof(struct bg_prc));
                a = a->next;
                for (int i = 0; i <= strlen(na); i++)
                {
                    a->name[i] = na[i];
                }
                for (int i = 0; i < w_count; i++)
                {
                    for (int j = 0; j <= strlen(wo[i]); j++)
                    {
                        a->whole[i][j] = wo[i][j];
                    }
                }
                a->pid = pi;
                a->w_c = w_count;
                a->next = NULL;
                // a->active = 1;
            }
        }
    }
}

void rem_bg(int pi)
{
    struct bg_prc *a = malloc(sizeof(struct bg_prc));
    a = initial;
    a->size = a->size - 1;
    if (initial->size == 0)
    {
        return;
    }
    else if (initial->size > 0)
    {
        if (a->pid == pi)
        {
            a->next->size = a->size;
            initial = initial->next;
        }
        else
        {
            while (a->next->pid != pi)
            {
                a = a->next;
            }
            a->next = a->next->next;
        }
    }
}

char *find_bg(int pi)
{
    struct bg_prc *a = malloc(sizeof(struct bg_prc));
    a = initial;
    while (a->pid != pi)
    {
        a = a->next;
    }
    // a->active = 0;
    return a->name;
}

void func(int signum)
{
    // int pid_change;
    int stat;
    pid_t pid_change;
    while ((pid_change = waitpid((pid_t)(-1), &stat, WNOHANG)) > 0)
    {
        char *na = find_bg(pid_change);
        rem_bg(pid_change);
        int exit = WEXITSTATUS(stat);
        // rem_bg(pid_change);
        fprintf(stderr, "\n%s with pid %d exited with exit code %d\n", na, pid_change, exit);
    }
}

int other(char *command, int bg)
{
    error_code=0;
    char *list[1024];
    int word_count = 0;
    int char_count = 0;
    char word[1024][1024];
    for (int i = 0; i < strlen(command); i++)
    {
        if (command[i] != ' ' && command[i] != '\t' && command[i] != '\0')
        {
            while (command[i] != ' ' && command[i] != '\t' && command[i] != '\0')
            {
                word[word_count][char_count] = command[i];
                i++;
                char_count++;
            }
            word[word_count][char_count] = '\0';
            word_count++;
            char_count = 0;
            list[word_count - 1] = word[word_count - 1];
        }
    }
    list[word_count] = NULL;
    // for(int i=0;i<word_count;i++)
    // {
    //     printf("token: %s\n", list[i]);
    // }
    if (bg == 1)
    {
        error_code = 0;
        int pid = fork();
        if (pid < 0)
        {
            error_code = 1;
            fprintf(stderr, "Could not create new process\n");
            return -1;
        }
        else if (pid == 0)
        {
            setpgid(0, 0);
            if (execvp(list[0], list) < 0)
            {
                fprintf(stderr, "Could not run command.\n");
                _exit(1);
            }
        }
        else
        {
            setpgid(pid, 0);
            add_bg(list[0], pid, word_count, word);
            signal(SIGCHLD, func);
            return 0;
        }
    }
    else
    {
        int pid = fork();
        if (pid < 0)
        {
            error_code=1;
            fprintf(stderr, "Could not create new process.\n");
            return -1;
        }
        else if (pid == 0)
        {
            setpgid(0, 0);
            if (execvp(list[0], list) < 0)
            {
                error_code=1;
                fprintf(stderr, "Could not run command.\n");
                _exit(1);
            }
        }
        else
        {
            *child_pid_global=pid;
            strcpy(current_na, list[0]);
            current_pi = pid;
            current_word_count = word_count;
            for (int i = 0; i < word_count; i++)
            {
                strcpy(current_total_command[i], word[i]);
            }
            int stat;
            signal(SIGTTOU, SIG_IGN);
            signal(SIGTTIN, SIG_IGN);
            tcsetpgrp(STDIN_FILENO, pid);
            waitpid(pid, &stat, WUNTRACED);
            tcsetpgrp(STDIN_FILENO, getpgrp());
            signal(SIGTTOU, SIG_DFL);
            signal(SIGTTIN, SIG_DFL);
            int oo=stat;
            if (WIFSTOPPED(stat))
            {
                error_code = 1;
                add_bg(current_na, current_pi, current_word_count, current_total_command);
            }
            if(WIFEXITED(oo)!=1)
            {
                error_code=1;
            }
            if(WEXITSTATUS(oo)!=0)
            {
                error_code=1;
            }
            *child_pid_global=-1;
            return 0;
        }
    }
    for (int i = 0; i < word_count; i++)
    {
        memset(word[i], 0, strlen(word[i]));
    }
}