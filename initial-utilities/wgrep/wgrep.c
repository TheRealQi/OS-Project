#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc == 1) { // Fewer than 2 arguments error handling
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    } else if (argc == 2) { // If no file is given, read from stdin
        char *buffer; // Buffer to store line
        size_t len = 0; // Length of line (will be dynamically changed by getline)
        while (getline(&buffer, &len, stdin) != -1) { // Read line from stdin
            if (strstr(buffer, argv[1])) { // If line contains search term
                printf("%s", buffer); // Print line to stdout
            }
        }
        free(buffer); // Free buffer's allocated memory
    } else {
        for (int i = 2; i < argc; i++) {
            FILE *fp; // File pointer
            fp = fopen(argv[i], "r");
            if (fp == NULL) { // If file cannot be opened error handling
                printf("wgrep: cannot open file\n");
                exit(1);
            }
            char *buffer; // Buffer to store line
            size_t len = 0; // Length of line (will be dynamically changed by getline)
            while (getline(&buffer, &len, fp) != -1) {
                if (strstr(buffer, argv[1])) {
                    printf("%s", buffer);
                }
            }
            fclose(fp); // Close file
        }
    }
    return 0;
}
