#include "headers.h"

void extract_first(char *a, char *b)
{
    int f = 0, f1 = 0, j = 0;
    for (int i = 0; i < strlen(b); i++)
    {
        if ((b[i] == ' '||b[i]=='\t') && f == 0)
        {
            f1 = 1;
            continue;
        }
        f = 1;
        if (b[i] != ' ' && b[i]!='\t')
        {
            a[j] = b[i];
            j++;
        }
        else
        {
            break;
        }
    }
    a[j]='\0';
}