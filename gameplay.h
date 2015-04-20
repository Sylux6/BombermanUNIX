#ifndef _GAMEPLAY_INCLUDED
#define _GAMEPLAY_INCLUDED

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>

#include "term.h"
#include "player.h"
#include "map.h"
#include "mylib.h"
struct player;
struct board;
char* initGameplay();

char *random_named_pipe();

void mainGame(char* rand_name, struct player *p1, struct player *p2, struct board *map);

void tryMove(struct player *p,struct board *map);

#endif
