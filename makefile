#link
Bsort : Bsort.o
	gcc -m32 -g -Wall -o Bsort Bsort.o

#compile 
Bsort.o : Bsort.c
	gcc -m32 -g -Wall -c -o Bsort.o Bsort.c

#clean
clean :
	rm -f *.o Bsort