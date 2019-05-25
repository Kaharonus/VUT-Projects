// eratosthenes.c
// Řešení IJC-DU1, příklad a), 12.3.2019
// Autor: Tomáš Oplatek, FIT
// Přeloženo: gcc 7.3
#include <math.h>
#include "bit_array.h"
#include "error.h"

void eratostehenes(bit_array_t pole){
    bit_array_setbit(pole,0,1);
    bit_array_setbit(pole,1,1);
    for(unsigned long i = 2; i <= sqrt((long)(double)pole[0]); i++){
        if(bit_array_getbit(pole,i) == 1){
            continue;
        }
        for(unsigned long  j = 2*i; j < pole[0]; j += i){
            bit_array_setbit(pole,j,1);
        }
    }
}