// steg-decode.c
// Řešení IJC-DU1, příklad a), 12.3.2019
// Autor: Tomáš Oplatek, FIT
// Přeloženo: gcc 7.3
// Najde a zobrazí skrytou zprávu


#include <stdio.h>
#include <time.h>
#include <tgmath.h>
#include "bit_array.h"
#include "eratosthenes.h"
#include "ppm.h"



int getMaxMsgLen(bit_array_t arr){
    int result = 0;
    for(int i = 0; i < arr[0]; i++){
        if(bit_array_getbit(arr,i) == 0){
            result++;
        }
    }
    return result;
}

int getChar(int *buffer){
    int res = 0;
    for(int i = 0; i < 8; i++){
        if(buffer[i] == 1){
            res |= 1 << i;
        }
    }
    return res;
}

void readMessage(struct ppm* img, bit_array_t arr){
    int buffer[8] = {0};
    int ended = 0;
    int index = 0;
    int len = getMaxMsgLen(arr);
    for(int i = 19;i < len; i++){
        if(bit_array_getbit(arr,i) == 1){
            continue;
        }
        buffer[index] = ((unsigned char)img->data[i] % 2);
        index++;
        if(index == 8){
            int res = getChar(buffer);
            if(res == 0){
		ended = 1;
                break;
            }
            printf("%c", res);
            index = 0;
        }
    }
    if(ended == 0){
        error_exit("File does not end with \0 character");
    }

}


int main(int argc, char const *argv[]) {
    if(argc != 2){
        error_exit("Wrong number of args (got %d expected 1)", --argc);
    }
    struct ppm* img = ppm_read(argv[1]);
    if(img == NULL){
        return 1;
    }
    long dimensions = img->xsize * img->ysize * 3;
    bit_array_alloc(bitArr, dimensions);

    eratostehenes(bitArr);
    //printf("%s",img->data);
    readMessage(img, bitArr);
    printf("\n");
    bit_array_free(bitArr);
    ppm_free(img);
    return 0;
}

