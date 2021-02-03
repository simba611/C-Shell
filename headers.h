#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "other.h"
#include <sys/select.h>
#include "chint.h"

struct bg_prc* initial;

int* child_pid_global;
char current_na[1024];
int current_pi;
int current_word_count;
char current_total_command[1024][1024]; 
char* quit_global;
int error_code;