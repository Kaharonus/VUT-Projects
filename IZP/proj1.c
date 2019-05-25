#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_LINE_LENGTH 1024
#define MAX_NUMBER_OF_COMMANDS 100

//<editor-fold desc="Other functions">

//Converts char array to int
int charToInt(char * input) {
    int result = 0;
    int multiplier = 1;
    //Goes through char array backwards
    for (int i = strlen(input) - 1; i > 0; i--) {
        //Checks if character is number
        if (input[i] <= '9' && input[i] >= '0') {
            //character - 48 (ASCII table offset) * multiplier
            result += (input[i] - 48) * multiplier;
            multiplier *= 10;
        } else {
            fprintf(stderr, "Unexpected character %c in commands %s. Ignoring character", input[i], input);
        }
    }
    return result;
}

//Removes new line character from char array on both Windows and Linux
void removeNewLine(char * arr) {
    int lineLength = strlen(arr);
    if (arr[lineLength - 1] == '\n') {
        arr[lineLength - 1] = '\0';
        if (arr[lineLength - 2] == '\r') {
            arr[lineLength - 2] = '\0';
        }
    }
}

//Clears entire char array of length 1024 to "\0"
void cleanLine(char * cl) {
    for (int i = 0; i < MAX_LINE_LENGTH; i++) {
        cl[i] = '\0';
    }
}

//Reads file and fills 2D char array with commands, returns number of commands
int readCommands(char commands[MAX_NUMBER_OF_COMMANDS][MAX_LINE_LENGTH], char * file) {
    FILE * pFile = fopen(file, "r");
    //Checks if file is open
    if (pFile == NULL) {
        fprintf(stderr, "File %s couldn't be opened. Exiting now.\n", file);
        exit(EXIT_FAILURE);
    }
    //Reads lines one by one to commands 2d array.
    int index = 0;
    while (fgets(commands[index], MAX_LINE_LENGTH - 1, pFile)) {
        removeNewLine(commands[index]);
        index++;
    }
    fclose(pFile);
    //Returns the index which is also number of lines
    return index;
}

//Used when there are no more commands, but there is still input
void readRemaining() {
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH - 1, stdin)) {
        if (line[strlen(line) - 1] != '\n') {
            strcat(line, "\n");
        }
        printf("%s", line);
    }
}

//</editor-fold>

//<editor-fold desc="Commands">

//Function for command "n" or "nX"
void nextLineCmd(char * command) {
    if (command[1] != '\0') {
        char line[MAX_LINE_LENGTH] = "";
        //Reads number of lines in nX command.
        int numberOfLines = charToInt(command);
        //Reads and then writes specified amount of lines
        for (int i = 1; i < numberOfLines; i++) {
            if (fgets(line, MAX_LINE_LENGTH - 1, stdin)) {
                printf("%s", line);
            }
        }
    }
}

//Function for command aTEXT
void appendCmd(char * command, char * line) {
    strcat(line, command + 1);
}

//Function for command b/TEXT
void beforeCmd(char * command, char * line) {
    char tempLine[MAX_LINE_LENGTH] = "";
    strcpy(tempLine, line);
    cleanLine(line);
    strcat(line, command + 1);
    /*int index = 1;
    while(command[index] != '\0'){
        line[index-1] = command[index];
        index++;
    }*/
    strcat(line, tempLine);
}

//inserts line in front of current line
void insertCmd(char * command, char * line) {
    char tempLine[MAX_LINE_LENGTH] = "";
    strcpy(tempLine, line);
    cleanLine(line);
    strcat(line, command + 1);
    line[strlen(line)] = '\n';
    strcat(line, tempLine);
}

