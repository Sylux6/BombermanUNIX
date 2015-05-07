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
typedef struct powerup* powerup;

powerup createPowerup(const int, int);

// int getPowerup(player, powerup);

#endif