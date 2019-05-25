// primes.c
// Řešení IJC-DU1, příklad a), 12.3.2019
// Autor: Tomáš Oplatek, FIT
// Přeloženo: gcc 7.3
// Zobrazí posledních 10 prvočísel do 123000000

#include <stdio.h>
#include "eratosthenes.h"
#include "bit_array.h"

int main(){
    bit_array_alloc(arr, 123000000);
    eratostehenes(arr);
    unsigned long results[10];
    int counter = 0;
    for(unsigned long i = (arr[0]-1); i > 0; i--) {
        if(counter == 10){
            break;
        }
        if(bit_array_getbit(arr, i) == 0){
            results[counter] = i;
            counter++;
        }
    }
    for(int i = 9; i >= 0; i--){
        printf("%ld\n",results[i]);
    }
    bit_array_free(arr);
    return 0;
}
