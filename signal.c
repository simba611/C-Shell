#include "headers.h"
#include "signal.h"
#include "other.h"

void ctrl(int sig)
{
    // printf("child_pid_global: %d\n", *child_pid_global);
    // printf("outside\n");
    if(*child_pid_global!=-1)
    {
        // printf("inside\n");
        kill(*child_pid_global, sig);
        if(sig==2)
        {}
        else
        {
            error_code=1;
            add_bg(current_na, current_pi, current_word_count, current_total_command);
        }
        *child_pid_global=-1;
        return;
    }
    return;
}