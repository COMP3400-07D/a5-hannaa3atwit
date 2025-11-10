#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    // Declare the buffer to use. Do NOT change!
    enum { BUFSIZE = 8 };
    char buffer[BUFSIZE];

    if (argc != 2) {
        // No file provided (or too many) → usage + exit 1
        printf("USAGE: parta FILEIN\n");
        return 1;
    }

    const char *path = argv[1];
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        // File open error → exact message + exit 2
        printf("ERROR: %s not found\n", path);
        return 2;
    }

    // Counters
    long upper = 0;
    long lower = 0;
    long number = 0;
    long space = 0;   // counts all isspace() (spaces, newlines, tabs, etc.)
    long other = 0;

    // Read loop
    for (;;) {
        ssize_t n = read(fd, buffer, BUFSIZE);
        if (n == 0) break;          // EOF
        if (n < 0) {                // Unexpected read error (not in tests)
            close(fd);
            printf("ERROR: %s not found\n", path); // keep same exit contract
            return 2;
        }
        for (ssize_t i = 0; i < n; i++) {
            unsigned char ch = (unsigned char)buffer[i];
            if (isupper(ch))      upper++;
            else if (islower(ch)) lower++;
            else if (isdigit(ch)) number++;
            else if (isspace(ch)) space++;
            else                  other++;
        }
    }

    close(fd);

    // output format 
    printf("Upper,%ld\n",  upper);
    printf("Lower,%ld\n",  lower);
    printf("Number,%ld\n", number);
    printf("Space,%ld\n",  space);
    printf("Other,%ld\n",  other);

    return 0;
}
