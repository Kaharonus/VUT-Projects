// ppm.h
// Řešení IJC-DU1, příklad a), 12.3.2019
// Autor: Tomáš Oplatek, FIT
// Přeloženo: gcc 7.3
struct ppm {
    unsigned xsize;
    unsigned ysize;
    char data[];    // RGB bajty, celkem 3*xsize*ysize
};
struct ppm * ppm_read(const char * filename);
void ppm_free(struct ppm *p);

