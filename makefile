all: bin/matrix.o bin/transformations.o bin/line.o bin/parse_util.o bin/sphere.o bin/vector.o bin/box.o bin/screen.o bin/renderer.o
	gcc -Wall src/interpret.c $^ -lm `sdl-config --cflags --libs` -o bin/a.out

bin/%.o: src/matrix/%.c
	gcc -o $@ -c $^

bin/%.o: src/shapes/%.c
	gcc -o $@ -c $^

bin/%.o: src/%.c
	gcc -o $@ -c $^

clean:
	-rm *.o 
	-rm a.out
	-rm *.ppm
	-rm *.gif
	-rm *.png
run:
	make clean
	make all 
	./bin/a.out
