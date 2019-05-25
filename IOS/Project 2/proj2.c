#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <semaphore.h>
#include <time.h>
#include <stdarg.h>
#include <sys/mman.h>


typedef struct args{
    int p, h, s, r, w, c;
    int error;
    char errorText[512];
} args_t;

//Returns new structure with error set to true and error msg set
struct args parseError(char msg[512]){
    struct args args;
    args.error = 1;
    strcpy(args.errorText, msg);
    return args;
}

struct args parseArgs(int count, char *argList[]){
    if(count != 7){
        return parseError("Invalid number of args");
    }
    struct args args;
    char* end;
    args.p = strtol(argList[1], &end, 0);
    if(*end != 0){
        return parseError("1st arg is not a valid number");
    }
    if(!(args.p >= 2 && args.p % 2 == 0)){
        return parseError("1st arg is not in valid range");
    }

    args.h = strtol(argList[2], &end, 0);
    if(*end != 0){
        return parseError("2nd arg is not a valid number");
    }
    if(!(args.h >= 0 && args.h <= 2000)){
        return parseError("2nd arg is not in valid range");
    }

    args.s = strtol(argList[3], &end, 0);
    if(*end != 0){
        return parseError("3rd arg is not a valid number");
    }
    if(!(args.s >= 0 && args.s <= 2000)){
        return parseError("3rd arg is not in valid range");
    }

    args.r = strtol(argList[4], &end, 0);
    if(*end != 0){
        return parseError("4th arg is not a valid number");
    }
    if(!(args.r >= 0 && args.r <= 2000)){
        return parseError("4th arg is not in valid range");
    }

    args.w = strtol(argList[5], &end, 0);
    if(*end != 0){
        return parseError("5th arg is not a valid number");
    }
    if(!(args.w >= 20 && args.w <= 2000)){
        return parseError("5th arg is not in valid range");
    }

    args.c = strtol(argList[6], &end, 0);
    if(*end != 0){
        return parseError("Last arg is not a valid number");
    }
    if(args.c < 5){
        return parseError("Last arg is not in valid range");
    }
    args.error = 0;
    return args;
}

typedef struct pier{
    //Current count of people on pier
    int count;
    //Pier capacity -> max amount of people
    int capacity;
    //number of hackers on pier
    int hackerCount;
    //number of serfs on pier
    int serfCount;
    //Determines if captain is already selected for the next voyage
    bool captSelected;
    //Used to determine if vars above can be written or read from
    sem_t canWrite;
    //Opens when there is a free space for a serf on the boat
    sem_t canSerfEnter;
    //Same but for hackers
    sem_t canHackerEnter;
    //Does some extra sync -> imposes fake limits on pier for better sync
    sem_t canPrint;
    //Determines if people can leave the boat
    sem_t canLeave;
    //Sem is posted by people when they leave which is waited for by the captain
    sem_t canCaptLeave;
    //
    sem_t sailing;
    //Introduces new "fake" pier with capacity for 5 people max (simplifies some logic)
    sem_t hasCaptain;
    //Used as a counter in the proj2.out file
    int printCounter;
} pier_t;


