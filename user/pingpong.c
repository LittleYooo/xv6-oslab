#include "kernel/types.h"
#include "user.h"

int read_pipe(int p[2]) {
    int x;
    
    close(p[1]);
    read(p[0], &x, sizeof(x));
    close(p[0]);
    return x;
}

void write_pipe(int p[2], int x) {
    close(p[0]);
    write(p[1], &x, sizeof(x));
    close(p[1]);
}


int main() {
    int c2f[2], f2c[2];
    pipe(c2f);
    pipe(f2c);

    if(fork() == 0) {
        int pid, father_pid;
        pid = getpid();

        father_pid = read_pipe(f2c);
        printf("%d: received ping from pid %d\n", pid, father_pid);

        write_pipe(c2f, pid);
    } else {
        int pid, child_pid;
        pid = getpid();

        write_pipe(f2c, pid);

        child_pid = read_pipe(c2f);
        printf("%d: received pong from pid %d\n", pid, child_pid);
    }
    exit(0);
}