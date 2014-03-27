all: matrix.o transformations.o line.o parse_util.o sphere.o vector.o box.o screen.o renderer.o
	gcc -Wall interpret.c vector.o matrix.o transformations.o line.o parse_util.o sphere.o box.o renderer.o screen.o -lm `sdl-config --cflags --libs`
matrix.o: matrix.c matrix.h
	gcc -c matrix.c
vector.o:vector.c vector.h
	gcc -c vector.c
transformations.o: transformations.c transformations.h
	gcc -c transformations.c
line.o: line.c line.h 
	gcc -c line.c
parse_util.o: parse_util.c parse_util.h
	gcc -c parse_util.c
box.o: box.c box.h
	gcc -c box.c
sphere.o: sphere.c sphere.h
	gcc -c sphere.c
screen.o: screen.c screen.h
	gcc -c screen.c
renderer.o: renderer.c renderer.h
	gcc -c renderer.c
clean:
	-rm *.o 
	-rm a.out
	-rm *.ppm
	-rm *.gif
	-rm *.png
run:
	make clean
	make main
	./a.out sphere.data
	convert sphere.ppm sphere.png
	open sphere.png

run_all:
	make clean
	make main
	./a.out sphere.data
	./a.out cube.data
	./a.out cube1.data
	./a.out cube2.data
	./a.out mystery.data
	convert sphere.ppm sphere.gif
	convert cube.ppm cube.gif
	convert cube1.ppm cube1.gif
	convert cube2.ppm cube2.gif
	convert mystery.ppm mystery.gif
sdl:
	gcc main.c `sdl-config --cflags --libs`
