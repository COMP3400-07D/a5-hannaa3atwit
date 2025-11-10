#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("USAGE: partc FILEIN\n");
        return 1;
    }

    int pfd[2];
    if (pipe(pfd) == -1) {
        perror("pipe");
        return 2;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 2;
    }

    if (pid > 0) {
        close(pfd[0]);
        if (dup2(pfd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            return 2;
        }
        close(pfd[1]);

        char *eargs[] = { "parta", argv[1], NULL };
        execv("./parta", eargs);
        perror("execv parta");
        return 3;
    } else {
        close(pfd[1]);
        if (dup2(pfd[0], STDIN_FILENO) == -1) {
            perror("dup2");
            _exit(2);
        }
        close(pfd[0]);

        char *sargs[] = { "sort", "-t,", "-k2", "-n", NULL };
        execv("/usr/bin/sort", sargs);
        perror("execv sort");
        _exit(3);
    }
}
