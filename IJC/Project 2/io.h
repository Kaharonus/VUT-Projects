// io.h
// Řešení IJC-DU2, příklad a), 20.4.2019
// Autor: Tomáš Oplatek, FIT
// Přeloženo: gcc 8.3
// Header file pro get_word

#ifndef C_IO_H
#define C_IO_H

#include <stdio.h>
#include <ctype.h>

int get_word(char *s, int max, FILE *f);

#endif //C_IO_H
