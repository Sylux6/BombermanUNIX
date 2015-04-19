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


struct position{
	unsigned int x;
	unsigned int y;
};
typedef struct position position;
struct player{
	char* name;
	char* view;
	struct position pos;
	uint8_t life;
	uint8_t nb_bombe;
	uint8_t speed;
	uint8_t range_boom;
};
typedef struct player player;

void spawn(player *p,int pos_x,int pos_y,int width,int length);

player create_player(int nb);

#endif
