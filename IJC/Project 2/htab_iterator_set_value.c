// htab_iterator_set_value.c
// Řešení IJC-DU2, příklad a), 20.4.2019
// Autor: Tomáš Oplatek, FIT
// Přeloženo: gcc 8.3
// nastaví hodnotu záznamu v tabulce. Vrací buď hodnotu nebo -1 pokuď hodnota nebyla nastavena

#include "htab_struct.h"

int htab_iterator_set_value(htab_iterator_t it, int val){
    if(it.ptr == NULL){
        return -1;
    }
    it.ptr->value = val;
    return val;
}