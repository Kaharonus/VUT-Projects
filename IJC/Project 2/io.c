// io.c
// Řešení IJC-DU2, příklad a), 20.4.2019
// Autor: Tomáš Oplatek, FIT
// Přeloženo: gcc 8.3
// Funkce pro čtení souboru slovo po slovu. Pokuď je slovo delší než maximum načte se pouze  část slova a zbytek je zahozen

#include "io.h"

int get_word(char *s, int max, FILE *f){
    int counter = 0;
    char c = getc(f);
    while (!isspace(c)){
        if(c == EOF){
            return -1;
        }
        //when counter is greater or equal max don't save chars.
        if(counter < max){
            s[counter] = c;
        }
        c = getc(f);
        counter++;
    }
    s[counter] = 0;
    return counter;
}