#ifndef _POWERUP_INCLUDED
#define _POWERUP_INCLUDED

#include <stdlib.h>

#include "player.h"

#define EMPTY 0
// #define INVULNERABILITY 1
#define SPEED 2
#define BOMB_RADIUS 3
#define BOMB_MAX 4

struct powerup {
	int type;
	char symbol;
	int value;
};
struct player;
// typedef struct powerup* powerup;

struct powerup createPowerup(const int, int);

void lootPowerup(struct player* p, struct powerup* pow);

void destroyPowerup(struct powerup* pow);

#endif