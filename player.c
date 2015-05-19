#include "player.h"

player create_player(int nb){
	player p = malloc(sizeof(struct player));
	p->nb = nb;
	p->name = malloc(10);
	//let him choose his name (later)
	sprintf(p->name,"Player%d",nb);
	p->pos.x = 0;
	p->pos.y = 0;
	
	p->view = malloc(2);
	strcpy(p->view,"P");
	
	p->color = malloc(12);
	if(nb == 1){
		p->color = YELLOW; 
	}else{
		p->color = CYAN; 
	}
	p->effet = malloc(12);
	strcpy(p->effet,NORMAL); 

	p->life = 3;
	//about bomb
	p->nb_bomb = 1;
	p->nb_bomb_set = 0;
	p->radius_bomb = RADIUS;

	//about move
	p->speed = 400;
	p->wait = 0;

	//about bonus
	p->invinsible = 0;
	p->invinsible_time = 0;
	return p;
}

void upgradeNumberBomb(struct player* p,int value){
	p->nb_bomb += value;
}
void upgradeRadius(struct player* p,int value){
	p->radius_bomb += value;
}
void upgradeSpeed(struct player* p,int value){
	p->speed += value;
}
void spawn(player p1, player p2, board map){
	int randX, randY, area = 0, i, j, ok = 0;
	char** tmp = malloc(sizeof(char*)*map->x);
	for(i = 0; i < map->x; i++)
		tmp[i] = malloc(sizeof(char)*map->y);
	do{
		do {
			randX = my_rand(1, map->x-1);
			randY = my_rand(1, map->y-1);
			if(map->powerups[randX][randY].type != EMPTY)
				continue;
			for(i = 0; i < map->x; i++)
				for(j = 0; j < map->y; j++) {
					if(map->map[i][j] != ' ')
						tmp[i][j] = 1;
					else
						tmp[i][j] = 0;
				}
			area = area_calcul(tmp, map, randX, randY);
		}
		while(area <= p1->radius_bomb + 2);
		map->map[randX][randY] = ' ';//*(p->view);
		p1->pos.x = randX;
		p1->pos.y = randY;


		do {
			randX = my_rand(0, map->x-1);
			randY = my_rand(0, map->y-1);
			if(map->powerups[randX][randY].type != EMPTY)
				continue;
			for(i = 0; i < map->x; i++)
				for(j = 0; j < map->y; j++) {
					if(map->map[i][j] != ' ')
						tmp[i][j] = 1;
					else
						tmp[i][j] = 0;
				}
			area = area_calcul(tmp, map, randX, randY);
		}
		while(area <= p2->radius_bomb + 2);
		
		for(i = 0; i < map->x; i++)
			free(tmp[i]);
		free(tmp);
		print_number(area, 2, 40);
		
		map->map[randX][randY] = ' ';//*(p->view);
		p2->pos.x = randX;
		p2->pos.y = randY;
		if((p1->pos.x == p2->pos.x && p1->pos.y == p2->pos.y) || spawnValide(p1->pos.x, p1->pos.y, p2->pos.x, p2->pos.y, map))
			ok=1;
	}while(ok != 1);
}



int spawnValide(int x,int y,int x_f,int y_f,board map){
	int i, j;
	char **tmp = malloc(sizeof(char*)*map->x);
	for(i = 0; i < map->x; i++)
		tmp[i] = malloc(sizeof(char)*map->y);
	for(i = 0; i < map->x; i++)
		for(j = 0; j < map->y; j++)
			tmp[i][j] = 0;
	if(check_path(tmp, map, x, y, x_f, y_f)) {
		for(i = 0; i < map->x; i++)
			free(tmp[i]);
		free(tmp);
		return 1;
	}
	for(i = 0; i < map->x; i++)
		free(tmp[i]);
	free(tmp);
	return 0;
}
void is_touch(player p1, player p2, board map){
	//si p1 ou p2 est dans une range de bombe en phase 3 , il perd une vie


	listBomb c = map->listBombs->next;

	while(c != NULL){
		if(isEnded(c->bomb)){
			in_explode(p1, p2, c->bomb,map);
		}	

		c = c->next;
	}
}

void in_explode(player p1, player p2 , bomb b, board map){

	// a change a cause du fait que map ne contient plus les 'P'
	int i;
	int x = b->x;
	int y = b->y;
	for(i=0; i<=b->owner->radius_bomb ; i++){
		
		if(map->map[x-i][y] == ' ' || map->map[x-i][y] == 'X'){
			if((p1->pos.x == x-i && p1->pos.y == y) && p1->invinsible == 0){
				p1->life--;
				setInvinsibility(p1);
				strcpy(p1->effet,BLINK);
			}
			if((p2->pos.x == x-i && p2->pos.y == y) && p2->invinsible == 0){
				p2->life--;
				setInvinsibility(p2);
				strcpy(p2->effet,BLINK);	
			}
		}else{
			break;
		}
	}

	for(i=1; i<=b->owner->radius_bomb ; i++){
		if(map->map[x+i][y] == ' ' || map->map[x+i][y] == 'X'){
			if((p1->pos.x == x+i && p1->pos.y == y) && p1->invinsible == 0){
				p1->life--;
				
				setInvinsibility(p1);
				strcpy(p1->effet,BLINK);
			}
			if((p2->pos.x == x+i && p2->pos.y == y) && p2->invinsible == 0){
				p2->life--;
				setInvinsibility(p2);
				strcpy(p2->effet,BLINK);
			}
		}else{
			break;
		}
	}

	for(i=1; i<=b->owner->radius_bomb ; i++){
		if(map->map[x][y-i] == ' ' || map->map[x][y-i] == 'X'){
			if((p1->pos.x == x && p1->pos.y == y-i) && p1->invinsible == 0){
				p1->life--;
				setInvinsibility(p1);
				strcpy(p1->effet,BLINK);
			}
			if((p2->pos.x == x && p2->pos.y == y-i) && p2->invinsible == 0){
				p2->life--;
				setInvinsibility(p2);
				strcpy(p2->effet,BLINK);
			}
		}else{
			break;
		}
	}

	for(i=1; i<=b->owner->radius_bomb ; i++){
		if(map->map[x][y+i] == ' '|| map->map[x][y+i] == 'X'){
			if((p1->pos.x == x && p1->pos.y == y+i) && p1->invinsible == 0){
				p1->life--;
				setInvinsibility(p1);
				strcpy(p1->effet,BLINK);
			}
			if((p2->pos.x == x && p2->pos.y == y+i) && p2->invinsible == 0){
				p2->life--;
				setInvinsibility(p2);
				strcpy(p2->effet,BLINK);
			}
		}else{
			break;
		}
	}

}

int endOfInvinsibility(player p1,player p2){
	int next = GOD_TIME + 1;

	if(p1->invinsible == 1 && next > p1->invinsible_time){
		next = p1->invinsible_time;
	}

	if(p2->invinsible == 1 && next > p2->invinsible_time){
		next = p2->invinsible_time;
	}
	if(next != GOD_TIME+1 && next > 0)
		return next;
	else
		return -1;
}

void setInvinsibility(player p){
	p->invinsible = 1;
	p->invinsible_time = GOD_TIME;
}