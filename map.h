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

void print_map(struct board* map, const struct player p1, const struct player p2);

void print_player(const struct player p,struct board *map);

void del_board(struct board *map);

#endif
