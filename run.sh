#!/bin/sh
# fastest-primes entry: print pi(N) for N read from stdin.
# Fast path: prebuilt static binary (no libc dependency, ~0.5 ms/run).
# Fallback: if the binary cannot execute (e.g. wrong architecture),
# compile from source with cc (static first, then dynamic) and run that.
# NB: primes.c exits 0 whenever it actually runs, so a non-zero exit here
# can only mean the binary failed to execute -- stdin is then still unread.
dir=${0%/*}
[ "$dir" != "$0" ] && cd "$dir"
./primes || {
    cc -O3 -static -o primes primes.c 2>/dev/null || cc -O3 -o primes primes.c
    exec ./primes
}
