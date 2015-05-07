#include "player.h"

struct player create_player(int nb){
	player p;
	p.nb = nb;
	p.name = malloc(10);
	//let him choose his name (later)
	sprintf(p.name,"Player%d",nb);
	p.pos.x = 0;
	p.pos.y = 0;
	
	p.view = malloc(2);
	strcpy(p.view,"P");
	
	p.color = malloc(12);
	if(nb == 1){
		p.color = YELLOW; 
	}else{
		p.color = CYAN; 
	}
	p.effet = malloc(12);
	strcpy(p.effet,NORMAL); 

	p.life = 3;
	p.nb_bomb = 1;
	p.speed = 500;
	p.wait = 0;
	p.invinsible = 0;
	p.invinsible_time = 0;
	p.radius_bomb = 2;
	p.bomb_own = malloc(sizeof(struct bomb)*p.nb_bomb);
		for(int i = 0 ; i < p.nb_bomb ; i++){
		p.bomb_own[i].state = 0;
		p.bomb_own[i].time = -1;
		// p.bomb_own[i].time_explode = -1;
		p.bomb_own[i].x = 0;
		p.bomb_own[i].y = 0;

	}
	return p;
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
	map->changed = 1;
}


void explode(int x, int y, struct player p, struct board *map){
	//make the explosion
	int i = 0;
	map->map[x][y] = 'X';
	int continue_ = 1;
	do{ //NORTH
		switch(map->map[x-i][y]){	
			
			case '1':
				continue_ = 0;
			case ' ':
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

		}
		i++;
	}while(i <= p.radius_bomb && continue_);
	i = 0;
	continue_ = 1;

	do{ //SOUTH
		switch(map->map[x+i][y]){	
			
			case '1':
				continue_ = 0;
			case ' ':
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
		}
		i++;
	}while(i <= p.radius_bomb && continue_);
	i = 0;
	continue_ = 1;

	do{ //WEST
		switch(map->map[x][y-i]){	
		
			case '1':
				continue_ = 0;
			case ' ':
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
			

		}
		i++;
	}while(i <= p.radius_bomb && continue_);
	i = 0;
	continue_ = 1;

	do{ //EAST
		switch(map->map[x][y+i]){	
			
			case '1':
				continue_ = 0;
			case ' ':
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
			

		}
		i++;
	}while(i <= p.radius_bomb && continue_);

}

void clear_range_bomb(int x, int y, struct player p, struct board *map){
	int i = 1;
	map->map[x][y] = ' ';

	do{ //NORTH
		if(map->map[x-i][y] == 'X')
			map->map[x-i][y] = ' ';
		i++;
	}while(i <= p.radius_bomb);
	i = 1;


	do{//SOUTH
		
		if(map->map[x+i][y] == 'X')
			map->map[x+i][y] = ' ';
		i++;
	}while(i <= p.radius_bomb);
	i = 1;

	do{//WEST
		if(map->map[x][y-i] == 'X')
			map->map[x][y-i] = ' ';
		i++;
	}while(i <= p.radius_bomb);
	i = 1;

	do{//EAST
		if(map->map[x][y+i] == 'X'){
			map->map[x][y+i] = ' ';
		}
		i++;
	}while(i <= p.radius_bomb);

}	

void is_touch(struct player *p1, struct player *p2,struct board *map){
	//si p1 ou p2 est dans une range de bombe en phase 3 , il perd une vie

	for(int i = 0 ; i < p1->nb_bomb ; i++){
		if(p1->bomb_own[i].state == 3){
			in_explode(p1,p2,p1->bomb_own[i].x,p1->bomb_own[i].y,p1->radius_bomb,map);
		}
	}
	for(int i = 0 ; i < p2->nb_bomb ; i++){
		if(p2->bomb_own[i].state == 3){
			in_explode(p1,p2,p2->bomb_own[i].x,p2->bomb_own[i].y,p2->radius_bomb,map);
		}
	}
}

void in_explode(struct player *p1,struct player *p2 ,int x,int y,int range,struct board *map){
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