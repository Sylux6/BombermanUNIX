#ifndef _PLAYER_INCLUDED
#define _PLAYER_INCLUDED

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
#include "powerup.h"

struct board;

struct player {
	int nb;
	char* name;
	char* view;
	char* color;
	char* effet;
	struct position_{
		int x;
		int y;
	}pos;
	uint8_t life;
	uint8_t nb_bomb;
	int speed;
	int wait;
	int invinsible;
	int invinsible_time;
	uint8_t radius_bomb;
	struct bomb* bomb_own;
	int bomb_max;
};
typedef struct player player;

// structure primaire a ameliorer
struct bomb {
	int time; //explose dans time milliseconde 
	int state; // 0 = none , 1 = poser , 2 = explosert
	int time_explode;
	int x;
	int y;
	player owner;
};

void spawn(struct player *p, struct board *map);

player create_player(int nb);
void upgradeNumberBomb(struct player* p);
void explode(int x, int y, struct player p,struct board *map);

void clear_range_bomb(int x, int y, struct player p, struct board *map);

void in_explode(struct player *p1,struct player *p2 ,int x,int y,int range,struct board *map);

void is_touch(struct player *p1, struct player *p2,struct board *map);

#endif
