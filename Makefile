PORT=1337
IP=127.0.0.1
DIM = 3
DEFINE = -DMYPORT=${PORT} -DMYIP='"${IP}"' -DDIMENSION=${DIM}

all: online offline

online: server ttt.cgi /var/www/html/index.html
	touch online

offline: offline.c ttt_game.o full_solve.o optimal_play.o
	gcc offline.c ttt_game.o full_solve.o optimal_play.o -o offline ${DEFINE}

server: ttt_server.c ttt_game.o full_solve.o
	gcc ttt_server.c ttt_game.o full_solve.o -o server ${DEFINE}

ttt.cgi: ttt_cgi.c
	gcc ttt_cgi.c -o ttt.cgi ${DEFINE}
	chmod a+x ttt.cgi

/var/www/html/index.html: index.c
	gcc index.c -o index ${DEFINE}
	./index
	mv -f index.html /var/www/html
	chmod a+x /var/www/html/index.html

ttt_game.o: ttt_game.c ttt_game.h
	gcc ttt_game.c -c ${DEFINE}

full_solve.o: full_solve.c full_solve.h ttt_game.c ttt_game.h
	gcc full_solve.c -c ${DEFINE}

optimal_play.o: optimal_play.c optimal_play.h ttt_game.c ttt_game.h
	gcc optimal_play.c -c ${DEFINE}

clean:
	rm -f server ttt.cgi index
	rm -f /var/www/html/index.html
	rm -f ttt_game.o full_solve.o optimal_play.o
	rm -f online offline