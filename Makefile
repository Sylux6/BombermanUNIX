EXEC=Bomberman
SOURCES=game.c menu.c map.c term.c lib.c
OBJECTS=$(SOURCES:.c=.o)
CC=gcc
CFLAGES=-Wall -Werror -std=gnu99
.PHONY: default clean

default: $(EXEC)

game.o: game.c menu.h map.h term.h lib.h
menu.o: menu.c menu.h
map.o: map.c map.h
term.o: term.c term.h
lib.o: lib.c lib.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGES)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^

clean:
	rm $(EXEC) $(OBJECTS) 