pier_t *initPier(struct args args){
    pier_t *pier = mmap(NULL, sizeof(pier), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    pier->capacity = args.c;
    pier->count = pier->hackerCount = pier->serfCount  = pier->printCounter = 0;
    sem_init(&pier->canWrite, 1, 1);
    sem_init(&pier->canSerfEnter,1, 0);
    sem_init(&pier->canHackerEnter,1, 0);
    sem_init(&pier->canLeave,1, 1);
    sem_init(&pier->canCaptLeave,1, 1);
    sem_init(&pier->canPrint,1, 1);
    sem_init(&pier->sailing,1, 1);
    sem_init(&pier->hasCaptain,1, 5);
    pier->captSelected = false;
    return pier;
}


bool isPierFull(pier_t *pier){
    bool result = false;
    sem_wait(&pier->canWrite);
    if(pier->count >= pier->capacity){
       result = true;
    }
    sem_post(&pier->canWrite);
    return result;
}

void addPerson(pier_t *pier, bool isSerf){
    sem_wait(&pier->canWrite);
    if(isSerf){
        pier->serfCount++;
    }else{
        pier->hackerCount++;
    }
    pier->count++;
    sem_post(&pier->canWrite);
}

//Determines if there is enough people on pier to create a valid group for voyage
bool containsValidCombination(pier_t *pier){
    bool result = false;
    sem_wait(&pier->canWrite);
    if(pier->serfCount >=2 && pier->hackerCount >=2){
        result = true;
    }else if(pier->serfCount >= 4 || pier->hackerCount >= 4){
        result = true;
    }
    sem_post(&pier->canWrite);

    return result;
}

void printLine(FILE* file,pier_t *pier, bool printCounts, const char *fmt,...){
    sem_wait(&pier->canPrint);
    pier->printCounter++;
    va_list valist;
    va_start(valist, fmt);
    fprintf(file, "%d\t: ", pier->printCounter);
    //Buffer needs to be flushed and \n is not enough.
    fflush(file);
    vfprintf(file,fmt, valist);
    fflush(file);
    if(printCounts){
        sem_wait(&pier->canWrite);
        fprintf(file,"\t: %d\t: %d",pier->hackerCount, pier->serfCount);
        fflush(file);
        sem_post(&pier->canWrite);

    }
    fprintf(file, "\n");
    fflush(file);
    va_end(valist);
    sem_post(&pier->canPrint);
}

//Creates spaces on ship for people to enter
void createFreeSpace(pier_t *pier,int serfs, int hacks){
    //printLine(pier,false,"Opening up some space for %d serfs and %d hacks\n",serfs, hacks);
    int count = 0;
    while(count < serfs){
        sem_post(&pier->canSerfEnter);
        count++;
    }
    count = 0;
    while(count < hacks){
        sem_post(&pier->canHackerEnter);
        count++;
    }
}

bool isCaptSelected(pier_t *pier){
    bool result;
    sem_wait(&pier->canWrite);
    result = pier->captSelected;
    sem_post(&pier->canWrite);
    return result;
}


//Takes care of captain functions
void handleCaptain(FILE *file,pier_t *pier, args_t args, int count,bool isSerf, char name[4]) {
    //Seed new random -> prob. not necessary
    srand(time(NULL) ^ getpid());
    int len = rand() % args.w;
    sem_wait(&pier->canWrite);
    pier->captSelected = true;
    //Add people to the ship and remove them from the pier
    if(isSerf && pier->serfCount >= 4){
        createFreeSpace(pier, 3,0);
        pier->serfCount -=4;
    }else if(!isSerf && pier->hackerCount >=4){
        createFreeSpace(pier, 0,3);
        pier->hackerCount -=4;
    }else if(isSerf){
        createFreeSpace(pier,1,2);
        pier->serfCount -=2;
        pier->hackerCount -=2;
    }else if(!isSerf){
        createFreeSpace(pier,2,1);
        pier->serfCount -=2;
        pier->hackerCount -=2;

    }
    pier->count -= 4;
    sem_post(&pier->canWrite);
    printLine(file, pier,true, "%s %d\t: boards", name, count + 1);
    //Voyage
    usleep(len);
    //Tell all people that they can leave
    sem_post(&pier->canLeave);
    sem_post(&pier->canLeave);
    sem_post(&pier->canLeave);
    //Wait for all people to confirm that they left
    sem_wait(&pier->canCaptLeave);
    sem_wait(&pier->canCaptLeave);
    sem_wait(&pier->canCaptLeave);


    sem_post(&pier->canWrite);
    pier->captSelected = false;
    sem_post(&pier->canWrite);
    //Lets just say the captain is taking some time to say bye to the ship.
    //usleep(3);
    printLine(file, pier,true, "%s %d\t: captain exits", name, count + 1);
}

void handlePassanger(FILE *file,pier_t *pier, int count, char name[4]){
    //All passanger does is wait to get a signal from a captain that he can leave after which he tails the captain that hes out
    sem_wait(&pier->canLeave);
    printLine(file, pier,true, "%s %d\t: member exits ", name, count + 1);
    sem_post(&pier->canCaptLeave);
}


void createPerson(FILE *file,pier_t *pier, args_t args, int count ,bool isSerf, char name[4]){
    srand(time(NULL) ^ getpid());
    printLine(file, pier,false, "%s %d\t: starts", name, count + 1);
    while(isPierFull(pier)){
        printLine(file, pier,true, "%s %d\t: leaves queue", name, count + 1);
        usleep(rand() % args.r);
        printLine(file, pier,false, "%s %d\t: is back", name, count + 1);

    }

    addPerson(pier, isSerf);
    printLine(file, pier,true, "%s %d\t: waits ", name, count + 1);
    //check if the "fake" pier is full or not
    sem_wait(&pier->hasCaptain);
    //
    if(containsValidCombination(pier) && !isCaptSelected(pier)){
        sem_wait(&pier->canWrite);
        pier->captSelected = true;
        sem_post(&pier->canWrite);
        sem_wait(&pier->sailing);
        handleCaptain(file, pier, args, count,isSerf, name);
        sem_post(&pier->sailing);

    }else{
        //Wait for cap to give signal that normal person can enter
        sem_wait(isSerf ? &pier->canSerfEnter : &pier->canHackerEnter);
        handlePassanger(file, pier, count, name);
    }
    // Empty this "fake" pier
    sem_post(&pier->hasCaptain);
    fclose(file);
    exit(0);

}

void startGenerator(FILE * file,pier_t *pier, args_t args, bool generateSerf){
    int stat;
    int pidArr[args.p];
    srand(time(NULL) ^ getpid());
    int count = 0;
    while (count < args.p){
        //Subprocess creation
        int pid = fork();
        //it is child
        if(pid == 0){
            createPerson(file,pier, args, count ,generateSerf, generateSerf ? "SERF" : "HACK");
        }else{ // parent -> save pid so we can wait for exit
            pidArr[count] = pid;
            count++;
        }
        if(args.s > 0){
            usleep(rand() % args.s);
        }
    }
    for(int i = 0; i < count; i++){
        //Wait for all child process' to end
        waitpid(pidArr[i],&stat,0);
        if(stat != 0){
            //Error handling
            printLine(file ,pier,false,"%s (PID: %d) Exited with unusual exit code: %d", generateSerf ? "SERF" : "HACK" ,pidArr[i], stat);
        }else{
        }
    }
    fclose(file);
    exit(0);
}


int main(int argc, char *argv[]) {
    args_t args = parseArgs(argc, argv);
    if(args.error){
        fprintf(stderr, "%s\n", args.errorText);
        return 1;
    }
    FILE* file = fopen("proj2.out", "w");
    if(file == NULL){
        fprintf(stderr, "File proj2.out could not be opened for write");
        return 1;
    }
    int stat;
    pier_t *pier = initPier(args);
    int serfPid = fork();
    if(serfPid == 0){ //Start generator for selfs
        startGenerator(file,pier, args, true);
    }else{ //start generator for hacks
        int hackerPid = fork();
        if(hackerPid == 0){
            startGenerator(file,pier, args, false);
        }else{ //Wait for both of them to finish
            waitpid(serfPid,&stat,0);
            waitpid(hackerPid,&stat,0);
        }
    }
    return 0;
}
