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
			b->state = NONE;
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
		listBomb add = malloc(sizeof(struct listBomb));
		add->bomb = b;
		add->next = NULL;

		b->owner->nb_bomb_set++;
		listBomb p = l;
		listBomb c = l->next;
		while(c != NULL){
			p = c;
			c = c->next;
		}
		p->next = add;

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
			// if(c->bomb->time_explode <= 0){
			// 	changeState(c->bomb,NONE);
			// }
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
			free(c);
			c = p->next;
		}else{
			p = c;
			c = c->next;	
		}
	}
}

int isBomb(listBomb l,int x,int y){
	listBomb c = l->next
	;
	while(c != NULL) {
		if(c->bomb->x == x && c->bomb->y == y)
			return 1;
		c = c->next;
	}
	return 0;	
}

int isSet(bomb b){
	return (b->state == SET);
}
int isEnded(bomb b){
	return (b->state == ENDED);
}
int isExploding(bomb b){
	return (b->state == EXPLODING);
}

void explode(bomb b,board map){
	//a faire
	int x = b->x;
	int y = b->y;
	// char* X = "X";
	// map->map[x][y] = *X;//center
	// print_line_(X,3,map->up_left_corner.x + x , map->up_left_corner.y + y,RED);	
	map->map[x][y] = 'X';
	int continue_ = 1;
	int i = 0;
	do{ //NORTH
		switch(map->map[x-i][y]){	
			
			case '1':
				continue_ = 0;
				// map->map[x-i][y] = 'X';
				map->map[x-i][y] = ' ';
				break;
			case ' ':
				destroyPowerup(&(map->powerups[x-i][y]));// map->powerups[x-i][y].symbol = 
				map->map[x-i][y] = 'X';
				break;
			case 'X':
				break;
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				map->map[x-i][y]--;
			case '0':
				continue_ = 0;
				break;
			// default:

		}
		i++;
	}while(i <= b->owner->radius_bomb && continue_);
	i = 0;
	continue_ = 1;


	do{ //SOUTH
		switch(map->map[x+i][y]){	
			
			case '1':
				continue_ = 0;
				map->map[x+i][y] = ' ';

				break;
			case ' ':
				destroyPowerup(&(map->powerups[x+i][y]));
				map->map[x+i][y] = 'X';
				break;
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				map->map[x+i][y]--;
			case '0':
				continue_ = 0;
				break;
			// default:
		}
		i++;
	}while(i <= b->owner->radius_bomb && continue_);
	i = 0;
	continue_ = 1;

	do{ //WEST
		switch(map->map[x][y-i]){	
		
			case '1':
				continue_ = 0;
				map->map[x][y-i] = ' ';
				break;
			case ' ':
				destroyPowerup(&(map->powerups[x][y-i]));
				map->map[x][y-i] = 'X';
				break;
			case 'X':
				break;
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				map->map[x][y-i]--;
			case '0':
				continue_ = 0;
				break;
			// default:
			

		}
		i++;
	}while(i <= b->owner->radius_bomb && continue_);
	i = 0;
	continue_ = 1;

	do{ //EAST
		switch(map->map[x][y+i]){	
			
			case '1':
				continue_ = 0;
				map->map[x][y+i] = ' ';
				break;
			case ' ':
				destroyPowerup(&(map->powerups[x][y+i]));
				map->map[x][y+i] = 'X';
				break;
			case 'X':
				break;
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				map->map[x][y+i]--;
			case '0':
				continue_ = 0;
				break;
			// default:
			

		}
		i++;
	}while(i <= b->owner->radius_bomb && continue_);


}

void clear_bomb(bomb b,board map){
	int i = 1;
	int x = b->x;
	int y = b->y;
	map->map[x][y] = ' ';

	do{ //NORTH
		if(map->map[x-i][y] == 'X')
			map->map[x-i][y] = ' ';
		else
			break;
		i++;
	}while(i <= b->owner->radius_bomb);
	i = 1;


	do{//SOUTH
		
		if(map->map[x+i][y] == 'X')
			map->map[x+i][y] = ' ';
		else
			break;
		i++;
	}while(i <= b->owner->radius_bomb);
	i = 1;

	do{//WEST
		if(map->map[x][y-i] == 'X')
			map->map[x][y-i] = ' ';
		else
			break;
		i++;
	}while(i <= b->owner->radius_bomb);
	i = 1;

	do{//EAST
		if(map->map[x][y+i] == 'X')
			map->map[x][y+i] = ' ';
		else
			break;
		i++;
	}while(i <= b->owner->radius_bomb);
}