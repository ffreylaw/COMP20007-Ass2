/* COMP20007 Design of Algorithms
 * Semester 1 2016
 *
 * Geoffrey Law
 * 759218
*/
#include "extra.h"
#include "hash.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_RANDOMS     8
#define NUM_ASCIIS      94
#define ASCII_LOWER     33
#define ASCII_UPPER     126

int is_prime(int n);
void char_swap(char *x, char *y);
void str_gen(char *str, int left, int rght, int k,
             unsigned int size, int *counter, unsigned int *r);
unsigned int extended_euclid(unsigned int a, unsigned int b, int *x, int *y);
int check_zero(unsigned int size, int n, unsigned int *r);

/* Determine appropriate size of a hash table given input size n */
unsigned int determine_size(unsigned int n) {
    unsigned int size = 2*n + 1;
    /* find next prime */
    while (!is_prime(size)) {
        size++;
    }
    return size;
}

/* Print n strings that are hashed to 0 by universal_hash seeded with seed */
void collide_dumb(unsigned int size, unsigned int seed, int n) {
    srand(seed);
    int i = 0, counter = n;
    unsigned int r[NUM_RANDOMS];
    char ascii[NUM_ASCIIS];

    fprintf(stdout, "Number of randoms: %d\n", NUM_RANDOMS);
    fprintf(stdout, "Random elements: \n");
    /* construct r (the same as the one used in universal_hash) */
    for (i = 0; i < NUM_RANDOMS; i++) {
        r[i] = rand() % size;
        fprintf(stdout, "%d\n", r[i]);
    }

    /* generate a string of all distinct printable ASCII characters */
    for (i = 0; i < NUM_ASCIIS; i++) {
        ascii[i] = i + ASCII_LOWER;
    }

    fprintf(stdout, "Clashing strings: \n");
    /* enumerate strings */
    for (i = 1; i < NUM_ASCIIS; i++) {
        str_gen(ascii, 0, NUM_ASCIIS-1, i, size, &counter, r);
        if (counter == 0) {
            break;
        }
    }
}

/* Print n strings that are hashed to 0 by universal_hash seeded with seed */
void collide_clever(unsigned int size, unsigned int seed, int n) {
    srand(seed);
    int i = 0, j = 0, counter = 0;
    unsigned int r[NUM_RANDOMS];

    fprintf(stdout, "Number of randoms: %d\n", NUM_RANDOMS);
    fprintf(stdout, "Random elements: \n");
    /* construct r (the same as the one used in universal_hash) */
    for (i = 0; i < NUM_RANDOMS; i++) {
        r[i] = rand() % size;
        fprintf(stdout, "%d\n", r[i]);
    }

    /* check whether first two value of r has a zero */
    if (!check_zero(size, n, r)) {
        return;
    }

    fprintf(stdout, "Clashing strings: \n");
    /* one character string (see report for detailed explaination) */
    unsigned int s = 0;
    j = 0;
    while ((counter < n) && ((s = size + size * j) <= ASCII_UPPER)) {
        if (s >= ASCII_LOWER) {
            fprintf(stdout, "%c\n", s);
            counter++;
        }
        j++;
    }

    /* two character string (see report for detailed explaination) */
    int u0 = 0, v0 = 0, cnst = 0, upper = 0, w = 0, z = 0;
    extended_euclid(r[1], size, &u0, &v0);
    for (i = ASCII_LOWER; i <= ASCII_UPPER; i++) {
        if (counter == n) {
            break;
        }
        j = 0;
        cnst = r[0] * abs(u0) * i;
        upper = abs(cnst - ASCII_LOWER) < abs(cnst - ASCII_UPPER) ?
                abs(cnst - ASCII_UPPER) : abs(cnst - ASCII_LOWER);
        while (((w = upper - j) % size) != 0) {
            if (j == NUM_ASCIIS) {
                break;
            }
            j++;
        }
        w = cnst - w;
        j = 0;
        while ((counter < n) && ((z = w + size * j) <= ASCII_UPPER)) {
            if (z >= ASCII_LOWER) {
                fprintf(stdout, "%c%c\n", i, z);
                counter++;
            }
            j++;
        }
    }
    if (counter < n) {
        fprintf(stderr, "collide_clever fails to generate more strings\n");
        exit(EXIT_FAILURE);
    }
}

