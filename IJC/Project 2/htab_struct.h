// htab_struct.h
// Řešení IJC-DU2, příklad a), 20.4.2019
// Autor: Tomáš Oplatek, FIT
// Přeloženo: gcc 8.3
// Header file s definicemi struktur

#include "htab.h"

struct htab_item
{
    char *key;
    int value;
    struct htab_item *next;
};

struct htab
{
    unsigned int size;
    unsigned int arr_size;
    struct htab_item *ptr[];
};
