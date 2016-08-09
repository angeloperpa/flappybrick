CFLAGS = -Wall
NCURSES_FLAGS = -lncurses

all: flappy_brick

flappy_brick:	flappy_brick.o bib_game.o data.h
	gcc flappy_brick.o bib_game.o data.h -o flappy_brick $(CFLAGS) $(NCURSES_FLAGS)

flappy_brick.o: flappy_brick.c data.h bib_game.h
	gcc -c flappy_brick.c $(CFLAGS) $(NCURSES_FLAGS)

bib_game.o: bib_game.c data.h
	gcc -c bib_game.c $(CFLAGS) $(NCURSES_FLAGS)

clean:
	-rm *.o

purge:
	-rm flappy_brick
