/* COMP20007 Design of Algorithms
 * Semester 1 2016
 *
 * Geoffrey Law
 * 759218
*/
#include "hash.h"

#include <stdlib.h>

#define MAXSTRLEN 256

/* Used as the second hashing function on double hash */
unsigned int linear_probe(void *e, unsigned int size) {
    (void) e;
    (void) size;
    return 1;
}

/* Very simple hash */
unsigned int worst_hash(void *e, unsigned int size) {
    (void) e;
    (void) size;
    return 0;
}

/* Basic numeric hash function */
unsigned int num_hash(long n, unsigned int size) {
    return n % size;
}

/* Bad hash function */
unsigned int bad_hash(char *key, unsigned int size) {
    static int flag = 0;
    static unsigned int a = 0;
    if (!flag) {
        flag = 1;
        a = rand() % size;
    }
    return a * (*key) % size;
}

/* Universal hash function as described in Dasgupta et al 1.5.2 */
unsigned int universal_hash(unsigned char *key, unsigned int size) {
    int i = 0;
    static int flag = 0;
    unsigned int sum = 0;
    static unsigned int r[MAXSTRLEN];
    if (!flag) {
        flag = 1;
        for (i = 0; i < MAXSTRLEN; i++) {
            r[i] = rand() % size;
        }
    }
    for (i = 0; *key; i++) {
        /* summation */
        sum += r[i] * (unsigned int)(*key);
        key++;
    }
    return sum % size;
}
