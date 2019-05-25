// error.c
// Řešení IJC-DU1, příklad a), 12.3.2019
// Autor: Tomáš Oplatek, FIT
// Přeloženo: gcc 7.3
//  zobrazuje chyby a warningy

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "error.h"

void warning_msg(const char *fmt, ...){
    va_list valist;
    va_start(valist, fmt);
    fprintf(stderr, "Chyba: ");
    vfprintf(stderr,fmt, valist);
    fprintf(stderr, "\n");
    va_end(valist);
}

void error_exit(const char *fmt, ...){
    va_list valist;
    va_start(valist, fmt);
    fprintf(stderr, "Chyba: ");
    vfprintf(stderr,fmt, valist);
    fprintf(stderr, "\n");
    va_end(valist);
    exit(1);
}