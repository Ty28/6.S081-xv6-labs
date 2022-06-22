#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RD 0
#define WR 1

const uint INT_LEN = sizeof(int);

void prime(int fd) 
{
    // get a number from left neighbour
    int a;
    read(fd, &a, INT_LEN);
    if(a == 0)
        exit(0);
    // print p
    fprintf(1, "prime %d\n", a);
    
    int num;
    int p[2];
    pipe(p);
    while(read(fd, &num, INT_LEN) != 0) 
    { 
        if(num % a != 0) 
            write(p[WR], &num, INT_LEN);
    }
    close(fd);
    close(p[WR]);
    if(fork() == 0)
        prime(p[RD]);
    else {
        close(p[RD]);
        wait(0);
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    int p[2];
    int n = 35;
    pipe(p);
    int pid = fork();
    for(int i = 2; i <= n; i++) 
        write(p[WR], &i, INT_LEN);
    if(pid == 0)    // child 
    {
        close(p[WR]);
        prime(p[RD]);
        close(p[RD]);
    }
    else
    {
        close(p[RD]);
        
        close(p[WR]);
        wait(0);
    }
    exit(0);
}



// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user/user.h"

// #define RD 0
// #define WR 1

// const uint INT_LEN = sizeof(int);

// void prime(int fd) 
// {
//     // get a number from left neighbour
//     int a;
//     read(fd, &a, INT_LEN);
//     fprintf(1, "prime %d\n", a);
//     int num;
//     int p[2];
//     int flag = 0;
//     while(read(fd, &num, INT_LEN) != 0) 
//     {
//         if(flag == 0) 
//         {
//             flag = 1;
//             pipe(p);
//             int pid = fork();
//             if(pid == 0)    // child
//             {
//                 close(p[WR]);
//                 prime(p[RD]);
//                 exit(0);
//             }
//             else            // parent
//                 close(p[RD]);
//         }   
//         if(num % a != 0) 
//             write(p[WR], &num, INT_LEN);
//     }
//     close(fd);
//     close(p[WR]);
//     wait(0);
// }

// int main(int argc, char *argv[])
// {
//     int p[2];
//     int n = 35;
//     pipe(p);
//     int pid = fork();
//     if(pid == 0)    // child 
//     {
//         close(p[WR]);
//         prime(p[RD]);
//         close(p[RD]);
//     }
//     else
//     {
//         close(p[RD]);
//         for(int i = 2; i <= n; i++) 
//             write(p[WR], &i, INT_LEN);
//         close(p[WR]);
//         wait(0);
//     }
//     exit(0);
// }