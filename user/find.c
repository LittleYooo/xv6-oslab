#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"


void find(char *path, char *target) {
    char buf[512], *p;
    int fd;
    struct stat st;
    struct dirent de;

    if((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return ;
    }

    if(fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return ;
    }

    /* stop recursive if not dir */
    if(st.type != T_DIR) {
        close(fd);
        return ;
    }

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';

    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        if(de.inum == 0) continue;
        if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        
        if(strcmp(de.name, target) == 0) {
            printf("%s\n", buf);
        }

        find(buf, target);  // recursive
    }

    close(fd);
}


int main(int argc, char* argv[]) {
    if(argc != 3) {
        printf("Usage: find <path> <name>\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}