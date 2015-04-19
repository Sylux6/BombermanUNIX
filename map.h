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


struct board{
	char** map;
	int x;
	int y;
	int changed;
};
typedef struct board board;
void map_init(board* tab,char* map,int x,int y);

void launch_game(char* folder);

void print_map(board* map,int x,int y);

void del_board(board *map);

#endif
