#link
virusDetector : virusDetector.o
	gcc -m32 -g -Wall -o virusDetector virusDetector.o

#compile 
virusDetector.o : virusDetector.c
	gcc -m32 -g -Wall -c -o virusDetector.o virusDetector.c

#clean
clean :
	rm -f *.o virusDetector