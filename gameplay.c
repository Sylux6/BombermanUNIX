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
void mainGame(char* rand_name, struct player *p1, struct player *p2, struct board *map){
	//PLAYER 1
	if(!fork()) {
		int fd_p1 = open(rand_name, O_WRONLY);
		if(fd_p1 == -1) {
			perror(rand_name);
			return;
		}
		char move;
		int ret;
		while(p1->life > 0){//while both player are alive
			if((ret = read(0, &move, 1)) == -1)
				perror("input error");
			else {
				switch(move) {
					case P1_UP:
						move = 1;
						break;
					case P1_DOWN:
						move = 2;
						break;
					case P1_RIGHT:
						move = 3;
						break;
					case P1_LEFT:
						move = 4;
						break;
					case P1_BOMB:
						move = 5;
						break;
					default:
						continue;
				}
				if(write(fd_p1, &move, 1) == -1)
					perror("error communication");
			}
		}
		//envoyer un signal au pere pour qu'il tue l'autre fils
		exit(1);
	}
	//PLAYER 2
	else if(!fork()) {
		int fd_p2 = open(rand_name, O_WRONLY);
		if(fd_p2 == -1) {
			perror(rand_name);
			return;
		}
		char move;;
		int ret;
		while(p2->life > 0){//while both player are alive
			if((ret = read(0, &move, 1)) == -1)
				perror("input error");
			else {
				switch(move) {
					case P2_UP:
						move = 1;
						break;
					case P2_DOWN:
						move = 2;
						break;
					case P2_RIGHT:
						move = 3;
						break;
					case P2_LEFT:
						move = 4;
						break;
					case P2_BOMB:
						move = 5;
						break;
					default:
						continue;
				}
				if(write(fd_p2, &move, 1) == -1)
					perror("error communication");
			}
		}
		//envoyer un signal au pere pour qu'il tue l'autre fils
		exit(1);
	}
	else {
		int ret;
		struct pollfd fd_players;
		fd_players.fd = open(rand_name, O_RDONLY);
		fd_players.events = POLLIN;
		if(fd_players.fd == -1) {
			perror(rand_name);
			return;
		}
		while(1) {//while both player are alive
			//si un fils est mot , tuer l'autre (plus bool = 0)
			ret = poll(&fd_players, 1, -1);
			if(ret > 0) {
				if(fd_players.revents & POLLIN) {
					//TODO
				}
			}
		}

	}
}

void tryMove(struct player *p,struct board *map){
	// a implementer
}