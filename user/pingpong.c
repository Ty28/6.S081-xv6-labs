#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RD 0
#define WR 1

int main(int argc, char *argv[])
{
    int p2c[2];
    int c2p[2];
    char byte_ = 'a';
    pipe(p2c);
    pipe(c2p);
    int pid = fork();
    int status = 0;
    if (pid < 0) {
        fprintf(2, "fork() error!\n");
        close(c2p[RD]);
        close(c2p[WR]);
        close(p2c[RD]);
        close(p2c[WR]);
        exit(1);
    }
    else if(pid == 0) {  // child process
        // close write end of p2c and read end of c2p;
        close(p2c[WR]);
        close(c2p[RD]);
        if(read(p2c[RD], &byte_, sizeof(char)) != sizeof(char)) {
            fprintf(2, "child %d read error: out of 1 byte\n", getpid());
            status = 1;
        }
        else
            fprintf(1, "%d: received ping\n", getpid());
        if(write(c2p[WR], &byte_, sizeof(char)) != sizeof(char)) {
            fprintf(2, "child %d write error: out of 1 byte\n", getpid());
            status = 1;
        }
        
        // close read end of p2c and write end of c2p;
        close(p2c[RD]);
        close(c2p[WR]);

        exit(status);
    }
    else {          // parent process
        // close read end of p2c and write end of c2p;
        close(p2c[RD]);
        close(c2p[WR]);
        if(write(p2c[WR], &byte_, sizeof(char)) != sizeof(char)) {
            fprintf(2, "parent %d write error: out of 1 byte\n", getpid());
            status = 1;
        }
        if(read(c2p[RD], &byte_, sizeof(char)) != sizeof(char)) {
            fprintf(2, "parent %d read error: out of 1 byte\n", getpid());
            status = 1;
        }
        else
            fprintf(1, "%d: received pong\n", getpid());
        // close write end of p2c and read end of c2p;
        close(p2c[WR]);
        close(c2p[RD]);

        exit(status);
    }
    
}