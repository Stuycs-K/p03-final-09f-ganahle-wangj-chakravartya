notDefault:
	@echo "Insufficient args. "

compile: server client

server: server.c networking.c networking.h game.c game.h
	@gcc -Wall -Wextra -std=c11 server.c networking.c game.c -o server

client: client.c networking.c networking.h game.c game.h
	@gcc -Wall -Wextra -std=c11 client.c networking.c game.c -o client

clean:
	@rm -f server client *.o

.PHONY: notDefault compile clean
