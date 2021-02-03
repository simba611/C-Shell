#include "updir.h"
#include "headers.h"

int funcpwd()
{
    char pwd[PATH_MAX];
    getcwd(pwd, PATH_MAX);
    printf("%s\n", pwd);
    error_code=0;
    return 1;
}