#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define LINE 1024

int fileexists(const char * filename){
    /* try to open file to read */
    FILE *file = fopen(filename, "r");
    if (file != NULL){
        fclose(file);
        return 1;
    }
    return 0;
}

int parseNumber(char * str){
    int counter = 0;
    while(str[counter] != '\0'){
        if(!(str[counter] <= '9' && str[counter] >= '0')){
            return -1;
        }
        counter++;
    }
    return atoi(str);
}

char readLine(char * str, int max, FILE *file){
    int counter = 0;
    static int triggered = 0;
    char c = getc(file);
    while (c != '\n'){
        if(c == EOF){
            return EOF;
        }
        //when counter is greater or equal max don't save chars.
        if(counter < max){
            str[counter] = c;
        }else if(triggered++ == 0){
            fprintf(stderr, "ERR: Line was too long");
        }
        c = getc(file);
        counter++;
    }
    str[counter] = 0;
    return '\n';
}

void skipLines(char* fileName, int skipCount){
    FILE *file;
    if(fileName[0] == 0){
        file = stdin;
    }else {
        file = fopen(fileName, "r");
    }
    char buffer[LINE] = {0};
    for(int i = 0; i < skipCount; i++){
        fgets(buffer, LINE-1, file);
    }
    while(readLine(buffer, LINE-1, file) != EOF){
        printf("%s\n", buffer);
    }
    fclose(file);
}

void move(char** arr, int size){
    char buffer[LINE] = {0};
    for(int i = size - 1; i >=1 ; i--){
        strcpy(buffer, arr[i-1]);
        strcpy(arr[i], buffer);
    }
}

void tailLines(char* fileName, int readCount){
    //Allocate the memory requiered;
    char *lines[++readCount];
    for(int i = 0; i < readCount; i++){
        lines[i] = (char*)malloc(sizeof(char)*LINE);
        lines[i][0] = 0;
    }
    FILE *file;
    if(fileName[0] == 0){
        file = stdin;
    }else {
        file = fopen(fileName, "r");
    }
    while(readLine(lines[0], LINE-1, file) != EOF){
        move(lines, readCount);
    }
    for(int i = readCount-1; i > 0; i--){
        printf("%s\n",lines[i]);
    }
    for(int i = 0; i < readCount; i++){
        free(lines[i]);
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
    char fileName[LINE] = {0};
    int linesCount = 10;
    int skipLinesCount = -1;
    for(int i = 1; i < argc; i++){
        //if the files exists it means that the arg is a file
        if(fileexists(argv[i])) {
            strcpy(fileName, argv[i]);
            continue;
        }
        if(strcmp(argv[i], "-n") == 0){
            i++;
            if(!(i<argc)){
                break;
            }
            if(argv[i][0] == '+'){
                argv[i]++;
                skipLinesCount = parseNumber(argv[i]);
                continue;
            }else if(argv[i][0] == '-'){
                argv[i]++;
            }
            linesCount = parseNumber(argv[i]);
            if(linesCount <0){
                fprintf(stderr,"tail: invalid number of lines: ‘%s’",argv[i]);
                return 1;
            }
        }
    }
    if(skipLinesCount >= 0){
        skipLines(fileName, skipLinesCount);
    }else if(linesCount >= 0){
        tailLines(fileName, linesCount);
    }
    return 0;
}
