#ifndef _BOMB_INCLUDED
#define _BOMB_INCLUDED

#include <stdlib.h>

#include "player.h"


#define NORTH 	0
#define SOUTH 	1
#define WEST 	2
#define EAST	3

#define NONE 		0
#define SET 		1
#define EXPLODING 	2
#define ENDED		3

#define BOMB_TIME 		2500 //milliseconds
#define BOMB_EXPLODE 	1000 //milliseconds 

typedef struct player *player;
typedef struct board* board;
typedef struct bomb {
    int time; //explose dans time milliseconde 
    int time_explode;
    int state;
    int x;
    int y;
    player owner;
} *bomb;

typedef struct listBomb {
    bomb bomb;
    struct listBomb *next;
} *listBomb;

bomb createBomb(player owner);

bomb changeState(bomb b, int state);

listBomb initList();

int addBombToList(listBomb l, bomb b);

int removeBombFromList(listBomb l, int x, int y);

void removeListBomb(listBomb l);

void updateTimer(listBomb l, int ms);

void deletUseless(listBomb l);

int isBomb(listBomb l,int x,int y);

int nextBombEvent(listBomb l);

void clear_bomb(bomb b,board map);

int isSet(bomb b);

int isEnded(bomb b);

int isExploding(bomb b);

void explode(bomb b,board map);

#endif