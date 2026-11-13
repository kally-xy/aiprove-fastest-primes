/* Candidate B: pi-table (every 1024) + segmented tail sieve.
 * Read N from stdin, print pi(N). Only sieves the <=1024 numbers past
 * the last table point, using precomputed primes <= 2236. */
#include <stdio.h>
#include "tables.h"

static long read_n(void) {
    int c;
    long n = 0;
    int seen = 0;
    while ((c = getchar_unlocked()) != EOF) {
        if (c >= '0' && c <= '9') { n = n * 10 + (c - '0'); seen = 1; }
        else if (seen) break;
    }
    return seen ? n : -1;
}

int main(void) {
    long n = read_n();
    if (n < 0) return 0;              /* empty stdin: exit quietly */
    if (n < 2) { printf("0\n"); return 0; }
    long base = (n / 1024) * 1024;
    long count = pi_table[base / 1024];
    if (n == base) { printf("%ld\n", count); return 0; }
    if (base == 0) count = 1;         /* prime 2 not in table[0] */
    /* sieve odd numbers in (base, n]; base is even, first odd is base+1 */
    unsigned char seg[512 + 1];
    long nseg = (n - base) / 2 + 1;   /* odds base+1 .. n (n odd -> +1) */
    if (nseg > 513) nseg = 513;
    for (long i = 0; i < nseg; i++) seg[i] = 0;
    /* NB: skip small_primes[0] == 2: 2 has no odd multiples, and the
     * seg[] slots represent odd numbers only. Marking with p==2 would
     * map even j onto odd slots and wrongly kill primes. */
    for (int k = 1; k < N_SMALL_PRIMES; k++) {
        long p = small_primes[k];
        long p2 = p * p;
        if (p2 > n) break;
        long start = p2 > base ? p2 : ((base / p) + 1) * p;
        if (start == p) start += 2 * p;
        if ((start & 1) == 0) start += p;
        for (long j = start; j <= n; j += 2 * p)
            seg[(j - base - 1) / 2] = 1;
    }
    for (long x = base + 1; x <= n; x += 2) {
        if (x >= 3 && !seg[(x - base - 1) / 2]) count++;
    }
    printf("%ld\n", count);
    return 0;
}
