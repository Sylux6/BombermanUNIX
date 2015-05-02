#include "player.h"

struct player create_player(int nb){
	player p;
	p.nb = nb;
	p.name = malloc(10);
	//let him choose his name (later)
	sprintf(p.name,"Player%d",nb);
	p.pos.x = 0;
	p.pos.y = 0;
	p.color = malloc(12);
	p.view = malloc(2);
	strcpy(p.view,"P");
	if(nb == 1){
		p.color = "\033[01;33m"; 
	}else{
		p.color = "\033[01;36m"; 
	}
	p.life = 3;
	p.nb_bomb = 1;
	p.speed = 500;
	p.wait = 0;
	p.radius_bomb = 1;
	// p.bomb_max = 10;
	p.bomb_own = malloc(sizeof(struct bomb)*p.nb_bomb);
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