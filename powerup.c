#include "powerup.h"

powerup createPowerup(const int type, int value) {
	powerup newPowerup = malloc(sizeof(struct powerup));
	newPowerup->type = type;
	newPowerup->value = value;
	switch(type) {
		case EMPTY:
			newPowerup->symbol = ' ';
			break;
		case SPEED:
			newPowerup->symbol = '+';
			break;
		case BOMB_RADIUS:
			newPowerup->symbol = '*';
			break;
		case BOMB_MAX:
			newPowerup->symbol = '@';
			break;
		default:
			newPowerup->symbol = ' ';
			break;
	}
	return newPowerup;
}

// int getPowerup(player user, powerup pow) {
// 	switch(pow->type) {
// 		case SPEED:
// 			user.speed += pow->value;
// 			break;
// 		case BOMB_RADIUS:
// 			user.radius_bomb += pow->value;
// 			break;
// 		case BOMB_MAX:
// 			user.bomb_max += pow->value;
// 			break;
// 		default:
// 			return EMPTY;
// 	}
// 	return pow->type;
// }