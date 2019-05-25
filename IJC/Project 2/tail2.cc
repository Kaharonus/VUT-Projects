#include <string>
#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

int parseNumber(char * str){
    int counter = 0;
    while(str[counter] != '\0'){
        if(!(str[counter] <= '9' && str[counter] >= '0')){
            return -1;
        }
        counter++;
    }
    return strtol(str, nullptr, 10);
}

void skipLines(istream & str, int skipCount){
    string line;
    for(int i = 0; i < skipCount-1; i++){
        getline(str, line);
    }
    while(getline(str,line)){
        cout << line << "\n";
    }
}

void tailLines(istream & str, int count){
    queue<string> q;
    string line;
    while(getline(str,line)){
        q.push(line);
        if(q.size() > (long unsigned int)count){
            q.pop();
        }
    }
    for(int i = 0; i < count; i++){
        cout << q.front() << "\n";
        q.pop();
    }
}



int main(int argc, char *argv[]) {
    //std::ios::sync_with_stdio(false);
    string fileName;
    int linesCount = 10;
    int skipLinesCount = -1;
    for(int i = 1; i < argc; i++){
        string n = "-n";
        if(argv[i] == n){
            i++;
            if(i>=argc){
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
            if(linesCount < 0){
                fprintf(stderr,"tail: invalid number of lines: ‘%s’",argv[i]);
                return 1;
            }
        }else{
            fileName.assign(argv[i]);
        }
    }
    ifstream file;
    file.open(fileName, ios::in);
    if(skipLinesCount >= 0){
        if(file.is_open()){
            skipLines(file, skipLinesCount);
        }else{
            skipLines(cin, skipLinesCount);
        }
    }else if(linesCount >= 0){
        if(file.is_open()){
            tailLines(file, linesCount);
        }else{
            tailLines(cin, linesCount);
        }
    }
    file.close();
    return 0;
}