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
struct bombe{
	int time; //explose dans time seconde
	int state; // 0 = none , 1 = poser , 2 = exploser
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
	uint8_t nb_bombe;
	uint8_t speed;
	uint8_t radius_bomb;
	struct bombe bombe_own[10]; // max 10 bombe (c'est bien non ?)
};

typedef struct player player;


void spawn(struct player *p, struct board *map);

struct player create_player(int nb);

#endif
