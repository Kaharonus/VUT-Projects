// htab_end.c
// Řešení IJC-DU2, příklad a), 20.4.2019
// Autor: Tomáš Oplatek, FIT
// Přeloženo: gcc 8.3
// Definuje tělo funkce, která vrací iterátor s posledním prvkem v tabulce

#include "htab_struct.h"

htab_iterator_t htab_end(const htab_t * t){
    struct htab_iterator iterator;
    iterator.ptr = NULL;
    iterator.idx = 0;
    iterator.t = t;
    for(int i = t->arr_size-1; i >=0; i--){
        if(t->ptr[i] == NULL){
            continue;
        }
        iterator.idx = i;
        struct htab_item* item = t->ptr[i];
        while(item->next != NULL){
            item = item->next;
        }
        iterator.ptr = item;

        return iterator;
    }
    return iterator;
}