/**
 * Kostra programu pro 3. projekt IZP 2018/19
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap) {
    assert(c != NULL);
    assert(cap >= 0);
    c->size = 0;
    c->capacity = cap;
    c->obj = malloc(sizeof(struct obj_t)*cap);
    if(c->obj == NULL){
        c->capacity = 0;
    }else{
        //inicializace na defaultní hodnoty
        c->obj->x =  c->obj->y = c->obj->id -1;
    }
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c) {
    free(c->obj);
    c->capacity = 0;
    c->size = 0;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap) {
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj) {
    c->size++;
    if(c->size > c->capacity){
        c = resize_cluster(c, c->size);
    }
    c->obj[c->size-1] = obj;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2) {
    assert(c1 != NULL);
    assert(c2 != NULL);
    for(int i = 0; i < c2->size; i++){
        append_cluster(c1,c2->obj[i]);
    }
    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx) {
    assert(idx < narr);
    assert(narr > 0);
    //vyčištění clusteru k odebrání
    clear_cluster(&carr[idx]);
    //posune indexy ostatních prvků
    for(int i = idx; i < narr-1; i++){
        carr[i] = carr[i+1];
    }
    return narr-1;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2) {
    assert(o1 != NULL);
    assert(o2 != NULL);
    //root of (x1-x2)^2 + (y1-y2)^2
    return (float)sqrt(pow(o1->x-o2->x,2) + pow(o1->y - o2->y,2));
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2) {
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);
    //základ podle kterého se počítá zbetek
    float minimumLen = obj_distance(&c1->obj[0],&c2->obj[0]);
    for(int first = 0; first < c1->size; first++){
        for(int second = 0; second < c2->size; second++){
            float tempMin = obj_distance(&c1->obj[first],&c2->obj[second]);
            if(tempMin < minimumLen){
                minimumLen = tempMin;
            }
        }
    }
    return minimumLen;

}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2) {
    assert(narr > 0);
    //Pokuď je málo clusterů nic se nestane
    if(narr <= 1){
        *c1 = 0;
        *c2 = 0;
        return;
    }
    //Výpočet minima prvních dvou prvků, podle kterých se počítá zbytek
    float minimumLen = cluster_distance(&carr[0],&carr[1]);
    *c1 = 0;
    *c2 = 1;
    for(int x = 0; x < narr; x++){
        for(int y = 0; y < narr; y++){
            //ujištění se že se prvek neporovnává sám ye sebou
            if(x == y){
                break;
            }
            //výpočet minimální vydálenosti
            float tempLen = cluster_distance(&carr[x],&carr[y]);
            if(tempLen <= minimumLen){
                minimumLen = tempLen;
                //nastavení ukazatelů na hodnoty
                *c1 = y;
                *c2 = x;
            }
        }
    }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b) {
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c) {
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c) {
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 * Frees memory by size
 */
void free_all_clusters(struct cluster_t *cluster, int count){
    for(int i = 0; i < count; i++){
        clear_cluster(&cluster[i]);
    }
    free(cluster);
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr) {
    assert(arr != NULL);
    int numberOfLines = -1;
    FILE *file = fopen(filename,"r");
    //kontrola otevĹenĂ­ souboru
    if(file != NULL){
        //načtení prvního řádku.
        fscanf(file,"count=%d", &numberOfLines);
        if(numberOfLines<0){
            fprintf(stderr, "Input file is not valid. Number of clusters is <0");
            exit(EXIT_FAILURE);
        }
        struct cluster_t *clusters = malloc(sizeof(struct cluster_t)*numberOfLines);
        if(clusters == NULL){
            arr = NULL;
            return 0;
        }
        //postupnĂŠ naÄĂ­tĂĄnĂ­ do pole
        for(int i = 0; i < numberOfLines; i++){
            struct cluster_t cluster;
            //Vytvoření prázdného clusteru
            init_cluster(&cluster,1);
            if(cluster.obj != NULL){
                if(feof(file)){
                    fprintf(stderr, "File says more lines are supposed to be loaded then there in the file");
                    free_all_clusters(clusters,i);
                    exit(EXIT_FAILURE);
                }
                //načtení hodnot z řádku souboru do paměti
                fscanf(file,"%d %f %f", &cluster.obj->id, &cluster.obj->x, &cluster.obj->y);
                //kontrola jestli je input ve správném rozmezí
                if(!(cluster.obj->x > 0 &&cluster.obj->x < 1000 && cluster.obj->y > 0 && cluster.obj->y < 1000)){
                    fprintf(stderr,"Values for x and y in input file are not in a valid range\n");
                    free_all_clusters(clusters,i);
                    fclose(file);
                    exit(EXIT_FAILURE);
                }
                cluster.size = 1;
                clusters[i] = cluster;
            }
        }
        //předání pointeru
        *arr = clusters;
        fclose(file);
    }else{
        fprintf(stderr,"File %s could not be opened.\n", filename);
        exit(EXIT_FAILURE);
    }
    return numberOfLines;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr) {
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++) {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[]) {
    struct cluster_t *clusters;
    if(argc == 3){
        int requestedNumberOfCluster = atoi(argv[2]);
        //kontrola jestli je počet clusterů ve správném roymeyí
        requestedNumberOfCluster = requestedNumberOfCluster > 0 ? requestedNumberOfCluster : 1 ;
        //kontrola nestandartních vstupů
        if(requestedNumberOfCluster < 1){
            fprintf(stderr,"Number of requested clusters must be >= 1");
            return EXIT_FAILURE;
        }
        int currentNumberOfClusters = load_clusters(argv[1],&clusters);
        if(currentNumberOfClusters < requestedNumberOfCluster){
            free_all_clusters(clusters,currentNumberOfClusters);
            fprintf(stderr,"Number of requested clusters must be >= number of clusters from input file");
            return EXIT_FAILURE;
        }
        //Zmenušuje počet clusterů, dokuď je jejich počet větší než požadovaná hodnota
        while(currentNumberOfClusters > requestedNumberOfCluster){
            //Hodnoty jsou naplněny ve funkci find_neighbours
            int clusterIndex1;
            int clusterIndex2;
            find_neighbours(clusters, currentNumberOfClusters, &clusterIndex1, &clusterIndex2);
            merge_clusters(&clusters[clusterIndex1], &clusters[clusterIndex2]);
            currentNumberOfClusters = remove_cluster(clusters, currentNumberOfClusters, clusterIndex2);
        }
        print_clusters(clusters, currentNumberOfClusters);
        free_all_clusters(clusters, currentNumberOfClusters);
    }else{
        fprintf(stderr, "Wrong usage.\nDefault usage ./proj3 input.txt number_of_clusters" );
        return EXIT_FAILURE;
    }
    return 0;
}