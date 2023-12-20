build:
	gcc -Wall -std=c99 -lSDL2 -o renderer ./src/*.c

run:
	./renderer

clean:
	rm renderer

dev:
	make clean && make build && make run
