OPTIM=-O2
OPTIONS=-std=c99 -Wall -pedantic 

FILES=wordcount.c

all: wordcount wordcount-dynamic tail tail2


wordcount: wordcount.c htab*.c
	gcc ${OPTIM} ${OPTIONS} htab*.c -c
	ar rcs libhtab.a htab*.o
	gcc ${OPTIM} ${OPTIONS} wordcount.c io.c  libhtab.a -o wordcount

wordcount-dynamic: wordcount.c htab*.c
	gcc ${OPTIM} ${OPTIONS} -shared -fPIC htab*.c -o libhtab.so
	export LD_LIBRARY_PATH="."
	gcc ${OPTIM} ${OPTIONS} wordcount.c io.c -L. -lhtab -o wordcount-dynamic

tail: tail.c
	gcc ${OPTIM} ${OPTIONS} tail.c -o tail

tail2: tail2.cc
	g++ ${OPTIM} -Wall -pedantic  tail2.cc -o tail2

clear:
	find -name "*.o" -delete 	
	find -name "*.so" -delete 	
	find -name "*.gch" -delete 	
	find -name "*.a" -delete 	
	find -name "wordcount" -delete 	
	find -name "wordcount-dynamic" -delete 
	find -name "tail" -delete 		
	find -name "tail2" -delete 		