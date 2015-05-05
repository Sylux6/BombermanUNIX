#ifndef MAP
#define MAP

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
#include "player.h"
#include "mylib.h"
#include "gameplay.h"

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

struct position{
	int x;
	int y;
};
struct player;

struct board{
	char** map;
	struct position up_left_corner;
	int x;
	int y;
	int changed;
	struct position p1;
	struct position p2;
};
// typedef struct board board;
void map_init(struct board* tab,char* map);

void launch_game(char* folder);

void print_map(struct board* map,struct player *p1, struct player *p2);

void print_player(struct player *p,struct board *map);

void del_board(struct board *map);

#endif
