#ifndef _BOMB_INCLUDED
#define _BOMB_INCLUDED

#include <stdlib.h>

#include "player.h"

#define NONE 0
#define SET 1
#define EXPLODING 2

#define BOMB_TIME 2500 //milliseconds
#define BOMB_EXPLODE 333 //milliseconds 

typedef struct player *player;

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

bomb createBomb(player owner, int state);

bomb changeState(bomb b, int state);

listBomb initList();

void addBombToList(listBomb l, bomb b);

int removeBombFromList(listBomb l, int x, int y);

void removeListBomb(listBomb l);

void updateTimer(listBomb l, int ms);

#endif