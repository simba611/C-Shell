#ifndef __OTHER_H
#define __OTHER_H

int other(char* , int);

struct bg_prc{
    char name[30];
    char whole[1024][1024];
    int pid;
    struct bg_prc* next;
    int size;
    int w_c;
};

void add_bg(char *, int , int , char [1024][1024]);
void rem_bg(int);
void func(int);


#endif