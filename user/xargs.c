#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

char* readline()
{
    char* buf = malloc(240);
    char* p = buf;
    while(read(0, p, 1))
    {
        if(*p == '\n' || *p == '\0')
        {
            *p = '\0';
            return buf;
        }
        p++;
    }
    if(p != buf)
        return buf;
    free(buf);
    exit(0);
}

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("%d ", argc);
        fprintf(2, "usage: xargs [command]\n");
        exit(1);
    }
    char* line;
    argv++;
    char* argv_new[20];
    char** p = argv;
    char** p_new = argv_new;
    while(*p != 0)
    {
        *p_new = *p;
        p++;
        p_new++;
    }
    while((line = readline()) != 0)
    {
        char* h = line;
        char* buf = malloc(36);
        char* buf_h = buf;
        int argc_new = argc - 1;
        while(*h)
        {
            if(*h == ' ' && buf_h != buf)
            {
                *buf_h = 0;
                argv_new[argc_new] = buf;
                // printf("%s\n", buf);
                buf = malloc(36);
                buf_h = buf;
                argc_new++;
            }
            else
            {
                *buf_h = *h;
                buf_h++;
                // printf("%s\n", buf);
            }
            h++;
        }
        if(buf != buf_h)
        {
            argv_new[argc_new] = buf;
            argc_new++;
        }
        argv_new[argc_new] = 0;
        free(line);
        if(fork() == 0)
            exec(argv_new[0], argv_new);
        else
            wait(0);
    }
    exit(0);
}