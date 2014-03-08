main: matrix.o transformations.o line.o parse_util.o background.o
	gcc interpret.c matrix.o transformations.o line.o parse_util.o background.o -lm
matrix.o: matrix.c matrix.h
	gcc -c matrix.c
transformations.o: transformations.c transformations.h
	gcc -c transformations.c
line.o: line.c line.h background.h
	gcc -c line.c
parse_util.o: parse_util.c parse_util.h
	gcc -c parse_util.c
background.o: background.c background.h
	gcc -c background.c
clean:
	-rm *.o 
	-rm a.out
	-rm *.ppm
	-rm *.png
run:
	make clean
	make main
	./a.out test.data
	convert out.ppm out.png
	open out.png
