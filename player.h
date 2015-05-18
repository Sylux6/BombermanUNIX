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

typedef struct board* board;
typedef struct listBomb* listBomb;
typedef struct bomb* bomb;

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

void upgradeRadius(struct player* p,int value);

void upgradeSpeed(struct player* p,int value);

void in_explode(player p1, player p2 , bomb b, board map);

void is_touch(player p1, player p2, board map);

int endOfInvinsibility(player p1,player p2);

void setInvinsibility(player p);

#endif
