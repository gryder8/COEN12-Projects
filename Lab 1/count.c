#include <stdio.h> //I/O
#include <stdlib.h> //standard library
#include <stdbool.h> //bool

#define MAX_WORD_LENGTH 30 //maximum word length (30 chars)
#define FILE_NAME argv[1] //file name passed here

int main(int argc, char *argv[]) { //argc counts arugments, argv is the array of chars that has the arguments
    //the file name will be passed into the program in argv[1]

    if (argc != 2) { //args should be the first arg and the file path as the second arg
        printf("Expected 2 arguments, got %d! Program ended, pass only the file path and try again. \n", argc); //error
        return -1; //return
    }
    FILE *fileptr; //ptr to reference the file stream
    char c; //use to iterate along the file
    char prevChar; //hold the previous char (used at the end of the file)
    


    static int wordCount = 0; //init
    static int currentWordLength = 0; //init
    //char fileName[strlen(argv[1])];
    bool hasDelineatingChar = false; //init the flag to false

    //strcpy(fileName, argv[1]);

    fileptr = fopen(FILE_NAME, "r"); //open the file to be read (use the r argument)

    //check if the file exists
    if (fileptr == NULL) {
        printf("The file %s was not found!\n", FILE_NAME); //print error
        return -1; //return
    } else { //the file exists
        c = fgetc(fileptr); //set the character to the first char in the file;
        //fgetc will return EOF on error or if the end of the file is reached

        while (c != EOF) { //while the file has not ended
            //RUNTIME: O(n)
            if (hasDelineatingChar) { //find the next character that's not whitespace so words are only counted once
                while (c == ' ' || c == '\t' || c == '\n' || c == '\0') {  //runtime negligible compared to main while loop
                    c = fgetc(fileptr); //get the next char
                }
                hasDelineatingChar = false; //reset the flag
            }
            
            if (c == ' ' || c == '\t' || c == '\n' || c == '\0') { //check for whitespace chars (white space characters delineate a word)
                wordCount++; //count the word
                currentWordLength = 0; //reset the word count
                hasDelineatingChar = true; //set flag
            } else if (currentWordLength >= MAX_WORD_LENGTH) { //max length hit, increase word count and reset
                hasDelineatingChar = false; //set the flag
                wordCount++; //count as a word
                currentWordLength = 0; //reset
            } else {
                hasDelineatingChar = false; //set the flag
                currentWordLength++; //not a space, add to the length of the current word
            }
            prevChar = c; //save prevChar
            c = fgetc(fileptr); //next char
        }
        
        if (prevChar != ' ' && prevChar != '\t' && prevChar != '\n' && prevChar != '\0' && prevChar != -1) { //account for the file not ending with whitespace
            wordCount++; //add 1 to the word count
        }
            
        fclose(fileptr); //close the file stream
    }
    printf("%d total words\n", wordCount); //print the word count
    return wordCount; //return
}
