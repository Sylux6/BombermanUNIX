#include "gameplay.h"

char* random_named_pipe() {
	srand(time(NULL));
	size_t len = rand() % 10 + 4;	//Generates a random number between 4 and 13
	char *random_name = malloc(len * sizeof(char) + 2);
	random_name[0] = 'b';
	int i;
	for(i = 0; i < len; i++) {
		random_name[i+1] = rand() % 10 + 48;	//Generates a random char value between '0' and '9'
	}
	random_name[i+1] = '\0';

	return random_name;
}

void initGameplay() {

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

	int fd_players = open(rand_name, O_RDONLY);
	if(fd_players == -1) {
		perror(rand_name);
		return;
	}

	//PLAYER 1
	if(!fork()) {
		int fd_p1 = open(rand_name, O_WRONLY);
		if(fd_players == -1) {
			perror(rand_name);
			return;
		}
	}
	//PLAYER 2
	else if(!fork()) {
		int fd_p2 = open(rand_name, O_WRONLY);
		if(fd_players == -1) {
			perror(rand_name);
			return;
		}
	}
	else {

	}
}
