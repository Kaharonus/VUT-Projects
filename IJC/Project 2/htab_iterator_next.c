// htab_iterator_next.c
// Řešení IJC-DU2, příklad a), 20.4.2019
// Autor: Tomáš Oplatek, FIT
// Přeloženo: gcc 8.3
// Vrátí další prvek v tabulce

#include "htab_struct.h"

htab_iterator_t htab_iterator_next(htab_iterator_t it){
    struct htab_iterator iterator;
    iterator.ptr = NULL;
    iterator.idx = 0;
    htab_t *table = (htab_t*)it.t;
    iterator.t = table;
    //current iterator is not pointing on anything -> return the last item in the table
    if(it.ptr == NULL){
        return htab_end(it.t);
    }
    //Next item is null -> this item is the last one in table index -> find item on some other index
    if(it.ptr->next == NULL){
        for(int i = it.idx+1; i < it.t->arr_size; i++){
            //following chain of responsibility design pattern
            if(table->ptr[i] == NULL){
                continue;
            }
            iterator.idx = i;
            iterator.ptr = table->ptr[i];
            return iterator;
        }
        return htab_end(it.t);
    }else{
        iterator.ptr = it.ptr->next;
        iterator.idx = it.idx;
    }
    return iterator;
}