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

#define GOD_TIME 1000

struct board;

struct player {
	int nb;
	char* name;
	char* view;
	char* color;
	char* effet;
	struct position_ {
		int x;
		int y;
	} pos;
	int life;
	int nb_bomb;
	int nb_bomb_set;
	int speed;
	int wait;
	int invinsible;
	int invinsible_time;
	int radius_bomb;
};
typedef struct player *player;

void spawn(struct player *p, struct board *map);

player create_player(int nb);

void upgradeNumberBomb(struct player* p,int value);

void upgradeRaduis(struct player* p,int value);

void upgradeSpeed(struct player* p,int value);

int endOfInvinsibility(player p1,player p2);

void setInvinsibility(player p);

#endif
