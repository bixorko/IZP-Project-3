/**
 * Kostra hlavickoveho souboru 3. projekt IZP 2018/19
 * a pro dokumentaci Javadoc.
 */

/**
 * @file
 * @author Peter Vinarcik
 * @date 9.12.2018
*/

/**
 * @defgroup struktury STRUKTURY
 * @{
 */

/**
 * @brief Struktura objektu
 *
 * [ID] a Suradnice objektu
 */

struct obj_t {
    int id;
    float x;
    float y;
};

/**
 * @brief Struktura clusteru
 *
 * + Odkazuje na objekt
 */

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/**
 * @}
 */

/**
 * @defgroup praca_clustre PRACA S CLUSTERMI
 * @{
 */

/**
 * @brief Inicializacia Clusteru
 *
 * Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 * <i>Ukazatel NULL u pole objektu znamena kapacitu 0.</i>
 *
 * @pre cluster_t c nesmie byt prazdny
 *
 * @pre kapacita clusteru musi byt vacsia ako 0
 *
 * @param c Pointer na cluster_t
 * @param cap Definuje kapacitu daneho clusteru
 */

void init_cluster(struct cluster_t *c, int cap);

/**
 * @brief Clear Cluster
 *
 * Odstraneni vsech objektu shluku.
 * Inicializace na prazdny shluk.
 *
 * @param c Pointer na cluster_t, ktory bude odstraneny
 */

void clear_cluster(struct cluster_t *c);

/**
 * @brief Hodnota Resizu
 *
 * Hodnota pre reallokaciu pamate.
 * Pre kazdy resize reallocuje kapacitu +10.
 */

extern const int CLUSTER_CHUNK;

/**
 * @brief Resize Cluster
 *
 * Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 * Reallokuje kapacitu daneho clustru.
 * Reallokuje o velkost CLUSTER CHUNK.
 *
 * @pre cluster_t c musi existovat
 *
 * @pre povodna kapacita musi byt vacsia/rovna 0
 *
 * @pre nova kapacita musi byt vacsia/rovna 0
 *
 * @param c Pointer na cluster_t, ktory bude reallokovany
 * @param new_cap Hodnota kapacity o ktoru bude zvacseny
 *
 * @return Vracia cluster s novou kapacitou.
 */

struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * @brief Append Cluster
 *
 * @param c pointer na cluster kde bude pridany objekt
 * @param obj objekt ktory bude priradeny do clusteru
 *
 * @if Kapacita clusteru <= velkosti clusteru
 *  Rozsiri cluster o CLUSTER_CHUNK.
 *  Prida objekt 'obj' na konec shluku 'c'.
 * @endif
 *
 * @if Kapacita clusteru > velkost clusteru
 *  Prida objekt 'obj' na konec shluku 'c'.
 * @endif
 */

void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * @brief Merge Clusters
 *
 * Do clusteru 'c1' prida objekty 'c2'
 * Zoradi objekty v clusteri c1
 * Vzostupne podla identifikacneho cisla
 * Cluster c2 ostane nezmeneny
 *
 * @param c1 Cluster do ktoreho budu priradeny objekty z c2
 * @param c2 Cluster z ktoreho sa budu priradovat objekty do c1
 *
 * @pre Cluster c1 nesmie byt prazdny
 *
 * @pre Cluster c2 nesmie byt prazdny
 *
 * @if Kapacita c1 <= velkosti clusteru
 *  Rozsiri cluster c1 o CLUSTER_CHUNK.
 * @endif
 */

void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief Remove Cluster
 *
 * Odstrani cluster z pola shlukov 'carr'
 * Cluster na odstranenie je na indexe 'idx'
 *
 * @param carr Pole clusterov z ktoreho bude cluster odstraneny
 * @param narr Velkost pola clustrov
 * @param idx Index clustru, ktory bude z pola clustrov odstraneny
 *
 * @pre index 'idx' musi byt mensi ako max. velkost pola
 *
 * @pre velkost pola clustrov musi byt vacsia ako 0
 *
 * @return nova_velkost vrati novu velkost pola (povodna velkost -1)
 */

int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * @}
 */

/**
 * @defgroup vzdialenosti VZDIALENOSTI
 * @{
 */

/**
 * @brief Vzdialenost objektov
 *
 * Pocita euklidovsku vzdialenost dvoch objektov
 *
 * @param o1 Prvy objekt pre vypocet vzdialenosti
 * @param o2 Druhy objekt pre vypocet vzdialenosti
 *
 * @pre Objekt o1 nesmie byt prazdny
 *
 * @pre Objekt o2 nesmie byt prazdny.
 *
 * @return Vracia vzdialenost dvoch objektov.
 */

float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * @brief Vzdialenost clusterov
 *
 * Pocita vzdialenost dvoch clusterov.
 *
 * @param c1 Prvy cluster pre vypocet vzdialenosti
 * @param c2 Druhy cluster pre vypocet vzdialenosti
 *
 * @pre Cluster c1 nesmie byt prazdny
 *
 * @pre Velkost clusteru c1 musi byt vacsia ako 0
 *
 * @pre Cluster c2 nesmie byt prazdny
 *
 * @pre Velkost cluster c2 musi byt vacsia ako 0
 *
 * @return Vracia najmensiu vzdialenost dvoch clustrov
 */

float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief Najdenie dvoch najblizsich clustrov
 *
 * Funckia najde dva najblizsie clustre.
 * V poli clustrov hlada dva najblizsie clustre
 * Po najdeni identifikuje ich indexy
 * Po indentifikovani ich ulozi do c1 a c2
 *
 * @param carr Pole clusterov
 * @param narr Velkost pola clusterov
 * @param c1 Index dvoch najblizsich zhlukov - 1 Index
 * @param c2 Index dvoch najblizsich zhlukov - 2 Index
 *
 * @pre velkost pola narr musi byt vacsia ako 0
 */

void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * @brief Zoradenie clusterov
 *
 * Zoradi objekty v clusteri
 * Zoraduje vzostupne podla ID
 *
 * @param c Pointer na cluster ktory ma byt zoradeny
 */

/**
 * @}
 */

void sort_cluster(struct cluster_t *c);

/**
 * @defgroup vypisanie VYPISANIE CLUSTEROV
 * @{
 */

/**
 * @brief Vypis clusteru
 *
 * Vypise cluster 'c' na stdout.
 *
 * @param c
 */

void print_cluster(struct cluster_t *c);

/**
 *@}
 */

/**
 * @defgroup loadovanie NACITANIE CLUSTEROV
 * @{
 */

/**
 * @brief Nacitanie Clusterov
 *
 * Nacita objekty zo suboru
 * Vytvori cluster pre kazdy objekt
 * Ulozi dany objekt do clusteru
 * Alokacia priestoru pre pole vsetkych clusterov
 *
 * @param filename Subor pre nacitanie objektov
 * @param arr Pointer na pole clusterov nacitanych zo suboru
 *
 * @pre pole clusterov 'arr' nesmie byt prazdne
 *
 * @return Vracia pocet nacitanych objektov resp. clusterov
 */


int load_clusters(char *filename, struct cluster_t **arr);

/**
 *@}
 */

/**
 * @ingroup vypisanie
 * @{
 */

/**
 * @brief Vypis pola clusterov
 *
 * Vypise pole clusterov.
 * Vypise sa prvych 'narr' znakov
 *
 * @param carr Pointer na prvy cluster
 * @param narr Vypise prvych 'narr' clusterov
 */

void print_clusters(struct cluster_t *carr, int narr);

/**
  *@}
  */