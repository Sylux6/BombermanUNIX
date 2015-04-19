#include "player.h"

player create_player(int nb){
	player p;
	
	p.name = malloc(3);
	sprintf(p.name,"p%d",nb);
	p.pos.x = 0;
	p.pos.y = 0;
	p.view = malloc(2);
	p.view = "p"; 

	p.life = 3;
	p.nb_bombe = 1;
	p.speed = 10;
	p.range_boom = 4;

	return p;
}


void spawn(player *p,int pos_x,int pos_y,int width,int length){

	int randX = my_rand(0,width);
	int randY = my_rand(0,length);
	int bool = 1;
	// tand que la case est non vide 
	char* buff = malloc(2);
	
	// printf("la map fait %d - %d \n",width,length );

	do{
		set_pos(pos_x+randX,pos_y+randY);
		my_print_err("on lis le caractere\n");
		if(read(0,buff,1) <= 0){// cette lecture ne fonctionne pas // faire avec un tableau (plus chiant)
			my_print_err("lecture position impossible");
			exit(-1);
		}
		set_pos(15,4);
		write(1,buff,1 );
		if(*buff == ' '){
			bool = 0;
			// my_print_err("good");
		}else{
			// my_print_err("err");
			randX = my_rand(0,width);
			randY = my_rand(0,length);
		}

	}while(bool);
	free(buff);
	p->pos.x = pos_x + randX;
	p->pos.y = pos_y + randY;
		set_pos(15,4);
	printf("x->%d , y->%d\n",randX,randY );
	// sleep(2);
}