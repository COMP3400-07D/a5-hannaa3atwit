#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("USAGE: partb FILEIN FILEOUT\n");
        return 1;
    }

    const char *file_in  = argv[1];
    const char *file_out = argv[2];

    // open/prepare output file with 0770 perms
    int fd = open(file_out, O_WRONLY | O_CREAT | O_TRUNC, 0770);
    if (fd == -1) {
        printf("ERROR: cannot open %s\n", file_out);
        return 2;
    }

    // Redirect stdout to the file
    if (dup2(fd, STDOUT_FILENO) == -1) {

        printf("ERROR: dup2 failed\n");
        close(fd);
        return 2;
    }
    close(fd); 

    // write header line and flush before exec
    printf("Category,Count\n");
    fflush(stdout);

    char *eargs[] = { "parta", (char *)file_in, NULL };
    int eret = execv("./parta", eargs);
    if (eret == -1) {
        dprintf(2, "exec failed\n");
        return 3;
    }

    return 0; 
}
