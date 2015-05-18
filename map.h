#ifndef _MAP_INCLUDED
#define _MAP_INCLUDED

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
#include <math.h>

#include "term.h"
#include "mylib.h"
#include "gameplay.h"
#include "powerup.h"
#include "bomb.h"

#define SAV_CUR "\x1b[s"
#define LOAD_CUR "\x1b[u"

#define CLEAR_AFTER_CUR "\x1b[0J"
#define CLEAR_BEFOR_CUR "\x1b[1J"
#define CLEAR_TERM "\x1b[2J\x1b[;H"
#define CUR_ON "\x1b[?25h"
#define CUR_OFF "\x1b[?25l"

#define ENTER 13
#define DEL 127

#define UP_ARROW 65
#define DOWN_ARROW 66
#define RIGHT_ARROW 67
#define LEFT_ARROW 68
typedef struct listBomb* listBomb;
typedef struct player* player;

typedef struct position{
	int x;
	int y;
} position;

struct board{
	char **map;
	struct powerup **powerups;
	struct bomb **bombs;
	listBomb listBombs;
	position up_left_corner;
	int x;
	int y;
	int refresh;
};
typedef struct board* board;

void map_init(board tab,char* map);

void launch_game(char* folder);

void print_map(board map,player p1, player p2);

void print_player(player p,board map);

void printBomb(board map);

void print_carac(struct player p1,struct player p2);

void del_board(board map);

int area_calcul(board, int, int);

#endif
