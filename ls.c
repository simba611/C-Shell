#include "headers.h"
#include "updir.h"

//account for special files

int print_file_permissions(char *path, char *file_name)
{
    struct stat buffer;
    stat(path, &buffer);
    if (S_ISDIR(buffer.st_mode))
    {
        printf("d");
    }
    else
    {
        printf("-");
    }
    if (buffer.st_mode & S_IRUSR)
    {
        printf("r");
    }
    else
    {
        printf("-");
    }
    if (buffer.st_mode & S_IWUSR)
    {
        printf("w");
    }
    else
    {
        printf("-");
    }
    if (buffer.st_mode & S_IXUSR)
    {
        printf("x");
    }
    else
    {
        printf("-");
    }
    if (buffer.st_mode & S_IRGRP)
    {
        printf("r");
    }
    else
    {
        printf("-");
    }
    if (buffer.st_mode & S_IWGRP)
    {
        printf("w");
    }
    else
    {
        printf("-");
    }
    if (buffer.st_mode & S_IXGRP)
    {
        printf("x");
    }
    else
    {
        printf("-");
    }
    if (buffer.st_mode & S_IROTH)
    {
        printf("r");
    }
    else
    {
        printf("-");
    }
    if (buffer.st_mode & S_IWOTH)
    {
        printf("w");
    }
    else
    {
        printf("-");
    }
    if (buffer.st_mode & S_IXOTH)
    {
        printf("x");
    }
    else
    {
        printf("-");
    }
    printf("%*ld", 4, buffer.st_nlink);
    uid_t uid = buffer.st_uid;
    gid_t gid = buffer.st_gid;
    struct passwd *f;
    f = getpwuid(uid);
    // if(f!=NULL)
    printf("%*s", 15, f->pw_name);
    struct group *g;
    g = getgrgid(gid);
    printf("%*s", 15, g->gr_name);
    long int size = buffer.st_size;
    printf("%*ld", 8, size);
    time_t now;
    time(&now);
    struct tm *tm = localtime(&(buffer.st_ctime));
    int then_month = tm->tm_mon;
    int then_date = tm->tm_mday;
    char with_time[20];
    char with_year[20];
    strftime(with_year, 20, "%b %e  %Y", tm);
    strftime(with_time, 20, "%b %e %R", tm);
    struct tm *now_info = localtime(&now);
    //for files more than 6 months old, display year instead of time
    if (now_info->tm_mon - then_month > 6)
    {
        printf(" %s", with_year);
    }
    else if (now_info->tm_mon - then_month == 6)
    {
        if (now_info->tm_mday > then_date)
        {
            printf(" %s", with_year);
        }
        else
        {
            printf(" %s", with_time);
        }
    }
    else
    {
        printf(" %s", with_time);
    }
    // printf("\t%s", date);
    printf(" %s\n", file_name);
}

