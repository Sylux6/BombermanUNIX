#include "map.h"
#include "term.h"

#include "mylib.h"
#define SAV_CUR "\x1b[s"
#define LOAD_CUR "\x1b[u"

#define CLEAR_AFTER_CUR "\x1b[0J"
#define CLEAR_BEFOR_CUR "\x1b[1J"
#define CLEAR_TERM "\x1b[2J\x1b[;H"
#define CUR_ON "\x1b[?25h"
#define CUR_OFF "\x1b[?25l"

#define ENTER 13
#define DEL 127

#define UP_ARROW 65
#define DOWN_ARROW 66
#define RIGHT_ARROW 67
#define LEFT_ARROW 68


void launch_game(char* folder){
	print_line2(CLEAR_TERM);
	// launche the game
	// printf("on ouvre le dossier : %s\n",folder);
	// ouvrir le dossier passer en parametre 
	// lire le fichier deroulement
	// ouvrir le niveau corespondant
	// map1(1,1);
	DIR* mod = opendir(folder); // tester avec un access peut etre plutot
	if(mod == NULL){
		my_print_err("ouverture du dossier de mod impossible");
		sleep(5);
		exit(-1);
	}
	closedir(mod);
	char* string1 = malloc(100);
	char* string2 = malloc(100);
	string1 = mystrcpy(string1,folder);
	strcat(string1,"/");
	strcat(string1,"deroulement");
	int game_line = open(string1,O_RDONLY);
	if(game_line == -1){
		perror("impossible d'ouvrire le fichier de deroulement de la partie");
		my_print_err(string1);
		sleep(5);
		exit(-1);
	}
	free(string1);
	string1 = read_line(game_line);//name of the game
	print_line(string1,1,1);
	free(string1);
	while((string1 = read_line(game_line)) != NULL){
		strcpy(string2,folder);
		strcat(string2,"/");
		strcat(string2,"niveaux");
		strcat(string2,"/");
		strcat(string2,string1);
		my_print_err("map a ouvrir ->");
		my_print_err(string2);
		print_map(string2,4,7);
		sleep(2);
		print_line2(CLEAR_TERM);
	}
	sleep(5);
}




void print_map(char* map,int x,int y){
	char *buffer2 = malloc(100);
	char* tmp;
	print_line(map,2,2);
	int lvl1 = open(map,O_RDONLY);
	 // int lvl1 = open("mod1/niveaux/1",O_RDONLY);
	int n=0;
	unsigned int line,colomnus;
	char* map_buf;
	if(lvl1 == -1){
		my_print_err(map);
		perror("ouverture map1");
		sleep(2);
		exit(-1);
	}
	do{
		n += myread(lvl1, buffer2+n, 1);

	}while(buffer2[n-1] != '\n');
	buffer2[n]='\0';
	
	if((tmp = strpbrk(buffer2," ")) != NULL){
		*tmp = '\0';
		tmp++;
		line = atoi(buffer2);
		colomnus = atoi(tmp);
	}
	int i =1;
	print_line(buffer2,3,3);

	print_line(tmp,3,7);
	map_buf = malloc(colomnus+1);
	do{
		char* bufff = malloc(10);
		myread(lvl1,map_buf,colomnus+1);
		map_buf[colomnus]='\0';
		sprintf(bufff,"%d",i);
		print_line(bufff,x+i,y-3);
		print_line(map_buf,x+i,y);
		free(bufff);
		i++;
	}while(i <= line);
}
