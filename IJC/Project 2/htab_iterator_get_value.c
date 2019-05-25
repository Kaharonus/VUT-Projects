//
// Created by tomas on 17.4.19.
//

#include "htab_struct.h"


int htab_iterator_get_value(htab_iterator_t it){
    if(it.ptr == NULL){
        return 0;
    }
    return it.ptr->value;
}