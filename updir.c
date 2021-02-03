#include "updir.h"
#include "headers.h"

int update_dir(char* dir, char* fdir)
{
    char new_dir[PATH_MAX];
    getcwd(new_dir, PATH_MAX);
    // printf("dir: %d, new_dir: %d\n", strlen(dir), strlen(new_dir));
    // printf("this is cwd: %s\n", new_dir);
    if(strlen(dir)>strlen(new_dir))
    {
        getcwd(fdir,PATH_MAX);
      //  printf("result2: %s\n", dir);
        return -1;
    }
    else if(strlen(dir)==strlen(new_dir))
    {
        fdir[0]='~';
        fdir[1]='\0';
     //   printf("result1: %s\n", dir);
        return 0;
    }
    else
    {
        fdir[0]='~';
        fdir[strlen(new_dir)-strlen(dir)+1]='\0';
        for(int i=0;i<strlen(new_dir)-strlen(dir);i++)
        {
            fdir[i+1]=new_dir[strlen(dir)+i];
        }
     //   printf("result: %s\n", dir);
        return 1;   
    }
}