// ppm.c
// Řešení IJC-DU1, příklad a), 12.3.2019
// Autor: Tomáš Oplatek, FIT
// Přeloženo: gcc 7.3
// Stará se o načítání souboru a uvolňování paměti poté, co už soubor není potřeba
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "error.h"
#include "ppm.h"

struct ppm * ppm_read(const char * filename){
    int height, width;
    FILE *file  = fopen(filename, "r"); // read only
    if(file == NULL){
        warning_msg("File %s not found",filename);
        return NULL;
    }
    char buffer[1024] = {0};
    //read and ignore first line
    if(fgets(buffer, 1023, file) == NULL){
        fclose(file);
        error_exit("File is not in valid format");
    }
    //read 2nd
    if(fgets(buffer, 1023, file) == NULL){
        fclose(file);
        error_exit("File is not in valid format");
    }
    char *part = strtok(buffer, " ");
    width = atoi(part);
    part = strtok(NULL, " ");
    height = atoi(part);
    struct ppm *img = (struct ppm*)malloc(sizeof(struct ppm)*width*height*3);

    if(img->ysize > 8000 || img->xsize > 8000){
	fclose(file);
        warning_msg("Image is too big");
        return NULL;
    }
    //read and skip 3rd
    if (fgets(buffer, 1023, file) == NULL){
        fclose(file);
        error_exit("File is not in valid format");
    } //read 4th
    if(!fread(img->data,1,width*height*3,file)){
        fclose(file);
        error_exit("File is not in valid format");
    }
    img->xsize = width;
    img->ysize = height;
    fclose(file);
    return img;
}
void ppm_free(struct ppm *p){
    if(p != NULL){
        free(p);
    }
}

