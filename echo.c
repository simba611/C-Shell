#include "headers.h"

int echo(char* str)
{
    char final[1024];
    int j=0;
    int f=0;
    int k=1024;
    for(int i=0;i<strlen(str);i++)
    {
        if((str[i]==' ' ||str[i]=='\t')&& f==0)
        {
            continue;
        }
        f=1;
        if(str[i]==' '||str[i]=='\t')
        {
            k=i;
            break;
        }
    }
    f=0;
    for(int i=k;i<strlen(str);i++)
    {
        if((str[i]==' ' ||str[i]=='\t')&& f==0)
        {
            continue;
        }
        f=1;
        if(str[i]!=' ' && str[i]!='\t')
        {
            final[j]=str[i];
            j++;
            if(str[i+1]=='\0')
            {
                break;
            }
            else if(str[i+1]==' ' || str[i+1]=='\t')
            {
                final[j]=' ';
                j++;
            }
        }
    }
    final[j]='\0';
    printf("%s\n", final);
    error_code=0;
    return 1;
}