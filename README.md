# OS-Project
This project was done as a partial requirement for the ECEN-427 course @Nile University.
### Team Members:
- Mohamed Osama Shosha
- SeifEldien Galal
- Mohab Hassan Hanfy
- Abdelrahman Yasser Zakaria
- Karim Haitham Salama
- Ali Mohamed Ahmed
## Tasks Progress:
- [x] Task 1 - Initial Utilities
    - [x] wcat
    - [x] wgrep
    - [x] wzip
    - [x] wunzip
- [x] Task 2 - Initial Reverse
- [ ] Task 3 - Process Shell (Unix Shell)
- [x] Task 4 - Initial xv6 (getreadcount System Call)
- [x] Task 5 - Scheduling Algorithm (Lottery)
    - [x] settickets System Call
    - [x] getpinfo System Call
    - [x] Lottery Scheduling Algorithm Implementation

## Task 1: Initial Utilities:
**In the first task we were asked to make 4 different C tools:**
1. wcat 
2. wgrep
3.	wzip
4.	wunzip

To accomplish these tools, we made use of some c functions that handles files operations:
 - **fopen/fclose:** These 2 functions are used to open/close files through a file pointer.  
 - **getline:** Similar to fgets, which is used to read an entire line from a file, every time its invoked it reads the next line. 
 -- **Note:** We opted to use getline instead of fgets to dynamically allocate the line buffer according to the length of each line in a file.
 - **getc:** Reads a character from a file, every time its invoked it reads the next character. 
 - **fwrite:** Used to write into a file.
 - **fread:** Used to read raw data from a file.
 - **free:** Used to free dynamically allocated memory, like for example the lines buffer. 
### wcat:
In this tool we were asked to make a simplified version of the cat command which is called wcat. Cat is a linux command that concatenates files together. However, in our version, wcat, we only need to read into the command line files specified by the user.

To accomplish this we used getline to read the lines of the given file/files and print it.
```
        char *buffer; // Buffer to store each line
        size_t len = 0; // Length of each line (will be dynamically changed by getline)
        while (getline(&buffer, &len, fp) != -1) { // Read line from file
            printf("%s", buffer); // Print line to stdout
        }
```
### wgrep:
In this tool we were asked to make something similar to the grep command which is called wgrep. This tool, wgrep, main function is to look for a keyword in the specified file/files given by the user and if it finds it the corresponding line will be printed.

To accomplish this we used the same logic we used in implementing the wcat tool, but instead of printing the contents of the file we used strstr function to check if the given keyword is a substring of the current line and if it is the corresponding line will be printed.
```
        char *buffer; // Buffer to store line
        size_t len = 0; // Length of line (will be dynamically changed by getline)
        while (getline(&buffer, &len, fp) != -1) {
            if (strstr(buffer, argv[1])) {
                printf("%s", buffer);
            }
        }
```

### wzip:
In this tool we were asked to make a file compression tool that uses a simple form of compression called run-length encoding which works by specifying the number of times a character bit has occurred next to each other. Wzip compresses the file/files into a specific binary format.
**Example:**
>aaaaaaaaaabbbb

**Is compressed into this:**
 >10a4b

To accomplish this the tool goes through several steps:
1. The tool starts by reading the given file/files and copy their contents into a temporary file. This temporary file is crucial since the user might give multiple files to compress, so we need to combine the contents of all the files into a single file.
```
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
```
2. After copying the contents of the given files into the temp file, we need to rewind the temp file pointer to point at the start of the file since its currently pointing at the end.
```
    rewind(temp); // Set file pointer to beginning of file
```
3. To compress the file/files we will have a character counter which counts the number of times a character has occurred next to each other. Afterwards, we will read through a file character by character if the current character (ch) is the same as the character after it (next_ch) the counter will be incremented by 1. Furthermore, if the current character (ch) is not the same as the character after it (next_ch) the counter and the current character (ch) will be written into the default output stream stdout  with sizes 4 bytes (int) and 1 byte (char) respectively. This will continue until we have reach the end of file (EOF).
```
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
``` 
### wunzip:
In this tool we were asked to make a file decompression tool that decompresses compressed files similar to those created by the wzip tool.

To accomplish this, the tool goes through the given binary compressed file character by character. Firstly, we will use fread to read 4 bytes (int) which is the counter of a character and then use it to read 1 byte (char) which is the character that the counter corresponds to. Secondly, we will print this character counter times.

```
        while (fread(&counter, sizeof(int), 1, fp)) { // Read the number of characters
            fread(&ch, sizeof(char), 1, fp); // Read the character
            for (int j = 0; j < counter; j++) { // Print the character number of counter times
                printf("%c", ch);
            }
```

### Tool arguments and file considerations:
1. **Number of arguments**: 
					1. **Wcat**: Accepts 1 or more arguments, should be the path of text files.
					2. **Wgrep**: Accepts  1 or 2 arguments, the keyword and/or the file name              (if the file name were not given the tool will instead read from stdin).
					3. **Wzip**: Accepts 1 or more arguments, the text files to be compressed.
					4. **Wunzip**: Accepts 1 or more arguments, the text files to be uncompressed.
-- **Note:** The number of arguments (argc) specified doesn't include the tool invocation name (ex: ./wcat). Additionally, if wrong number of arguments were given an error will be returned. 
2. **Invalid Files:** If either input/output files were given as arguments but for some reason they couldn't be opened an error will be returned.

## Task 2: Initial Reverse:
In the second task we were asked to make a simple tool called "reverse". Reverse is a simple tool that reads a text file and prints out its content but in reverse order.

To accomplish this, we had to use a dynamic data structure that stores the contents of the file in a LIFO fashion, thus we used a linked list. We implemented a linked list such that whenever we insert a new line (node) it will be inserted at the beginning, which will become our new head. Furthermore, when we traverse through the linked list it will start from the head of the list till the tail of the list.

*Using test case 6 as an example:*
File contents:
> hello
> this
> is
> a
> test

To store these lines in our linked list we are going to use "readline()" inside a while loop to iterate over all the lines.
```
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
```
```mermaid
graph LR
	test-->a-->is-->this-->hello-->null
```
To traverse our list and print into our designated file, we will iterate normally from the head till the tail which will return them in the following order:
> test
 a
 is 
 this
 hello
```
void traverseLL(struct Node *head, FILE *fp) {
    struct Node *temp = head;
    while (temp != NULL) {
        if (temp->data) {
            fprintf(fp, "%s", temp->data);
        }
        temp = temp->next;
    }
}
```
### Tool arguments and file considerations:
1. **Number of arguments**:  The tool only takes up to 2 arguments, if it takes more than 3 it will return an error. Moreover, if either input/output files were not given as an argument stdin/stdout will be used instead.
-- **Note:** The number of arguments (argc) specified doesn't include the tool invocation name (ex: ./reverse).
3. **Input and Output files must be different**: The input and output files can't be same, to check for this we compared the files serial numbers using st_ino from the stat library, if they are the same an error will be returned.
4. **Invalid Files:** If either input/output files were given as arguments but for some reason they couldn't be opened an error will be returned.
## Task 3: Process Shell
## Task 4: Initial xv6 (getreadcount System Call)

## Task 5: Lottery Scheduling Algorithm

