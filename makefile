all: client server

compile: client server

client: client.o networking.o game.o
	@gcc -o client client.o networking.o game.o

server: server.o networking.o game.o
	@gcc -o server server.o networking.o game.o

client.o: client.c networking.h game.h
	@gcc -c client.c

server.o: server.c networking.h game.h
	@gcc -c server.c

networking.o: networking.c networking.h
	@gcc -c networking.c

game.o: game.c game.h
	@gcc -c game.c

clean:
	@rm -f server client *.o
