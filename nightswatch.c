#include "headers.h"

int delay(int sec)
{
    int mili = 1000 * sec;
    clock_t timestart = clock();
    while (clock() < timestart + mili)
    {
        fd_set fds;
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        int ret_val;
        FD_ZERO(&fds);
        FD_SET(0, &fds);
        ret_val = select(1, &fds, NULL, NULL, &tv);
        if (ret_val == -1)
        {
            perror("select()");
        }
        else if (ret_val)
        {
            ret_val = select(1, &fds, NULL, NULL, &tv);
            char c = fgetc(stdin);
            char d = fgetc(stdin);
            if (c == 'q')
                return -1;
        }
    }
    return 1;
}

int nightwatch(char *command)
{
    error_code=0;
    int newborn = 0, interrupt = 0;
    char com[1024][1024];
    int word_count = 0;
    int char_count = 0;
    for (int i = 0; i < strlen(command); i++)
    {
        if (command[i] != ' ' && command[i]!='\t' && command[i] != '\0')
        {
            while (command[i] != ' ' && command[i]!='\t' && command[i] != '\0')
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
    if (word_count != 4)
    {
        error_code=1;
        fprintf(stderr, "Incorrect command format\n");
        return -1;
    }
    else if (strcmp("-n", com[1]) != 0)
    {
        error_code=1;
        fprintf(stderr, "Incorrect flag\n");
        return -1;
    }
    else if (chint(com[2]) < 0)
    {
        error_code=1;
        fprintf(stderr, "Invalid number\n");
        return -1;
    }
    else if (strcmp("interrupt", com[3]) == 0)
    {
        interrupt = 1;
    }
    else if (strcmp("newborn", com[3]) == 0)
    {
        newborn = 1;
    }
    else
    {
        error_code=1;
        fprintf(stderr, "Incorrect command format\n");
        return -1;
    }
    int der_sec = atoi(com[2]);
    if (interrupt == 1)
    {
        int cpu_first = 0;
        while (1)
        {
            int cpu_no = 0;
            char watch[100000];
            FILE *fd;
            fd = fopen("/proc/interrupts", "r");
            if (fd == NULL)
            {
                error_code=1;
                fprintf(stderr, "Permission denied\n");
                return -1;
            }
            else
            {
                fscanf(fd, "%[^\n]s", watch);
            }
            for (int i = 0; i < strlen(watch); i++)
            {
                if (watch[i] != ' ' && watch[i] != '\0')
                {
                    cpu_no++;
                    while (watch[i] != ' ' && watch[i] != '\0')
                    {
                        i++;
                    }
                }
            }

            int flag = 0;
            int bflag = 0;
            char line[1024];
            while (fgets(line, sizeof(line), fd))
            {
                for (int i = 0; i < strlen(line); i++)
                {
                    if (line[i] != ' ')
                    {
                        if (line[i] == '1')
                        {
                            bflag = 1;
                            flag = 1;
                            break;
                        }
                    }
                }
                if (bflag == 1)
                {
                    break;
                }
            }
            if (flag == 0)
            {
                error_code=1;
                fprintf(stderr, "No keyboard found\n");
            }
            else
            {
                if (cpu_first == 0)
                {
                    printf("%s\n", watch);
                }
                int no_spaces = 2;
                int f_time = 0;
                for (int i = 0; i < strlen(line); i++)
                {
                    if (f_time == 0)
                    {
                        f_time = 1;
                        while (line[i] == ' ')
                        {
                            printf(" ");
                            i++;
                        }
                        while (line[i] != ' ')
                        {
                            printf(" ");
                            i++;
                        }
                    }

                    printf("%c", line[i]);
                    if (line[i] == ' ')
                    {
                        no_spaces++;
                        while (line[i] == ' ')
                        {
                            i++;
                            printf("%c", line[i]);
                        }
                    }
                    if (no_spaces > cpu_no + 1)
                    {
                        printf("\n");
                        break;
                    }
                }
            }
            fclose(fd);
            cpu_first = 1;
            int exit = delay(der_sec * 1000);
            if (exit == -1)
            {
                break;
            }
        }
    }
    else if (newborn == 1)
    {
        while (1)
        {
            char line[100];
            int w_c = 0;
            int c_c = 0;
            char storepid[10][100];
            FILE *handle = fopen("/proc/loadavg", "r");
            if(handle==NULL)
            {
                error_code=1;
                fprintf(stderr, "Could not open /proc/loadavg\n");
                return -1;
            }
            fscanf(handle, "%[^\n]s", line);
            for (int i = 0; i < strlen(line); i++)
            {
                if (line[i] != ' ' && line[i] != '\0')
                {
                    while (line[i] != ' ' && line[i] != '\0')
                    {
                        storepid[w_c][c_c] = line[i];
                        i++;
                        c_c++;
                    }
                    storepid[w_c][c_c] = '\0';
                    c_c = 0;
                    w_c++;
                }
            }
            printf("%s\n", storepid[4]);
            fclose(handle);
            int exit = delay(der_sec * 1000);
            if (exit == -1)
            {
                break;
            }
        }
    }
    return 0;
}