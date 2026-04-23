#link
hexaPrint : hexaPrint.o
	gcc -m32 -g -Wall -o hexaPrint hexaPrint.o

#compile 
hexaPrint.o : hexaPrint.c
	gcc -m32 -g -Wall -c -o hexaPrint.o hexaPrint.c

#clean
clean :
	rm -f *.o hexaPrint