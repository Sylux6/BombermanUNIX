#include "gameplay.h"

void mainGame(player p1, player p2, board map){
		char buff[1];
		struct pollfd act[1];
		act[0].fd = 0;
		act[0].events = POLLIN;
		
		int timeout = 100;
		struct itimerval start;
		struct itimerval other;

		int time_left=0;
		int milliS=0;

		while(p1->life >0 && p2->life >0) {

			milliS = time_poll(&start,act,1,timeout);
			init_timer(&other);
			
			if(act->revents & POLLIN) 
				read(0 , buff , 1);
			else
				*buff = 0;
			*buff = code_action(*buff);
			updateData(milliS+time_left,p1,p2,map->listBombs);
			

			if(*buff-5 <= 0){
				if(p1->wait <=0 && *buff != 5)
					do_action(*buff,p1,map);
				else if(*buff == 5){
					do_action(*buff,p1,map);
				}
			}else{
				if(p2->wait <= 0 && *buff-5 != 5)
					do_action(*buff-5,p2,map);
				else if(*buff-5 == 5)
					do_action(*buff-5,p2,map);
			}
			// if(nextEvent(p1,p2,map->listBombs) == -1){
			// 	timeout = 100;
			// }else{
			// 	timeout = (nextEvent(p1,p2,map->listBombs) > 100)? 100 : nextEvent(p1,p2,map->listBombs);
			// }
			// if(!map->refresh)
			timeout = nextEvent(p1,p2,map->listBombs);
			// else{
			// 	timeout = 1;
			// 	map->refresh = 0;
			// }
			print_map(map,p1,p2);
			// printf(CLEAR_BEFOR_CUR);
			print_number(timeout,3,6 );
			is_touch(p1,p2,map);

			time_left = get_timer(&other);
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
	return action;
}
char do_action(char action, player p, board map){
	if(action != 5 ){
		return tryMove(action,p,map);
	}else {
		return tryDropBombe(p,map);
	}
}

int tryMove(char direction, player p, board map){
	int x = p->pos.x;
	int y = p->pos.y;
	switch(direction){
		case 1://up
			if(isPassable(map,x-1,y)){
				map->map[p->pos.x][p->pos.y] = ' ';

				p->pos.x--;
				map->map[p->pos.x][p->pos.y] = ' ';
				lootPowerup(p,&map->powerups[p->pos.x][p->pos.y]);

				p->wait = p->speed;
				return 1;
			}
			break;
		case 2://down
			if(isPassable(map,x+1,y)){
				map->map[p->pos.x][p->pos.y] = ' ';

				p->pos.x++;	
				p->wait = p->speed;
				map->map[p->pos.x][p->pos.y] = ' ';

				lootPowerup(p,&map->powerups[p->pos.x][p->pos.y]);
				return 1;
			}

			break;
		case 3://right
			if(isPassable(map,x,y+1)){
				map->map[p->pos.x][p->pos.y] = ' ';

				p->pos.y++;
				p->wait = p->speed;
				map->map[p->pos.x][p->pos.y] = ' ';

				lootPowerup(p,&map->powerups[p->pos.x][p->pos.y]);
				return 1;
			}
			break;
		case 4://left
			if(isPassable(map,x,y-1)){
				map->map[p->pos.x][p->pos.y] = ' ';

				p->pos.y--;
				p->wait = p->speed;
				map->map[p->pos.x][p->pos.y] = ' ';
				lootPowerup(p,&map->powerups[p->pos.x][p->pos.y]);
				return 1;
			}
			break;
	}
	return 0;
}

int tryDropBombe(player p,board map){
	int launch = addBombToList(map->listBombs,createBomb(p));
	if(launch == 1)
		map->map[p->pos.x][p->pos.y] = '@';
	return launch;
}

int isPassable(board map,int x,int y){
	if((map->map[x][y] == ' ' || map->map[x][y] == 'X')&& !isBomb(map->listBombs,x,y)){
		return 1;
	}
	return 0;
}


int time_poll(struct itimerval *start,struct pollfd *act,int nb,int timeout){
	int milliS;
	init_timer(start);
	poll(act,nb,timeout);
	milliS = get_timer(start);
	return milliS;
}

void updateData(int milliS,player p1,player p2,listBomb l){
	updateTimer(l, milliS);
	// p1->wait -= milliS;
	// p2->wait -= milliS;
	
	p1->wait = (isNeg(p1->wait-milliS))?-1:p1->wait - milliS;
	p2->wait = (isNeg(p2->wait-milliS))?-1:p2->wait - milliS;
	
	if(p1->invinsible == 1){
		p1->invinsible_time -= milliS;
		if(p1->invinsible_time <= 0){
			p1->invinsible_time = -1;
			p1->invinsible = 0;
			strcpy(p1->effet,NORMAL); 
		}
	}

	if(p2->invinsible == 1){
		p2->invinsible_time -= milliS;
		if(p2->invinsible_time <= 0){
			p2->invinsible_time = -1;
			p2->invinsible = 0;
			strcpy(p2->effet,NORMAL); 
		}
	}
}

void init_timer(struct itimerval *timer){
	timer->it_value.tv_sec = 60;
	timer->it_value.tv_usec = 0;

	timer->it_interval.tv_sec = 0;
	timer->it_interval.tv_usec = 0;

	setitimer(ITIMER_REAL,timer,NULL);
}

int get_timer(struct itimerval *timer){
	int microS,seconde,milliS;
	getitimer(ITIMER_REAL, timer);	
	seconde = 59-timer->it_value.tv_sec;
	microS = 1000000-timer->it_value.tv_usec + seconde*1000000;
	milliS = microS/1000;
	return milliS;	
}


int nextEvent( player p1, player p2, listBomb l){
	int event;
	int bombsT = nextBombEvent(l);
	int playerT = endOfInvinsibility(p1,p2);
	event = MIN(bombsT,playerT);
	if(event == -1){
		event = MAX(bombsT,playerT);
	}
	return event;
}