/* Determine if an input number is prime (Moffat, 2012) */
int is_prime(int n) {
    int divisor, isprime;
	isprime = 1;
	for (divisor = 2; divisor*divisor <= n; divisor++) {
		if (n%divisor==0) {
			isprime = 0;
			break;
		}
	}
	return isprime;
}
/* Reference
 * Moffat, A. (2012). Programming, Problem Solving, and Abstraction with C.
 * Sydney, Australia: Pearson Custom Books.
 */

/* Swap pointer to character x and y */
void char_swap(char *x, char *y) {
    char temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

/* String generator permutes all distinct ASCII characters for collide_dumb */
void str_gen(char *str, int left, int rght, int k,
             unsigned int size, int *counter, unsigned int *r) {
    /* check whether reaches the number of clashing strings that asked */
    if (*counter == 0) {
        return;
    }
    int i = 0;
    if (left == k) {
        int sum = 0;
        /* iterate a substring of length k that can be formed from the str */
        for (i = 0; i < k; i++) {
            sum += r[i] * str[i];
        }
        /* refer to universal hash, tests whether the string hashes to 0 */
        if (sum % size == 0) {
            for (i = 0; i < k; i++) {
                fprintf(stdout, "%c", str[i]);
            }
            fprintf(stdout, "\n");
            (*counter)--;
        }
    } else {
        for (i = left; i <= rght; i++) {
            char_swap((str+left), (str+i));
            /* backtracking (FEEL THE RECURSION!) */
            str_gen(str, left+1, rght, k, size, counter, r);
            char_swap((str+left), (str+i));
        }
    }
}

/* Extended Eculid Algorithm */
unsigned int extended_euclid(unsigned int a, unsigned int b, int *x, int *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    int x1, y1;
    unsigned int gcd = extended_euclid(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return gcd;
}

/* Consider if r0 or r1 is zero in collide_clever (see report for detail) */
int check_zero(unsigned int size, int n, unsigned int *r) {
    if (r[0] != 0 && r[1] != 0) {
        /* both are not zero */
        return 1;
    } else if ((r[0] == 0 && r[1] != 0) || (r[0] != 0 && r[1] == 0)) {
        /* either one is zero */
        fprintf(stdout, "Clashing strings: \n");
        int i = 0, j = 0, counter = 0;
        unsigned int x = 0;
        if (r[0] == 0) {
            for (i = ASCII_LOWER; i <= ASCII_UPPER; i++) {
                if (counter == n) {
                    break;
                }
                fprintf(stdout, "%c\n", i);
                counter++;
            }
        } else {
            j = 0;
            while ((counter < n) && ((x = size + size * j) <= ASCII_UPPER)) {
                if (x >= ASCII_LOWER) {
                    fprintf(stdout, "%c\n", x);
                    counter++;
                }
                j++;
            }
        }
        for (i = ASCII_LOWER; i <= ASCII_UPPER; i++) {
            if (counter == n) {
                break;
            }
            j = 0;
            while ((counter < n) && ((x = size + size * j) <= ASCII_UPPER)) {
                if (x >= ASCII_LOWER) {
                    if (r[0] == 0) {
                        fprintf(stdout, "%c%c\n", i, x);
                    } else {
                        fprintf(stdout, "%c%c\n", x, i);
                    }
                    counter++;
                }
                j++;
            }
        }
        if (counter < n) {
            fprintf(stderr, "collide_clever fails to generate more strings\n");
            exit(EXIT_FAILURE);
        }
        return 0;
    } else {
        /* both are zero */
        int i = 0, counter = n;
        char ascii[NUM_ASCIIS];
        for (i = 0; i < NUM_ASCIIS; i++) {
            ascii[i] = i + ASCII_LOWER;
        }
        fprintf(stdout, "Clashing strings: \n");
        for (i = 1; i < NUM_ASCIIS; i++) {
            str_gen(ascii, 0, NUM_ASCIIS-1, i, size, &counter, r);
            if (counter == 0) {
                break;
            }
        }
        return 0;
    }
}
