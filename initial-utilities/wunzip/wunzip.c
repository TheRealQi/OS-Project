#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc <= 1) { // Few arguments error handling
        printf("wunzip: file1 [file2 ...]\n");
        exit(1);
    }
    FILE *fp; // File pointer
    int counter; // Counter for the number of characters
    char ch; // Character
    for (int i = 1; i < argc; i++) {
        fp = fopen(argv[i], "r");
        if (fp == NULL) { // If file cannot be opened error handling
            printf("wzip: cannot open file\n");
            exit(1);
        }
        while (fread(&counter, sizeof(int), 1, fp)) { // Read the number of characters
            fread(&ch, sizeof(char), 1, fp); // Read the character
            for (int j = 0; j < counter; j++) { // Print the character number of counter times
                printf("%c", ch);
            }
        }
        fclose(fp); // Close the file
    }
    return 0;
}
