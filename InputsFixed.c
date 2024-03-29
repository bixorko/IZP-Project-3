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
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    // TODO
    c->obj = malloc(sizeof(struct obj_t) * cap);                        //alokuje pamat pre kapacitu aktualenho objektu
    if(c->obj != NULL){                                                 //ak je c->obj rozny od NULL
        c->capacity = cap;                                              //kapacite priradi velkost cap
        c->size = 0;                                                    //nastavi c->size na 0
    }else {
        c->capacity = 0;                                                //ak = NULL, capacity = 0
        c->size = 0;                                                    //a size = 0
    }
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO
    free(c->obj);
    c->obj = NULL;
    c->size = 0;
    c->capacity = 0;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
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
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    // TODO
    if (c->capacity <= c->size) {                                          //skontroluje ci je kapacita mensia ako velkost struktury
        resize_cluster(c, c->capacity + CLUSTER_CHUNK);                    //ak ano reallocuje pamat o CLLUSTER CHUNK
        c->obj[c->size] = obj;                                             //priradi do pola objekt na volnu poziciu
        c->size++;                                                         //zvacsi size o 1 pre dalsiu alokaciu resp. append
    } else {
        c->obj[c->size] = obj;                                             //to iste co pri vyssej podmienke
        c->size++;                                                         //ale bez reallokacie
    }
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
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    // TODO
    if(c1->capacity <= c2->size) {                                         //skontroluje kapacitu clustru
        resize_cluster(c1, c1->capacity + CLUSTER_CHUNK);                  //ak by bol size vacsi tak resize
        for (int i = 0; i < c2->size; i++) {                               //kym je cluster mensi ako c2->size
            append_cluster(c1, c2->obj[i]);                                //vlozci c2 do c1
        }
    }
    else{
        for (int i = 0; i < c2->size; i++) {                               //kym je cluster mensi ako c2->size
            append_cluster(c1, c2->obj[i]);                                //vlozi c2 do c1
        }
    }

    sort_cluster(c1);                                                      //zoradi cluster c1 podla indexu
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    // TODO
    int nova_velkost;                                                      //premenna pre novu velkost
    nova_velkost = narr-1;                                                 //odstranujeme 1 prvok z velkosti narr cize nova velkost bude narr-1;

    clear_cluster(&carr[idx]);                                             //vycisti miesto v poli clustrov pre objekt, ktory prenasame do druheho clustra
    for (int c = idx; c < nova_velkost; c++){                              //preindexuje cele pole
        carr[c] = carr[c+1];                                               //dalsi = predchadzajuci az po idx
    }

    return nova_velkost;                                                   //vrati novu velkost pola
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    // TODO
    float vzdialenost_obj;
    vzdialenost_obj = sqrtf(powf(o1->x - o2->x,2) + powf(o1->y - o2->y,2));//vzorec pre euklidovsku vzdialenost
                                                                           //odmocnina z [(a1-a2)^2 + (b1-b2)^2]
    return vzdialenost_obj;                                                //vrati vypocet vzdialenosti podla vzorca
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // TODO
    float stara_vzdialenost = INT_MAX;                                     //prvotna vzdialenost - bude prepisana
    float nova_vzdialenost;                                                //pomocna vzdialenost
    for (int i = 0; i < c1->size; i++){                                    //porovnava vsetky objekty 0-0/1/2
        for (int j = 0; j < c2->size; j++){                                //potom porovna 1-0/1/2
            nova_vzdialenost = obj_distance(&c1->obj[i], &c2->obj[j]);     //vypocita vzdialenost dvoch objektov
            if (nova_vzdialenost < stara_vzdialenost){                     //ak je nova vzdialenost mensia ako stara
                stara_vzdialenost = nova_vzdialenost;                      //priradi do starej novu
            }
        }
    }

    return stara_vzdialenost;                                              //vrati najmensiu vzdialenost
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    // TODO
    float stara_vzdialenost = INT_MAX;                                     //stara vzdialenost = INT_MAX-> bude prepisana
    float nova_vzdialenost;                                                //pomocna vzdialenost
    for(int i = 0; i<narr; i++){                                           //kym neprejde vsetky objekty 0-1/2/3
        for(int j = i+1; j<narr; j++) {                                    //ked prejde vsetkymi zvysi i a ide 1-2/3
            nova_vzdialenost = cluster_distance(&carr[i],&carr[j]);        //zisti vzdialenost 2 clustrov
            if(nova_vzdialenost < stara_vzdialenost) {                     //ak je nova vdialenost mensia ako stara
                stara_vzdialenost = nova_vzdialenost;                      //priradi novu do starej
                *c1 = i;                                                   //na konci najde indexy
                *c2 = j;                                                   //dvoch najblizsich a v merge ich zluci
            }
        }
    }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
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
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    // TODO
    FILE *subor;
    subor = fopen(filename, "r");                                          //otvori subor z argv1
    if (subor == NULL){                                                    //ak sa nepodari otvorit
        fprintf(stderr, "Nepodarilo sa otvorit subor\n");                  //chybove hlasnie na stderr
        return -1;                                                         //vrati neuspesne ukoncenie programu
    }

    int count, nacitane = 0;                                               //count = N --> nacitane cez fscanf zo suboru
    struct obj_t arr_obj;                                                  //obj pre strukturu obj_t

    if (fscanf(subor, "count=%d", &count) != 1){                           //ak prvy riadok nebude count=N
        fprintf(stderr, "Zle zadany prvy riadok. [count=N]\n");            //chybovy vystup
        return -1;                                                         //chybne ukoncenie suboru
    }                                                                      //ak je zadany dobre nacita N

    *arr = malloc(sizeof(struct cluster_t)*count);                         //alokacia pre vstupny subor
                                                                           //toto pole potom odosle do inych funkcii
    for (int i = 0; i<count; i++){
        fscanf(subor, "%d %f %f", &arr_obj.id, &arr_obj.x, &arr_obj.y);    //nacitanie id+suradnic [id,x,y]
        nacitane++;                                                        //pocita pocet nacitanych objektov
        init_cluster(&(*arr)[i],CLUSTER_CHUNK);                            //inicializacia kapacity pre shluk
        append_cluster(&(*arr)[i],arr_obj);                                //prida shluk
    }

    fclose(subor);                                                         //zatvori otvoreny subor
    return nacitane;                                                       //vrati pocet nacitanych objektov
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    struct cluster_t *clusters;
    // TODO
    long int arg_n;                                                        //premenna pre argument [argv1]
    int count;                                                             //nacitanie count=N zo suboru[vracia pocet nacitanych objektov
    int sused1, sused2;                                                    //premenne pre funkcie s hladnaim najblizsich zhlukov
    if (argc == 3) {                                                       //ak je zadany aj argument argv2
        count = load_clusters(argv[1], &clusters);                         //prida do premennej celkovy pocet objektov v suboru
        print_clusters(clusters, count);                                   //vypise naloadene clustre
        char *znakN;
        arg_n = strtol(argv[2], &znakN, 10);                               //prevedie argument na int + kontrola vstupu
        if (*znakN != '\0' || arg_n <= 0){                                 //ak znakN nie je prazdny a N je zaporne
            fprintf(stderr, "Zle zadany argument N\n");                    //chhybovy vystup
            return -1;                                                     //exit_failure
        }else {
            for (; count > arg_n;){                                        //cyklus prebehne kym pocet clustrov nebude = pozadovanemu argumentu
                find_neighbours(clusters, count, &sused1, &sused2);        //posle nacitane pole do fce a najde indexy dvoch najblizsich objektov
                merge_clusters(&clusters[sused1], &clusters[sused2]);      //hodi cluster 2 do clustru 1
                count = remove_cluster(clusters, count, sused2);           //zmaze cluster 2
            }
        }
    }
    else if (argc == 2) {                                                  //ak nie je zadany argv[2]
        count = load_clusters(argv[1], &clusters);                         //nacitanie count = N zo suboru [vracia pocet nacitanuch objektov]
        print_clusters(clusters, count);                                   //vypise nacitane clustre
        arg_n = 1;                                                         //pri absencii argv2 sa vysledne clustre=1
        for (; count > arg_n;){
            find_neighbours(clusters, count, &sused1, &sused2);            //posle nacitane pole do fce a najde indexy dvoch najblizsich objektov
            merge_clusters(&clusters[sused1], &clusters[sused2]);          //hodi cluster 2 do clustru 1
            count = remove_cluster(clusters, count, sused2);               //zmaze cluster 2
        }
    }
    else {                                                                 //ak je zadany chybny pocet argumentov
        fprintf(stderr, "Zle zadany pocet arugmentov\n");                  //chybovy vystup
        return -1;                                                         //ukoncenie programu exit_failure
    }
    printf("\n");
    print_clusters(clusters, count);                                       //vypise zjednotene clustre

    for (int i = 0; i<count; i++){                                         //pre vsetky zostavajuce clustre
        clear_cluster(&clusters[i]);                                       //deallocuje pamat vsade kde sa vyuzil malloc
    }
    free(clusters);                                                        //vycisti pole clustrov

    return 0;
}