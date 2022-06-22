#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *name)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if(st.type != T_DIR)
    {
        fprintf(2, "usage: find [DIRECTORY] [name]\n");
        close(fd);
        return;
    }

    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
        fprintf(2, "find path too long\n");
        close(fd);
        return;
    }

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';     // p指向最后一个'/'之后
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);        // 添加路径名称
        p[DIRSIZ] = 0;                      // 字符串结束标志
        if(stat(buf, &st) < 0){
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        if(st.type == T_DIR && strcmp(p, ".") && strcmp(p, ".."))
            find(buf, name);
        else if(!strcmp(p, name))
            fprintf(1, "%s\n", buf);
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        fprintf(2, "usage: find [path] [name]\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}