int ls(char *command, char *home)
{
    error_code=0;
    // printf("*%s*\n", command);
    int l_flag = 0, a_flag = 0, w_flag = 0;
    for (int i = 0; i < strlen(command); i++)
    {
        if (command[i] == '-')
        {
            if (command[i + 1] == 'l')
            {
                l_flag = 1;
                if (command[i + 2] == 'a')
                {
                    a_flag = 1;
                    if ((command[i + 3] != ' ' &&command[i+3]!='\t')&& command[i + 3] != '\0')
                    {
                        w_flag = 1;
                    }
                }
                else if ((command[i + 2] != ' '&&command[i+2]!='\t') && command[i + 2] != '\0')
                {
                    w_flag = 1;
                }
            }
            else if (command[i + 1] == 'a')
            {
                a_flag = 1;
                if (command[i + 2] == 'l')
                {
                    l_flag = 1;
                    if (command[i + 3] != ' ' &&command[i+3]!='\t' && command[i + 3] != '\0')
                    {
                        w_flag = 1;
                    }
                }
                else if (command[i + 2] != ' ' &&command[i+2]!='\t' && command[i + 2] != '\0')
                {
                    w_flag = 1;
                }
            }
            else
            {
                w_flag = 1;
            }
        }
    }
    // printf("l_flag: %d\na_flag: %d\nw_flag: %d\n", l_flag, a_flag,w_flag);
    char dir_list[1024][1024];
    char file_list[1024][1024];
    int file_count = 0;
    int word_count = 0;
    int n_word = 0;
    int n_char = 0;
    int req_count = 0;
    for (int i = 0; i < strlen(command); i++)
    {
        // printf("word_count: %d\n", word_count);
        if (command[i] != ' ' && command[i]!='\t')
        {
            if (i + 2 < strlen(command) + 1 && (command[i] == 'l' && command[i + 1] == 's' && (command[i + 2] == ' ' || command[i + 2] == '\0'|| command[i+2]=='\t')))
            {
                i = i + 2;
            }
            else if (command[i] == '-')
            {
                while (command[i] != ' ' && command[i]!='\t' && command[i] != '\0')
                {
                    i++;
                }
            }
            else
            {
                int k = i;
                if (command[i] == '~')
                {
                    for (int y = 0; y < strlen(home); y++)
                    {
                        dir_list[n_word][n_char] = home[y];
                        n_char++;
                    }
                    k++;
                }
                while (command[k] != ' ' && command[i]!='\t' && command[k] != '\0')
                {
                    dir_list[n_word][n_char] = command[k];
                    k++;
                    n_char++;
                }
                i = k;
                n_word++;
                n_char = 0;
                word_count++;
                req_count++;
                struct stat buffer;
                int exist = stat(dir_list[n_word - 1], &buffer);
                if (exist == -1)
                {
                    error_code=1;
                    fprintf(stderr, "ls: %s cannot be accessed: No such file/directory.\n", dir_list[n_word - 1]);
                    memset(dir_list[n_word - 1], 0, strlen(dir_list[n_word - 1]));
                    n_word--;
                    word_count--;
                }
                else
                {
                    if (S_ISDIR(buffer.st_mode))
                    {
                        int read_acc = access(dir_list[n_word - 1], R_OK);
                        if (read_acc == -1)
                        {
                            error_code=1;
                            fprintf(stderr, "ls: %s cannot be accessed: Read permission denied.\n", dir_list[n_word - 1]);
                            memset(dir_list[n_word - 1], 0, strlen(dir_list[n_word - 1]));
                            n_word--;
                            word_count--;
                        }
                    }
                    else
                    {
                        strcpy(file_list[file_count], dir_list[n_word - 1]);
                        file_count++;
                        memset(dir_list[n_word - 1], 0, strlen(dir_list[n_word - 1]));
                        n_word--;
                        word_count--;
                    }
                }
            }
        }
    }
    // for (int i = 0; i < word_count; i++)
    // {
    //     printf("word: %s\n", dir_list[i]);
    // }
    if (req_count == 0)
    {
        dir_list[0][0] = '.';
        dir_list[0][1] = '\0';
        word_count++;
    }
    // printf("word_count: %d\n", word_count);
    // for(int i=0;i<word_count;i++)
    // {
    //     printf("*%s*\n", dir_list[i]);
    // }
    if (w_flag == 1)
    {
        error_code=1;
        fprintf(stderr, "Wrong flag\n");
    }
    else
    {
        if (l_flag == 0 && a_flag == 0)
        {
            for (int i = 0; i < file_count; i++)
            {
                printf("%s  ", file_list[i]);
            }
            if (file_count > 0)
            {
                printf("\n");
            }
            if (word_count > 0 && file_count > 0)
            {
                printf("\n");
            }
            for (int i = 0; i < word_count; i++)
            {
                if (req_count >= 2)
                {
                    printf("%s:\n", dir_list[i]);
                }
                struct dirent **entry;
                int count = scandir(dir_list[i], &entry, NULL, alphasort);
                for (int j = 0; j < count; j++)
                {
                    if (entry[j]->d_name[0] != '.')
                    {
                        printf("%s  ", entry[j]->d_name);
                    }
                }
                printf("\n");
                if (req_count >= 2 && i != word_count - 1)
                {
                    printf("\n");
                }
            }
        }
        else if (l_flag == 0 && a_flag == 1)
        {
            for (int i = 0; i < file_count; i++)
            {
                printf("%s  ", file_list[i]);
            }
            if (file_count > 0)
            {
                printf("\n");
            }
            if (word_count > 0 && file_count > 0)
            {
                printf("\n");
            }
            for (int i = 0; i < word_count; i++)
            {
                if (req_count >= 2)
                {
                    printf("%s:\n", dir_list[i]);
                }
                struct dirent **entry;
                int count = scandir(dir_list[i], &entry, NULL, alphasort);
                for (int j = 0; j < count; j++)
                {
                    printf("%s  ", entry[j]->d_name);
                }
                printf("\n");
                if (req_count >= 2 && i != word_count - 1)
                {
                    printf("\n");
                }
            }
        }
        else if (l_flag == 1 && a_flag == 0)
        {
            for (int i = 0; i < file_count; i++)
            {
                print_file_permissions(file_list[i], file_list[i]);
            }
            if (word_count > 0 && file_count > 0)
            {
                printf("\n");
            }
            for (int i = 0; i < word_count; i++)
            {
                if (req_count >= 2)
                {
                    printf("%s:\n", dir_list[i]);
                }
                struct dirent **entry;
                int count = scandir(dir_list[i], &entry, NULL, alphasort);
                int total = 0;
                for (int j = 0; j < count; j++)
                {
                    if (entry[j]->d_name[0] != '.')
                    {
                        char file_path[1024];
                        sprintf(file_path, "%s/%s", dir_list[i], entry[j]->d_name);
                        struct stat s;
                        stat(file_path, &s);
                        total = total + s.st_blocks;
                    }
                }
                printf("total %d\n", total / 2);
                total = 0;
                for (int j = 0; j < count; j++)
                {
                    if (entry[j]->d_name[0] != '.')
                    {
                        char file_path[1024];
                        sprintf(file_path, "%s/%s", dir_list[i], entry[j]->d_name);
                        print_file_permissions(file_path, entry[j]->d_name);
                    }
                }
                if (req_count >= 2 && i != word_count - 1)
                {
                    printf("\n");
                }
            }
        }
        else if (l_flag == 1 && a_flag == 1)
        {
            for (int i = 0; i < file_count; i++)
            {
                print_file_permissions(file_list[i], file_list[i]);
            }
            if (word_count > 0 && file_count > 0)
            {
                printf("\n");
            }
            for (int i = 0; i < word_count; i++)
            {
                if (req_count >= 2)
                {
                    printf("%s:\n", dir_list[i]);
                }
                struct dirent **entry;
                int count = scandir(dir_list[i], &entry, NULL, alphasort);
                int total = 0;
                for (int j = 0; j < count; j++)
                {
                    char file_path[1024];
                    sprintf(file_path, "%s/%s", dir_list[i], entry[j]->d_name);
                    struct stat s;
                    stat(file_path, &s);
                    total = total + s.st_blocks;
                }
                printf("total %d\n", total / 2);
                total = 0;
                for (int j = 0; j < count; j++)
                {
                    char file_path[1024];
                    sprintf(file_path, "%s/%s", dir_list[i], entry[j]->d_name);
                    print_file_permissions(file_path, entry[j]->d_name);
                }
                if (req_count >= 2 && i != word_count - 1)
                {
                    printf("\n");
                }
            }
        }
    }
    for (int i = 0; i < word_count; i++)
    {
        memset(dir_list[i], 0, strlen(dir_list[i]));
    }
    for (int i = 0; i < file_count; i++)
    {
        memset(file_list[i], 0, strlen(file_list[i]));
    }
}
