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
	p->nb_bomb = 2;
	p->nb_bomb_set = 0;
	p->radius_bomb = 1;

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
void upgradeRaduis(struct player* p,int value){
	p->radius_bomb += value;
}
void upgradeSpeed(struct player* p,int value){
	p->speed += value;
}
void spawn(struct player *p, struct board *map){

	int randX = my_rand(0,map->x-1);
	int randY = my_rand(0,map->y-1);
	int bool = 1;
	// tand que la case est non vide 
	do{
		if(map->map[randX][randY] == ' '){
			bool = 0;
		}else{
			randX = my_rand(0,map->x-1);
			randY = my_rand(0,map->y-1);
		}

	}while(bool);
	map->map[randX][randY] = *(p->view);
	p->pos.x = randX;
	p->pos.y = randY;
}
/*

void clear_range_bomb(int x, int y, struct player p, struct board *map){
	int i = 1;
	map->map[x][y] = ' ';

	do{ //NORTH
		if(map->map[x-i][y] == 'X')
			map->map[x-i][y] = ' ';
		else
			break;
		i++;
	}while(i <= p.radius_bomb);
	i = 1;


	do{//SOUTH
		
		if(map->map[x+i][y] == 'X')
			map->map[x+i][y] = ' ';
		else
			break;
		i++;
	}while(i <= p.radius_bomb);
	i = 1;

	do{//WEST
		if(map->map[x][y-i] == 'X')
			map->map[x][y-i] = ' ';
		else
			break;
		i++;
	}while(i <= p.radius_bomb);
	i = 1;

	do{//EAST
		if(map->map[x][y+i] == 'X')
			map->map[x][y+i] = ' ';
		else
			break;
		i++;
	}while(i <= p.radius_bomb);

}	

void is_touch(player p1, player p2, board map, listBomb l){
	//si p1 ou p2 est dans une range de bombe en phase 3 , il perd une vie


	listBomb c = l->next;

	while(c != NULL){
		if(isEnded(c->bomb)){
			in_explode(p1, p2, c->bomb,map);
		}	

		c = c->next;
	}
	// for(int i = 0 ; i < p1->nb_bomb ; i++){
	// 	if(p1->bomb_own[i].state == 3){
	// 		in_explode(p1,p2,p1->bomb_own[i].x,p1->bomb_own[i].y,p1->radius_bomb,map);
	// 	}
	// }
	// for(int i = 0 ; i < p2->nb_bomb ; i++){
	// 	if(p2->bomb_own[i].state == 3){
	// 		in_explode(p1,p2,p2->bomb_own[i].x,p2->bomb_own[i].y,p2->radius_bomb,map);
	// 	}
	// }
}

void in_explode(player p1, player p2 , bomb b, board map){
	int i;

	for(i=0; i<=range ; i++){
		
		if(map->map[x-i][y] == 'P' || map->map[x-i][y] == 'X'){
			if((p1->pos.x == x-i && p1->pos.y == y) && p1->invinsible == 0){
				p1->life--;
				p1->invinsible = 1;
				p1->invinsible_time = 2000;
				strcpy(p1->effet,BLINK);
			}
			if((p2->pos.x == x-i && p2->pos.y == y) && p2->invinsible == 0){
				p2->life--;
				p2->invinsible = 1;
				p2->invinsible_time = 2000;
				strcpy(p2->effet,BLINK);	
			}
		}else{
			break;
		}
	}

	for(i=1; i<=range ; i++){
		if(map->map[x+i][y] == 'P' || map->map[x+i][y] == 'X'){
			if((p1->pos.x == x+i && p1->pos.y == y) && p1->invinsible == 0){
				p1->life--;
				p1->invinsible = 1;
				p1->invinsible_time = 2000;
				strcpy(p1->effet,BLINK);
			}
			if((p2->pos.x == x+i && p2->pos.y == y) && p2->invinsible == 0){
				p2->life--;
				p2->invinsible = 1;
				p2->invinsible_time = 2000;
				strcpy(p2->effet,BLINK);
			}
		}else{
			break;
		}
	}

	for(i=1; i<=range ; i++){
		if(map->map[x][y-i] == 'P' || map->map[x][y-i] == 'X'){
			if((p1->pos.x == x && p1->pos.y == y-i) && p1->invinsible == 0){
				p1->life--;
				p1->invinsible = 1;
				p1->invinsible_time = 2000;
				strcpy(p1->effet,BLINK);
			}
			if((p2->pos.x == x && p2->pos.y == y-i) && p2->invinsible == 0){
				p2->life--;
				p2->invinsible = 1;
				p2->invinsible_time = 2000;
				strcpy(p2->effet,BLINK);
			}
		}else{
			break;
		}
	}

	for(i=1; i<=range ; i++){
		if(map->map[x][y+i] == 'P'|| map->map[x][y+i] == 'X'){
			if((p1->pos.x == x && p1->pos.y == y+i) && p1->invinsible == 0){
				p1->life--;
				p1->invinsible = 1;
				p1->invinsible_time = 2000;
				strcpy(p1->effet,BLINK);
			}
			if((p2->pos.x == x && p2->pos.y == y+i) && p2->invinsible == 0){
				p2->life--;
				p2->invinsible = 1;
				p2->invinsible_time = 2000;
				strcpy(p2->effet,BLINK);
			}
		}else{
			break;
		}
	}

}
*/
int endOfInvinsibility(player p1,player p2){
	int next = GOD_TIME + 1;

	if(p1->invinsible == 1 && next > p1->invinsible_time){
		next = p1->invinsible_time;
	}

	if(p2->invinsible == 1 && next > p2->invinsible_time){
		next = p2->invinsible_time;
	}
	if(next != GOD_TIME+1)
		return next;
	else
		return -1;
}

void setInvinsibility(player p){
	p->invinsible = 1;
	p->invinsible_time = GOD_TIME;
}