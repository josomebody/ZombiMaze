CC=g++
all: zombimaze

zombimaze: zombimaze.o main.o
	$(CC) *.o -o zombimaze -lSDL -lSDL_image -I.

clean: 
	rm *.o zombimaze

