OPTIM=-O2
OPTIONS=-std=c11 -Wall -pedantic 

all: primes primes-i steg-decode

run: primes primes-i
	time ./primes
	time ./primes-i

primes: primes.c eratosthenes.c error.c error.h bit_array.h eratosthenes.h
	gcc ${OPTIM} ${OPTIONS} -o primes eratosthenes.c eratosthenes.h error.c primes.c -lm

primes-i: primes.c eratosthenes.c error.c error.h bit_array.h eratosthenes.h
	gcc ${OPTIM} ${OPTIONS} -DUSEINLINE -o primes-i eratosthenes.c eratosthenes.h error.c primes.c -lm

steg-decode: steg-decode.c  error.c error.h bit_array.h eratosthenes.h eratosthenes.c ppm.h ppm.c
		gcc ${OPTIM} ${OPTIONS}  -o steg-decode  eratosthenes.c eratosthenes.h error.c steg-decode.c ppm.h ppm.c -lm


