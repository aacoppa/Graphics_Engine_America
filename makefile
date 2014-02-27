main: matrix.o
	gcc main.c matrix.o 
matrix.o: matrix.c matrix.h
	gcc -c matrix.c
clean:
	-rm *.o 
	-rm a.out
	
