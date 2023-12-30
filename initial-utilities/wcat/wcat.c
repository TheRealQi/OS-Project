#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        FILE *fp; // File pointer
        fp = fopen(argv[i], "r");
        if (fp == NULL) { // If file cannot be opened error handling
            printf("wcat: cannot open file\n");
            exit(1);
        }
        char *buffer; // Buffer to store each line
        size_t len = 0; // Length of each line (will be dynamically changed by getline)
        while (getline(&buffer, &len, fp) != -1) { // Read line from file
            printf("%s", buffer); // Print line to stdout
        }
        free(buffer); // Free buffer's allocated memory
        fclose(fp); // Close file
    }
    return 0;
}
