#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


// Linked List Struct
struct Node {
    char *data;
    struct Node *next;
};

// Create Linked List Nodes at the beginning
void createLLNodes(struct Node **head, FILE *fp) {
    char *buffer;
    size_t len;
    while (getline(&buffer, &len, fp) != -1) {
        struct Node *newNode = malloc(sizeof(struct Node));
        newNode->data = strdup(buffer);
        newNode->next = *head;
        *head = newNode;
    }
}

// Traverse Linked List and print data
void traverseLL(struct Node *head, FILE *fp) {
    struct Node *temp = head;
    while (temp != NULL) {
        if (temp->data) {
            fprintf(fp, "%s", temp->data);
        }
        temp = temp->next;
    }
}

int main(int argc, char *argv[]) {
    if (argc >= 4) { // Argument error handling
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }
    FILE *input_fp = NULL; // Input File pointer
    FILE *output_fp = NULL; // Output File pointer
    if (argc <= 1) { // If no input file is given, read from stdin
        input_fp = stdin;
    } else { // If input file is given, read from file
        input_fp = fopen(argv[1], "r");
    }
    if (argc <= 2) { // If no output file is given, write to stdout
        output_fp = stdout;
    } else { // If output file is given, write to file
        output_fp = fopen(argv[2], "w+");
    }
    if (input_fp == NULL || output_fp == NULL) { // If can't open files error handling
        fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
        exit(1);
    }
    struct stat input_file_stat, output_file_stat; // Check if input and output files are the same
    fstat(fileno(input_fp), &input_file_stat);
    fstat(fileno(output_fp), &output_file_stat);
    if (input_file_stat.st_ino == output_file_stat.st_ino) {
        fprintf(stderr, "reverse: input and output file must differ\n");
        exit(1);
    }

    struct Node *head = NULL; // Create Linked List
    createLLNodes(&head, input_fp); // Create Linked List Nodes
    traverseLL(head, output_fp); // Traverse Linked List and print data

    // Close files
    fclose(input_fp);
    fclose(output_fp);
    return 0;
}
