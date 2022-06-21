#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// add a test branch
int main(int argc, char *argv[])
{
    int time = 0;
    if(argc != 2)   // parameters error
    {
        fprintf(2, "usage: sleep [time]\n");
        exit(1);
    }
    else
    {
        time = atoi(argv[1]);
        sleep(time);
    }
    exit(0);
}
