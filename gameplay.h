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
#include <time.h>
#include <signal.h>

#include "term.h"
#include "bomb.h"
#include "player.h"
#include "map.h"
#include "mylib.h"


#define MAX(x,y) (x>y)?x:y

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

typedef struct player* player;
typedef struct board* board;
typedef struct listBomb* listBomb;

void mainGame(player p1, player p2, board map, listBomb bombs);

char code_action(int action);

char do_action(char action, player p, listBomb l, board map);

int tryMove(char direction, player p,listBomb l, board map); // return 1 if ok 0 else

int tryDropBombe(player p,listBomb l,board map); // return 1 if ok 0 else


int isPassable(board map,listBomb l,int x,int y);

int time_poll(struct itimerval *start,struct pollfd *act,int nb,int timeout);

void updateData(int milliS,player p1,player p2,board map,listBomb l);

int get_timer(struct itimerval *timer);

void init_timer(struct itimerval *timer);

int nextEvent(player,player,listBomb);

#endif
