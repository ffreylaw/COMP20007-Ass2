/*
 * COMP20007 Design of Algorithms
 * Semester 1 2016
 *
 * Clement Poh (cpoh@unimelb.edu.au)
 *
 * This module provides fundamental functionality used to
 * interpret generic pointers into typed data.
 *
 * Geoffrey Law
 * 759218
*/
#include "types.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

/* Returns whether n1 is equal to n2 */
bool int_eq(long e1, long e2) {
    return e1 == e2;
}

/* Prints n to file */
void int_print(FILE *file, long e) {
    fprintf(file, " %ld", e);
}

/* Returns whether str1 is equal to str2 */
bool str_eq(char *str1, char *str2) {
    return strcmp(str1, str2) == 0 ? true : false;
}

/* Returns a copy of src */
char *str_copy(char *src) {
    char *str = NULL;
    str = (char*)malloc(strlen(src) * sizeof(char));
    src[strlen(src)-1] = '\0';
    return strcpy(str, src);
}

/* Prints str to file */
void str_print(FILE *file, char *str) {
    fprintf(file, " %s", str);
}
