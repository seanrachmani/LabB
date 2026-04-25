#link
AntiVirus : AntiVirus.o
	gcc -m32 -g -Wall -o AntiVirus AntiVirus.o

#compile 
AntiVirus.o : AntiVirus.c
	gcc -m32 -g -Wall -c -o AntiVirus.o AntiVirus.c

#clean
clean :
	rm -f *.o AntiVirus