#include "gameplay.h"
#include "mylib.h"

char* random_named_pipe() {
	size_t len = my_rand(4, 13);	//Generates a random number between 4 and 13
	char *random_name = malloc(len * sizeof(char) + 2);
	random_name[0] = 'b';
	int i;
	for(i = 0; i < len; i++) {
		random_name[i+1] = my_rand(48, 57);	//Generates a random char value between '0' and '9'
	}
	random_name[i+1] = '\0';

	return random_name;

}

char* initGameplay() {

//METHODE: Utilisation d'1 tube nommé avec 1 lecteur et 2 écrivains

	char *rand_name;
	int ret;
	umask(S_IRGRP | S_IROTH | S_IXUSR | S_IXGRP | S_IXOTH);
	do {
		rand_name = random_named_pipe();
		if((ret = mkfifo(rand_name, S_IXUSR | S_IRUSR | S_IWUSR | S_IWOTH | S_IWGRP)) == -1)
			free(rand_name);
	}
	while(ret == -1);
	return rand_name;
}




void mainGame(/*char* rand_name, */struct player *p1, struct player *p2, struct board *map){
	//PLAYER 1
	int fils1[2];

	// int ret1 = socketpair(AF_UNIX,SOCK_STREAM,0,fils1); ligne a garder , utilisation des soket unix sur une meme machine
	int ret1 = pipe(fils1);

	if( ret1 == -1){
		perror("erreur socket");
		exit(-1);
	}
	// both son send a number for a request to move or drop a bombe
	if(!fork()) {
		char move;
		char to_write;
		while(1){
			read(0, &move, 1);
			if((to_write = code_action(move)) > 0){
				char *carac = malloc(100);
				sprintf(carac,"%d -ecrit\n",to_write);
				my_print_err(carac);
				if(write(fils1[1], &to_write, 1) == -1)
					perror("error communication");
			}
		}
		exit(1);
	}
	else {
		//the father read this number and do the action aproprié
		char buff[100];
		struct pollfd act[1];
		act[0].fd = fils1[0];
		act[0].events = POLLIN;
		int timeout = 100;
		struct itimerval start;
		// struct itimerval end;

		start.it_interval.tv_sec = 0;
		start.it_interval.tv_usec = 0;
		// int time_left;

		int milliS;
		while(1) {

			milliS = time_poll(start,act,1,timeout);
			updateTimeBomb(milliS,p1,p2);
			map->changed = 1;
			if(milliS < timeout) 
				read(fils1[0] , buff , 1);
			else
				*buff = 0;
			p1->wait -= milliS;
			p2->wait -= milliS;

			if(*buff-5 <= 0){
				if(p1->wait <=0)
					do_action(*buff,p1,p2,map);
			}else{
				if(p2->wait <= 0)
					do_action(*buff-5,p2,p1,map);
			}
			print_map(map,*p1,*p2);
		}
	}
}

char code_action(int action){
		switch(action) {
			case P1_UP:
				action = 1;
				break;
			case P1_DOWN:
				action = 2;
				break;
			case P1_RIGHT:
				action = 3;
				break;
			case P1_LEFT:
				action = 4;
				break;
			case P1_BOMB:
				action = 5;
				break;
			case P2_UP:
				action = 6;
				break;
			case P2_DOWN:
				action = 7;
				break;
			case P2_RIGHT:
				action = 8;
				break;
			case P2_LEFT:
				action = 9;
				break;
			case P2_BOMB:
				action = 10;
				break;
			default:
				action = 0;
		}
	// }
	return action;
}
char do_action(char action, struct player *p,struct player *other, struct board *map){
	if(action != 5 ){
		return tryMove(action,p,other,map);
	}else {
		return tryDropBombe(p,map);
	}
}

