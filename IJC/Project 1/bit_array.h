// bit_array.h
// Řešení IJC-DU1, příklad a), 12.3.2019
// Autor: Tomáš Oplatek, FIT
// Přeloženo: gcc 7.3
//definuje makra a inline funkce pro práci s pole bity
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"



typedef unsigned long * bit_array_t;


#ifdef USE_INLINE

inline void bit_array_alloc(bit_array_t jmeno_pole,velikost) {
    assert(velikost < 123000001);
    long _size = 1;
    while((sizeof(unsigned long) * _size * 8) < velikost) {
        _size++;
    }
    jmeno_pole = (bit_array_t)malloc(sizeof (unsigned long) * ++_size);
    for(unsigned long  i = 1; i < _size; i++){
        jmeno_pole[i] = 0;
    }

    jmeno_pole[0] = velikost;
}

inline void bit_array_free(bit_array_t jmeno_pole){
    free(jmeno_pole);
}

inline unsigned long bit_array_size(bit_array_t jmeno_pole) {
    return jmeno_pole[0];
}

inline unsigned long bit_array_getbit(bit_array_t jmeno_pole,unsigned long index){
    return (index >= jmeno_pole[0] ? error_exit("bit_array_getbit: Index %lu mimo rozsah 0..%lu",
            index, jmeno_pole[0]), 10 : ((jmeno_pole[(index / (sizeof(unsigned long)*8))+1] >> ((index % (sizeof(unsigned long)*8))) & 1UL))) ;
}


inline void bit_array_setbit(bit_array_t jmeno_pole, unsigned long index, int vyraz) {
    if(index >= jmeno_pole[0]){
       error_exit("bit_array_getbit: Index %lu mimo rozsah 0..%lu",(unsigned long)index, jmeno_pole[0]);
    }
    if(!vyraz){
            jmeno_pole[(index / (sizeof(unsigned long)*8))+1] &= ~(1UL << (index % (sizeof(unsigned long)*8)));
    }else{
            jmeno_pole[(index / (sizeof(unsigned long)*8))+1] |= 1UL << ((index % (sizeof(unsigned long)*8)));
    }
}

#else


#define bit_array_create(jmeno_pole,velikost) \
        assert(velikost < 123000001); \
        unsigned long waeipfohawl = ((velikost % sizeof(unsigned long) * 8) > 0 ? (velikost / (sizeof(unsigned long) * 8))+1 : (velikost / (sizeof(unsigned long) * 8)))+1; \
        unsigned long jmeno_pole[waeipfohawl]; \
        {for(unsigned long  i = 1; i < waeipfohawl; i++){ jmeno_pole[i] = 0; }}\
        jmeno_pole[0] = velikost;


#define bit_array_alloc(jmeno_pole,velikost) \
        assert(velikost < 123000001); \
        bit_array_t jmeno_pole; { \
            long _size = 1; \
            while((sizeof(unsigned long) * _size * 8) < velikost) { \
               _size++; \
            } \
            jmeno_pole = (bit_array_t)malloc(sizeof (unsigned long) * ++_size); \
            for(unsigned long  i = 1; i < _size; i++){ jmeno_pole[i] = 0; } \
        } \
        jmeno_pole[0] = velikost;

#define bit_array_free(jmeno_pole) free(jmeno_pole)

#define bit_array_size(jmeno_pole) jmeno_pole[0]

#define bit_array_getbit(jmeno_pole,index) \
    (index >= jmeno_pole[0] ? error_exit("bit_array_getbit: Index %lu mimo rozsah 0..%lu", \
    (unsigned long)index, jmeno_pole[0]), 10 : ((jmeno_pole[(index / (sizeof(unsigned long)*8))+1] >> ((index % (sizeof(unsigned long)*8))) & 1UL))) \

//number &= ~(1 << x);
#define bit_array_setbit(jmeno_pole,index,vyraz) {\
    if(index >= jmeno_pole[0]){\
       error_exit("bit_array_getbit: Index %lu mimo rozsah 0..%lu",(unsigned long)index, jmeno_pole[0]);\
    }\
    if(!vyraz){ \
            jmeno_pole[(index / (sizeof(unsigned long)*8))+1] &= ~(1UL << (index % (sizeof(unsigned long)*8)));\
    }else{ \
            jmeno_pole[(index / (sizeof(unsigned long)*8))+1] |= 1UL << ((index % (sizeof(unsigned long)*8))); \
    } \
}

#endif




