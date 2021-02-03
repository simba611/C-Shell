#include "headers.h"
#include "fg.h"
#include "other.h"
#include "signal.h"

int fg(char *command)
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
    if (word_count != 2)
    {
        error_code = 1;
        fprintf(stderr, "fg: Enter one job number\n");
        return -1;
    }
    else
    {
        if (chint(com[1]) == -1 || atoi(com[1]) > initial->size)
        {
            error_code = 1;
            fprintf(stderr, "fg: Invalid job number\n");
            return -1;
        }
        else
        {
            int job_num = atoi(com[1]);
            struct bg_prc *a = malloc(sizeof(struct bg_prc));
            a = initial;
            int trv = 1;
            while (trv < job_num)
            {
                a = a->next;
                trv++;
            }
            // *child_pid_global = a->pid;
            strcpy(current_na, a->name);
            current_pi = a->pid;
            current_word_count = a->w_c;
            for (int i = 0; i < a->w_c; i++)
            {
                strcpy(current_total_command[i], a->whole[i]);
            }
            int cld_pgid = a->pid;
            rem_bg(cld_pgid);
            int status;
            signal(SIGTTOU, SIG_IGN);
            signal(SIGTTIN, SIG_IGN);
            if (tcsetpgrp(STDIN_FILENO, cld_pgid) < 0)
            {
                fprintf(stderr, "tcsetpgrp: Error\n");
            }
            kill(cld_pgid, SIGCONT);
            waitpid(cld_pgid, &status, WUNTRACED);
            tcsetpgrp(STDIN_FILENO, getpgrp());
            signal(SIGTTOU, SIG_DFL);
            signal(SIGTTIN, SIG_DFL);
            int oo=status;
            if (WIFSTOPPED(status))
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

            return 1;
        }
    }
}