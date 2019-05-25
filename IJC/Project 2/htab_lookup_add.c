// htab_lookup_add.c
// Řešení IJC-DU2, příklad a), 20.4.2019
// Autor: Tomáš Oplatek, FIT
// Přeloženo: gcc 8.3
// Přidá záznam do tabulky, pokuď zázam již existuje nic moc zásadního se nestane.

#include "htab_struct.h"

//Gets rid of some duplicate code.
struct htab_item *createNewItem(const char *key)
{
    struct htab_item *item = (struct htab_item *)malloc(sizeof(struct htab_item));
    //+1 to acconut for 0 char
    item->key = (char *)malloc((strlen(key) + 1) * sizeof(char));
    strcpy(item->key, key);
    item->value = 0;
    item->next = NULL;
    return item;
}

htab_iterator_t htab_lookup_add(htab_t *t, const char *key)
{
    htab_iterator_t iterator;
    int index = htab_hash_function(key) % t->arr_size;
    iterator.idx = index;
    //Nothing has been initialized in the table -> create the first element
    if (t->ptr[index] == NULL)
    {
        struct htab_item *item = createNewItem(key);
        t->size++;
        t->ptr[index] = item;
        iterator.ptr = t->ptr[index];
    }
    else
    {
        struct htab_item *itemPtr = t->ptr[index];
        //It will brake on something -> either item will be NULL or something else.
        while (1)
        {
            if (itemPtr->next == NULL && strcmp(itemPtr->key, key) != 0)
            { //next item is NULL and current key is not the requested key -> create the next item
                struct htab_item *item = createNewItem(key);
                itemPtr->next = item;
                iterator.ptr = item;
                //By adding a new item the table size is increased
                t->size++;
                break;
            }
            else if (strcmp(itemPtr->key, key) == 0)
            { //There is an item with the same key -> add to the counter
                iterator.ptr = itemPtr;
                break;
            }
            else
            {
                //go to the next item.
                itemPtr = itemPtr->next;
            }
        }
    }
    iterator.t = t;
    return iterator;
}