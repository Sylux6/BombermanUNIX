#ifndef PLAYER
#define PLAYER

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <termios.h>

#include "mylib.h"
#include "term.h"
#include "map.h"

struct board;

// structure primaire a amelioré
struct bomb{
	int time; //explose dans time seconde
	int state; // 0 = none , 1 = poser , 2 = exploser
	int x;
	int y;
};

struct player{
	int nb;
	char* name;
	char* view;
	char *color;
	struct position_{
		int x;
		int y;
	}pos;
	uint8_t life;
	uint8_t nb_bomb;
	int speed;
	int wait;
	uint8_t radius_bomb;
	struct bomb* bomb_own;
	int bomb_max;
};

typedef struct player player;


void spawn(struct player *p, struct board *map);

struct player create_player(int nb);

#endif
