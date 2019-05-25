// htab_iterator_get_key.c
// Řešení IJC-DU2, příklad a), 20.4.2019
// Autor: Tomáš Oplatek, FIT
// Přeloženo: gcc 8.3
// Vrací klíč k iterátoru, nebo prázdný char pokuď iterátor na nic neukazuje

#include "htab_struct.h"

const char * htab_iterator_get_key(htab_iterator_t it){
    if(it.ptr == NULL){
        return "";
    }
    return it.ptr->key;
}