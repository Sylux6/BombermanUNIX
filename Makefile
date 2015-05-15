EXEC=Bomberman
SOURCES=bomb.c powerup.c gameplay.c game.c menu.c map.c term.c mylib.c player.c
OBJECTS=$(SOURCES:.c=.o)
CC=gcc
CFLAGES= -std=gnu99 -Wall -Werror
.PHONY: default clean

default: $(EXEC)

game.o: game.c
player.o: player.c player.h
menu.o: menu.c menu.h
map.o: map.c map.h
term.o: term.c term.h
mylib.o: mylib.c mylib.h
gameplay.o: gameplay.c gameplay.h
powerup.o: powerup.c powerup.h
bomb.o: bomb.c bomb.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGES)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^

clean:
	rm $(EXEC) $(OBJECTS)