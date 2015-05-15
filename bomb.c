#include "bomb.h"

bomb createBomb(player owner, int state) {
	bomb newBomb = malloc(sizeof(struct bomb));
	newBomb->owner = owner;
	if(state == SET) {
		newBomb->state = SET;
		newBomb->time = BOMB_TIME;
		newBomb->time_explode = BOMB_EXPLODE;
	}
	else {
		newBomb->state = NONE;
	}
	return newBomb;
}

bomb changeState(bomb b, int state) {
	switch(state) {
		case NONE:
			free(b);
			return NULL;
		case SET:
			b->state = SET;
			b->time = BOMB_TIME;
			return b;
		case EXPLODING:
			b->state = EXPLODING;
			b->time_explode = BOMB_EXPLODE;
			return b;
	}
	return NULL;
}

listBomb initList() {
	listBomb newList = malloc(sizeof(struct listBomb));
	newList->bomb = NULL;
	newList->next = NULL;
	return newList;
}

void addBombToList(listBomb l, bomb b) {
	listBomb c = l->next;
	while(c != NULL)
		c = c->next;
	c->bomb = b;
}

void removeListBomb(listBomb l) {
	listBomb c = l->next;
	listBomb p;
	while(c != NULL) {
		free(c->bomb);
		p = c;
		c = c->next;
		free(p);
	}
	free(l);
}

//Return 1 on success, 0 if not found
int removeBombFromList(listBomb l, int x, int y) {
	listBomb p = l;
	listBomb c = l->next;
	while(c != NULL) {
		if(c->bomb->x == x && c->bomb->y == y) {
			free(c->bomb);
			p->next = c->next;
			free(c);
			return 1;
		}
		p = c;
		c = c->next;
	}
	return 0;
}

//Met à jour le timer de toutes les bombes actives
void updateTimer(listBomb l, int ms) {
	listBomb c = l->next;
	while(c != NULL) {
		c->bomb->time -= ms;
		c = c->next;
	}
}