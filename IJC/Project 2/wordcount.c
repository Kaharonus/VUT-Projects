// wordcount.c
// Řešení IJC-DU2, příklad a), 20.4.2019
// Autor: Tomáš Oplatek, FIT
// Přeloženo: gcc 8.3
// Program využívající knihovnu htab.h pro počítání počtu slov zadaných do stdin

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "htab.h"
#include "io.h"
#define LINE 1024

int main(){
    //Used in clion to redirect stdin
    //freopen("lorem.txt","r",stdin);

    //Kdyby byla známa velikost souburu vybral bych prvočíslo které je přibližně 75% počtu záznamů.
    //Zbytečně velká velikost by znamenalo nevužitou paměť zatímco moc malá velikost by způsobila zpomalení programu
    //Velikost 1009 volím protože to je 1. prvočíslo nad 1000
    htab_t *tab = htab_init(1009);
    char word[LINE] = {0};
    int counter = 0;
    //get_word returns -1 on EOF -> read file until EOF is detected
    while ((counter = get_word(word, LINE - 1, stdin)) != -1){
        //Empty words are ignored
        if (counter == 0){
            continue;
        }
        struct htab_iterator iterator = htab_lookup_add(tab, word);
        //value++
        htab_iterator_set_value(iterator, htab_iterator_get_value(iterator) + 1);
    }
    struct htab_iterator item = htab_begin(tab);
    struct htab_iterator end = htab_end(tab);
    while (item.ptr != end.ptr){
        printf("%s\t%d\n", htab_iterator_get_key(item), htab_iterator_get_value(item));
        item = htab_iterator_next(item);
    }
    //the while above ends 1 item before end so the last item also needs to be printed out.
    if (end.ptr != NULL){
        printf("%s\t%d\n", htab_iterator_get_key(end), htab_iterator_get_value(end));
    }
    htab_free(tab);
    return 0;
}