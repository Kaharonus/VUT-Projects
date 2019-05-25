// htab_bucket_count.c
// Řešení IJC-DU2, příklad a), 20.4.2019
// Autor: Tomáš Oplatek, FIT
// Přeloženo: gcc 8.3
// Vrátí velikost hash tabulky

#include "htab_struct.h"

size_t htab_bucket_count(const htab_t * t){
    return t->arr_size;
}
