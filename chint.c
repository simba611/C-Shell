#include "headers.h"


int chint(char * a)
{
    int l=strlen(a);
    for(int i=0;i<strlen(a);i++)
    {
        if(a[i]>='0' && a[i]<='9')
        {}
        else
        {
            return -1;   
        }
    }
    return 1;
}