int tryMove(char direction, struct player *p,struct player *other,struct board *map){
	// a implementer
	int x = p->pos.x;
	int y = p->pos.y;
	switch(direction){
		case 1://up
			if(isEmpty(p,other,map,x-1,y)){
				map->map[p->pos.x][p->pos.y] = ' ';

				p->pos.x--;
				if(p->nb == 1){
					map->p1.x--;
				}else{
					map->p2.x--;
				}
				map->changed = 1;
				map->map[p->pos.x][p->pos.y] = 'P';
				p->wait = 500;
				return 1;
			}
			break;
		case 2://down
			if(isEmpty(p,other,map,x+1,y)){
				map->map[p->pos.x][p->pos.y] = ' ';

				p->pos.x++;	
				if(p->nb == 1){
					map->p1.x++;
				}else{
					map->p2.x++;
				}
				map->changed = 1;
				p->wait = 500;
				map->map[p->pos.x][p->pos.y] = 'P';
				return 1;
			}

			break;
		case 3://right
			if(isEmpty(p,other,map,x,y+1)){
				map->map[p->pos.x][p->pos.y] = ' ';

				p->pos.y++;
				if(p->nb == 1){
					map->p1.y++;
				}else{
					map->p2.y++;
				}
				map->changed = 1;
				p->wait = 500;
				map->map[p->pos.x][p->pos.y] = 'P';
				return 1;
			}
			break;
		case 4://left
			if(isEmpty(p,other,map,x,y-1)){
				map->map[p->pos.x][p->pos.y] = ' ';

				p->pos.y--;
				if(p->nb == 1){
					map->p1.y--;
				}else{
					map->p2.y--;
				}
				map->changed = 1;
				p->wait = 500;
				map->map[p->pos.x][p->pos.y] = 'P';
				return 1;
			}
			break;
	}
	return 0;
}

int tryDropBombe(struct player *p,struct board *map){
	// a implementer aussi ;)
	int i;
	for(i = 0 ; i < p->nb_bomb ; i++){
		if(p->bomb_own[i].state == 0){
			p->bomb_own[i].state = 1;
			p->bomb_own[i].x = p->pos.x;
			p->bomb_own[i].y = p->pos.y;
			p->bomb_own[i].time = 2500;
			map->map[p->pos.x][p->pos.y] = '@';
			return 1;
		}
	}
	return 0;
}

int isEmpty(struct player *p,struct player *p_, struct board *map,int x,int y){// p is le player who want to move
	if(map->map[x][y] == ' ' && !isBomb(p,p_,x,y)){
		return 1;
	}
	return 0;
}

int isBomb(struct player *p,struct player *p_,int x,int y){
	for(int i = 0 ; i < p->nb_bomb ; i++){
		if((p->bomb_own[i].x == x && p->bomb_own[i].y == y) || (p_->bomb_own[i].x == x && p_->bomb_own[i].y == y)){
			return 1;
		}
	}
	return 0;
}

int time_poll(struct itimerval start,struct pollfd *act,int nb,int timeout){
	int microS,seconde,milliS;
	start.it_value.tv_sec = 60;
	start.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &start,NULL);	
	poll(act,nb,timeout);
	getitimer(ITIMER_REAL, &start);	

	seconde = 59-start.it_value.tv_sec;
	microS = 1000000-start.it_value.tv_usec + seconde*1000000;
	milliS = microS/1000;
	return milliS;
}

void updateTimeBomb(int milliS,struct player *p1,struct player *p2){
	for(int i = 0 ; i < p1->nb_bomb ; i++){
		if(p1->bomb_own[i].state == 1){
			p1->bomb_own[i].time -= milliS;
			if(p1->bomb_own[i].time <= 0){

				// BOOOMMMM
				p1->bomb_own[i].state = 2;
			}
		}
	}
	for(int i = 0 ; i < p2->nb_bomb ; i++){
		if(p2->bomb_own[i].state == 1){
			p2->bomb_own[i].time -= milliS;
			if(p2->bomb_own[i].time <= 0){

				// BOOOMMMM
				p2->bomb_own[i].state = 2;
			}
		}
	}
}