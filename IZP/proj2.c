#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

unsigned int absoluteValue(int n){
    return n > 0 ? n : n * -1;
}

void printDefault(){
    fprintf(stderr,"Default usage:\n--log for help with log functions.\n--pow for help with power of function\n");
}

//prints out application usage
void printUsage(char *param){
    if(strcmp(param, "--log") == 0){
        printf("log(X) = LOG_X\ncfrac_log(X) = CFRAC_LOG_X\ntaylor_log(X) = TAYLOR_LOG_X\n");
    }else if(strcmp(param, "--pow") == 0){
        printf("pow(X,Y) = POW\ntaylor_pow(X,Y) = TAYLOR_POW\ntaylorcf_pow(X,Y) = TAYLORCF_POW\n");
    }else{
        printDefault();
    }
}

//this method is called when taylor log has input value ABOVE 0.5
double taylorBigInput(double input, unsigned int iterations){
    double result = 0;
    //calculate base that will be iterated.
    double base = ((input - 1)/input);
    double basePow = base;
    for(unsigned int i = 1; i <= iterations; i++){
        result += basePow / i;
        basePow *= base;
    }
    return result;
}

//Used when taylor log has input UNDER 0.5
double taylorSmallInput(double input, unsigned int iterations){
    //x = y-1, y = 1-x and this function needs y;
    double y = 1-input;
    double yPow = y*y;
    //start at -y
    double result = y*-1;
    for(unsigned int i = 1; i < iterations; i++){
        result-= yPow/(i+1);
        //makes increases power of y
        yPow*=y;
    }
    return result;
}

double taylor_log(double x, unsigned int n){
    //Checks if input is in valid range...
    if(x < 0){
        return NAN;
    }else if(x == 0){
        return INFINITY*-1;
    }else{
        //decides which function to call - taylorBig - that means input is larger then .5, or taylorSmall - input would be smaller then .5
        return x > 0.5 ? taylorBigInput(x, n) : taylorSmallInput(x, n);
    }
}

double crafc_log(double x, unsigned int n){
    //checks input range
    if(x < 0){
        return NAN;
    }else if(x == 0){
        return -1*INFINITY;
    }
    //calculates z. x = (z+1)/(z-1)
    double z = (x-1) / (x+1);
    if(n==1){
        return 2*z;
    }else{
        double zPow = z*z;
        //lowest part of fraction - fractions lower then this are ignored.
        double base = (n+n-1) - ((n*n*zPow)/(n+n+1));
        //n-2 because 2z is 1 iteration and base is already 2nd iteration
        for(n -= 2; n > 0; n--){
            base = (n+n-1) - ((n*n*zPow)/base);
        }
        return  (z+z)/base;
    }
}

//Calls all log function and writes the result to the screen
void callLog(char *func, double x, int n){
    if(strcmp(func, "--log") == 0){
        printf("       log(%g) = %.12g\n", x, log(x));
        printf(" cfrac_log(%g) = %.12g\n", x, crafc_log(x, absoluteValue(n)));
        printf("taylor_log(%g) = %.12g\n", x, taylor_log(x, absoluteValue(n)));
    }else{
        printDefault();
    }
}

//Function that simplifies taylor_pow and taylorcf_pow by simply calling this function. Makes the code shorter
double pow_actual(double x, double y, unsigned int n, bool useTaylor){
    //save y and x for later usage
    double tempY = y;
    double tempX = x;
    x = fabs(x);
    //absolute value of y
    y = y > 0 ? y : y * -1;
    double result = 1;
    //decides if taylor_log or crafc_log should be used.
    double ln = useTaylor ? taylor_log(x,n) : crafc_log(x,n);
    double lnPow = ln;
    double yPow = y;
    double fact = 1;
    for(unsigned int i = 1; i < n; i++){
        if(isinf(result) || isnan(result)){
            fprintf(stderr, "There were too many iterations\n");
            exit(EXIT_FAILURE);
        }
        result += (yPow*lnPow)/fact;
        fact*= i+1;
        lnPow*=ln;
        yPow*=y;
    }
    if(tempX <0 && (int)y % 2 != 0){
        result*= -1;
    }
    //if y was > 0 return normal result else return 1/(x^y)
    return tempY > 0 ? result : 1 / result;
}

double taylor_pow(double x, double y, unsigned int n){
    return pow_actual(x, y, n, true);
}

double taylorcf_pow(double x, double y, unsigned int n){
    return pow_actual(x, y, n, false);
}

//Calls all pow functions and writes result to the screen
void callPow(char *func, double x, double y, int n){
    if(strcmp(func, "--pow") == 0){
        printf("         pow(%g,%g) = %.12g\n", x, y, pow(x,y));
        printf("  taylor_pow(%g,%g) = %.12g\n", x, y, taylor_pow(x,y,absoluteValue(n)));
        printf("taylorcf_pow(%g,%g) = %.12g\n", x, y, taylorcf_pow(x,y,absoluteValue(n)));
    }else{
        printDefault();
    }
}

int main(int argc, char *argv[]) {
    switch(argc){
        case 2:
            printUsage(argv[1]);
            break;
        case 4:
            callLog(argv[1], atof(argv[2]), atoi(argv[3]));
            break;
        case 5:
            callPow(argv[1], atof(argv[2]), atof(argv[3]), atoi(argv[4]));
            break;
        default:
            printDefault();
            break;
    }
    return 0;
}