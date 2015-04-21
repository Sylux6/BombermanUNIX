#ifndef _GAMEPLAY_INCLUDED
#define _GAMEPLAY_INCLUDED

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <poll.h>

#include "term.h"
#include "player.h"
#include "map.h"
#include "mylib.h"

#define P1_UP 122
#define P1_DOWN 115
#define P1_RIGHT 100
#define P1_LEFT 113
#define P1_BOMB 97

#define P2_UP 65
#define P2_DOWN 66
#define P2_RIGHT 67
#define P2_LEFT 68
#define P2_BOMB 70

struct player;
struct board;
char* initGameplay();

char *random_named_pipe();

void mainGame(char* rand_name, struct player *p1, struct player *p2, struct board *map);

void tryMove(struct player *p,struct board *map);

#endif
