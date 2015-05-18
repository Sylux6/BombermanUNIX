#include "powerup.h"

struct powerup createPowerup(const int type, int value) {
	struct powerup newPowerup;
	newPowerup.type = type;
	newPowerup.value = value;
	switch(type) {
		case EMPTY:
			newPowerup.symbol = ' '; 
			break;
		case SPEED:
			newPowerup.symbol = '+';
			break;
		case BOMB_RADIUS:
			newPowerup.symbol = '*';
			break;
		case BOMB_MAX:
			newPowerup.symbol = '@';
			break;
		default:
			newPowerup.symbol = ' ';
			break;
	}
	return newPowerup;
}

void lootPowerup(struct player* p, struct powerup* pow) {
	switch(pow->type) {
		case SPEED:
			upgradeSpeed(p,pow->value);
			break;
		case BOMB_RADIUS:
			upgradeRadius(p,pow->value);
			break;
		case BOMB_MAX:
			upgradeNumberBomb(p,pow->value);
			break;
	}
	destroyPowerup(pow);
}

void destroyPowerup(struct powerup* pow){
	pow->type = EMPTY;
	pow->symbol = ' ';
	pow->value = 0;
}