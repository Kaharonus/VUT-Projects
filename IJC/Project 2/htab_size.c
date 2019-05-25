// htab_size.c
// Řešení IJC-DU2, příklad a), 20.4.2019
// Autor: Tomáš Oplatek, FIT
// Přeloženo: gcc 8.3
// vrátí počet prvků v tabulce (ne velikost tabulky)

#include "htab_struct.h"

size_t htab_size(const htab_t * t){
    return t->size;
}
