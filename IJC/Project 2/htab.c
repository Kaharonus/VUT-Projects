// htab.c
// Řešení IJC-DU2, příklad a), 20.4.2019
// Autor: Tomáš Oplatek, FIT
// Přeloženo: gcc 8.3
// Definuje funkce pro vytváření a uvolnění tabulky

#include "htab_struct.h"

htab_t *htab_init(size_t n){
    htab_t* tab = malloc(sizeof(htab_t) + n*sizeof(struct htab_item*));
    tab->arr_size = n;
    tab->size = 0;
    for(int i = 0; i < n; i++){
        tab->ptr[i] =  NULL;
    }
    return tab;
}

htab_t *htab_move(size_t n, htab_t *from){
    htab_t* tab = malloc(sizeof(htab_t) + n*sizeof(struct htab_item));
    struct htab_iterator item = htab_begin(from);
    struct htab_iterator end = htab_end(from);
    while(item.ptr != end.ptr){
        struct htab_iterator iter = htab_lookup_add(tab, item.ptr->key);
        htab_iterator_set_value(iter, htab_iterator_get_value(iter) + 1);

        item = htab_iterator_next(item);
    }
    htab_clear(from);
    return tab;
}

void htab_clear(htab_t * t){
    for(int i = 0; i < t->arr_size; i++){
        struct htab_item *item = t->ptr[i];
        struct htab_item *tmp;
        while(item != NULL){
            tmp = item;
            item = item->next;
            free(tmp->key);
            free(tmp);
        }
    }
    t->size = 0;
}

void htab_free(htab_t * t){
    htab_clear(t);
    free(t);
}

