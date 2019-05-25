// htab_begin.c
// Řešení IJC-DU2, příklad a), 20.4.2019
// Autor: Tomáš Oplatek, FIT
// Přeloženo: gcc 8.3
// Tělo funkce htab_begin, která vrací iterátor s prvním prvkem tabulky

#include "htab_struct.h"

htab_iterator_t htab_begin(const htab_t *t){
    struct htab_iterator iterator;
    iterator.ptr = NULL;
    iterator.idx = 0;
    iterator.t = t;
    for (int i = 0; i < t->arr_size; i++){
        if (t->ptr[i] != NULL){
            iterator.idx = i;
            iterator.ptr = t->ptr[i];
            return iterator;
        }
    }
    return iterator;
}