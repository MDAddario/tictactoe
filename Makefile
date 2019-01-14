DIMENSION = 3
DEFINE = -DDIMENSION=${DIMENSION}

all: main

main: main.c ttt_game.o full_solve.o optimal_play.o
	gcc main.c ttt_game.o full_solve.o optimal_play.o -o main ${DEFINE}

ttt_game.o: ttt_game.c ttt_game.h
	gcc ttt_game.c -c ${DEFINE}

full_solve.o: full_solve.c full_solve.h ttt_game.c ttt_game.h
	gcc full_solve.c -c ${DEFINE}

optimal_play.o: optimal_play.c optimal_play.h ttt_game.c ttt_game.h
	gcc optimal_play.c -c ${DEFINE}

clean:
	rm -f main ttt_game.o full_solve.o optimal_play.o
