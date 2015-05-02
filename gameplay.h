#ifndef _GAMEPLAY_INCLUDED
#define _GAMEPLAY_INCLUDED

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/time.h>

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

void mainGame(/*char* rand_name, */struct player *p1, struct player *p2, struct board *map);

char code_action(int action);

char do_action(char action, struct player *p,struct player *other, struct board *map);

int tryMove(char action, struct player *p,struct player *other, struct board *map); // return 1 if ok 0 else

int tryDropBombe(struct player *p,struct board *map); // return 1 if ok 0 else

int isBomb(struct player *p,struct player *p_,int x,int y);

int isEmpty(struct player *p,struct player *p_, struct board *map,int x,int y);

int time_poll(struct itimerval start,struct pollfd *act,int nb,int timeout);
#endif
