#include "powerup.h"

struct powerup createPowerup(const int type, int value) {
	struct powerup newPowerup ;//= malloc(sizeof(struct powerup));
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
			// p->speed +=pow->value;
			upgradeSpeed(p,pow->value);
			break;
		case BOMB_RADIUS:
			// p->radius_bomb +=pow->value;
			upgradeRaduis(p,pow->value);
			break;
		case BOMB_MAX:
			// p->nb_bomb +=pow->value;
			upgradeNumberBomb(p,pow->value);
			break;
		// default:
			//return EMPTY;
	}
	destroyPowerup(pow);
	// return pow->type;
}

void destroyPowerup(struct powerup* pow){
	pow->type = EMPTY;
	pow->symbol = ' ';
	pow->value = 0;
}