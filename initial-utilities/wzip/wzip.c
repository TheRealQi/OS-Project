#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc <= 1) { // If few arguments are given error handling
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }
    FILE *fp, *temp; // File pointers
    temp = tmpfile(); // Create temporary file
    for (int i = 1; i < argc; i++) {
        fp = fopen(argv[i], "r");
        if (fp == NULL) { // If file cannot be opened error handling
            printf("wzip: cannot open file\n");
            exit(1);
        }
        char *buffer; // Buffer to store line
        size_t len = 0; // Length of line (will be dynamically changed by getline)
        while (getline(&buffer, &len, fp) != -1) {
            fprintf(temp, "%s", buffer);
        }
        free(buffer); // Free buffer's allocated memory
        fclose(fp); // CLose file
    }
    rewind(temp); // Set file pointer to beginning of file
    int counter = 1; // Counter for consecutive characters
    char ch = (char) fgetc(temp), next_ch; // Get first character in file
    while (ch != EOF) { // Loop through file
        next_ch = (char) fgetc(temp); // Get next character
        if (next_ch == ch) {
            counter++; // If next character is same as current character increment counter
        } else {
            fwrite(&counter, sizeof(int), 1, stdout); // Write counter to stdout
            fwrite(&ch, sizeof(char), 1, stdout); // Write character to stdout
            counter = 1;
        }
        ch = next_ch; // Set current character to next character
    }
    fclose(temp); // Close temporary file
    return 0;
}