//Function for command s:pattern:replacement, return 1 when something has been found, returns 0 if it wasnt
bool replaceCmd(char * command, char * line) {
    char pattern[MAX_LINE_LENGTH], replacement[MAX_LINE_LENGTH];
    char separator = command[1];
    //Index = 2 becouse 0 is commands and 1 is separator
    int index = 2;
    //Pattern extraction from command, could be simplified using strtok
    while (command[index] != '\0' && command[index] != separator) {
        pattern[index - 2] = command[index];
        index++;
    }
    index++;
    //replacement extraction from command, could also be simplified using strtok
    int patternLength = strlen(pattern);
    while (command[index] != '\0') {
        replacement[index - patternLength - 3] = command[index];
        index++;
    }
    if (replacement[0] == '\0') {
        fprintf(stderr, "Replacement not found. Replacing with empty.\n");
    }
    //scanning for pattern index
    char * pch = strstr(line, pattern);
    //if pattern was found replace it
    if (pch != NULL) {
        //Saves remaining text (text after pattern) in a variable for later usage
        char remaining[MAX_LINE_LENGTH];
        strcpy(remaining, pch + patternLength);
        //Ends line before pattern
        pch[0] = '\0';
        //Concat line and replacement after that concat line and remaining (this is fine since stracat null terminates string)
        strcat(line, replacement);
        strcat(line, remaining);
        return true;
    }
    return false;
}

//Function for command S:pattern:replacement
void replaceAllCmd(char * command, char * line) {
    while (replaceCmd(command, line));
}

//Function for line deletion
void deleteCmd(char * command) {
    char line[MAX_LINE_LENGTH] = "";
    //Checks if command is d or dX
    if (command[1] != '\0') {
        //Reads number of lines in dX command.
        int numberOfLines = charToInt(command);
        //Reads lines
        for (int i = 1; i < numberOfLines; i++) {
            fgets(line, 1023, stdin);
        }
    }
}

//</editor-fold>

int main(int argc, char * argv[]) {
    if (argc == 2) {
        //TODO: Remove this, used in clion to redirect stdin
        freopen("text.txt", "r", stdin);
        char line[MAX_LINE_LENGTH] = "";
        int commandIndex = 0;
        char commands[MAX_NUMBER_OF_COMMANDS][MAX_LINE_LENGTH];
        int numberOfCommands = readCommands(commands, argv[1]);
        //Reads from stdin while it can
        while (fgets(line, MAX_LINE_LENGTH - 1, stdin)) {
            //If stdin isn't redirected end on new line
            bool readNext = false;
            //Removes new line character on both windows and linux - simplifies command calls.
            removeNewLine(line);
            //This would be set as false if command r (remove EOL) would be called and line will then be printed without new line character
            bool addNewLine = true;
            //Does commands on current line until command "n" or "nX" is called
            while (!readNext) {
                //Decides what command to case
                switch (commands[commandIndex][0]) {
                case 'q':
                    //printf("\nQuitting on line: %s\n",line);
                    exit(0);
                case '\0':
                    exit(0);
                case 'i':
                    insertCmd(commands[commandIndex], line);
                    break;
                case 'b':
                    beforeCmd(commands[commandIndex], line);
                    break;
                case 'a':
                    appendCmd(commands[commandIndex], line);
                    break;
                case 'd':
                    deleteCmd(commands[commandIndex]);
                    readNext = true;
                    break;
                case 'r':
                    //Yes, I know this is not ideal solution
                    addNewLine = false;
                    break;
                case 'n':
                    if (addNewLine) {
                        strcat(line, "\n");
                    }
                    printf("%s", line);
                    nextLineCmd(commands[commandIndex]);
                    readNext = true;
                    break;
                case 'g':
                    ;
                    int len = charToInt(commands[commandIndex]);
                    if (len < numberOfCommands) {
                        //printf("Possible infinite loop on command %s.\n",commands[commandIndex]);
                    }
                    if (len > 0 && len < numberOfCommands) {
                        commandIndex = len - 2;
                    } else {
                        printf("GoTo is out of bounds");
                    }
                    break;
                case 's':
                    replaceCmd(commands[commandIndex], line);
                    break;
                case 'S':
                    replaceAllCmd(commands[commandIndex], line);
                    break;
                case 'e':
                    strcat(line, "\n");
                    break;
                default:
                    fprintf(stderr, "Command %s not recognized.\n", commands[commandIndex]);
                    exit(EXIT_FAILURE);
                }
                commandIndex++;
                //If index is higher then number of commands it means that there are more lines then commands.
                if (commandIndex >= numberOfCommands) {
                    printf("%s\n", line);
                    readRemaining();
                }
            }
            //Just to be sure set every single character to \0
            cleanLine(line);
        }
    } else {
        //Help
        fprintf(stderr, "Wrong usage.\nUsage: ./editor commandFile.txt, to redirect stdin use <input\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}