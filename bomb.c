#include "bomb.h"

bomb createBomb(player owner) {
	if(owner->nb_bomb_set < owner->nb_bomb){
		bomb newBomb = malloc(sizeof(struct bomb));
		newBomb->owner = owner;
		newBomb->x = owner->pos.x;
		newBomb->y = owner->pos.y;
		newBomb->state = SET;
		newBomb->time = BOMB_TIME;
		newBomb->time_explode = BOMB_EXPLODE;
		return newBomb;
	}else{
		return NULL;
	}
}

bomb changeState(bomb b, int state) {
	switch(state) {
		case NONE:
			// free(b);
			return NULL;
		case SET:
			b->state = SET;
			b->time = BOMB_TIME;
			return b;
		case EXPLODING:
			b->state = EXPLODING;
			b->time_explode = BOMB_EXPLODE;
			return b;
		case ENDED:
			b->state = ENDED;
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

int addBombToList(listBomb l, bomb b) {
	if(b != NULL){
		b->owner->nb_bomb_set++;

		listBomb c = l->next;
		while(c != NULL){
			c = c->next;
			if(c->next == NULL){
				c->next = malloc(sizeof(struct listBomb));
				break;
			}
		}
		c->bomb = b;
		c->next = NULL;
		return 1;
	}
	return 0;
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
		if(c->bomb->state == SET){
			c->bomb->time -= ms;
			if(c->bomb->time <= 0){
				changeState(c->bomb,EXPLODING);
			}
		}else if(c->bomb->state == ENDED){
			c->bomb->time_explode -=ms;
			if(c->bomb->time_explode <= 0)
				changeState(c->bomb,NONE);
		}
		c = c->next;
	}
	deletUseless(l);
}

int nextBombEvent(listBomb l){
	int next = BOMB_TIME + 1;
	listBomb c = l->next;

	while(c != NULL){
		if(c->bomb->state == SET &&  next > c->bomb->time){
			next = c->bomb->time;
		}
		if(c->bomb->state == ENDED && next > c->bomb->time_explode){
			next = c->bomb->time_explode;
		}
		c = c->next;
	}
	if(next != BOMB_TIME+1)
		return next;
	else
		return -1;
}

void deletUseless(listBomb l){
	listBomb p = l;
	listBomb c = l->next;
	while(c != NULL) {
		if(c->bomb->state == NONE) {
			p->next = c->next;

			c->bomb->owner->nb_bomb_set--;
			free(c->bomb);
			c = p;
		}else{
			p = c;
			c = c->next;	
		}
	}
}

int isBomb(listBomb l,int x,int y){
	listBomb c = l->next;
	while(c != NULL) {
		if(c->bomb->x == x && c->bomb->y == y)
			return 1;
		c = c->next;
	}
	return 0;	
}
