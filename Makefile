EXEC=Bomberman
SOURCES=game.c menu.c map.c term.c mylib.c
OBJECTS=$(SOURCES:.c=.o)
CC=gcc
CFLAGES=-Wall -Werror -std=gnu99
.PHONY: default clean

default: $(EXEC)

game.o: game.c
menu.o: menu.c menu.h
map.o: map.c map.h
term.o: term.c term.h
mylib.o: mylib.c mylib.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGES)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^

clean:
	rm $(EXEC) $(OBJECTS)