#include "map.h"


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

//print the map and launch the game 

void launch_game(char* folder){
	board* map;
	print_line2(CLEAR_TERM);
	DIR* mod = opendir(folder); // tester avec un access peut etre plutot
	if(mod == NULL){
		my_print_err(folder);
		my_print_err("ouverture du dossier de mod impossible");
		sleep(5);
		exit(-1);
	}
	closedir(mod);
	//se serai la fin du test avec access


	char* string1 = malloc(100);//contiendra le path du deroulement
	
	//make the link of "deroulement"
	string1 = mystrcpy(string1,folder);
	strcat(string1,"/");
	strcat(string1,"deroulement");
	//end of deroulement's link
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
	//now string1 est egale au nom du niveau
	char* string2;//contiendra le path du lvl
	while((string1 = read_line(game_line)) != NULL){
		map = malloc(sizeof(struct board));
		string2 = malloc(100);
		strcpy(string2,folder);
		strcat(string2,"/");
		strcat(string2,"niveaux");
		strcat(string2,"/");
		strcat(string2,string1);
		map_init(map,string2,4,7);
		print_map(map,4,7);

		// -----------launch the game here------------------
		// player p1 = create_player(1);
		// player p2 = create_player(2);
		// spawn(&p1,4,7,atoi(getenv("MAPX")),atoi(getenv("MAPY")));
		// spawn(&p2,4,7,atoi(getenv("MAPX")),atoi(getenv("MAPY")));
		// set_pos(15,3);
		// printf("%s %d - %d","j1",p1.pos.x,p1.pos.y );
		// set_pos(16,3);
		// printf("%s %d - %d","j1",p2.pos.x,p2.pos.y );

		// print_line(p1.view,p1.pos.x,p1.pos.y);
		// print_line(p2.view,p2.pos.x,p2.pos.y);
		

		//-------------------end---------------------------
		sleep(2);
		print_line2(CLEAR_TERM);
		free(string2);
		del_board(map);
	}
	// sleep(5);
}

void print_map(board* map,int x,int y){
	if(map->changed != 0){
		map->changed = 0;
		//print the map
		int i;
		for (i = 0; i < map->x; ++i)
		{
			print_line(map->map[i],x+i,y);
		}
	}

}

void map_init(board* map,char* file,int x,int y){
	char *buffer2 = malloc(100);
	char* tmp;
	print_line(file,2,2);
	int lvl1 = open(file,O_RDONLY); // open the map's file
	int n=0;
	unsigned int line,colomnus;
	char* map_buf;
	if(lvl1 == -1){
		my_print_err(file);
		perror("ouverture map1");
		sleep(2);
		exit(-1);
	}
	//read the first line of the file
	do{
		n += myread(lvl1, buffer2+n, 1);

	}while(buffer2[n-1] != '\n');
	buffer2[n]='\0';
	//find the number of line and colomns of the file
	if((tmp = strpbrk(buffer2," ")) != NULL){
		*tmp = '\0';
		tmp++;
		line = atoi(buffer2);
		colomnus = atoi(tmp);
	}

	// copy this map to the memory 
	int i =0;
	map_buf = malloc(colomnus+1);
	map->x = line;
	map->y = colomnus;
	map->changed = 1;
	map->map = malloc(sizeof(char*)*map->x);
	do{
		map->map[i] = malloc(sizeof(char)*(map->y+1));
		myread(lvl1,map_buf,colomnus+1);
		map_buf[colomnus]='\0';
		map->map[i] = strcpy(map->map[i], map_buf);
		// print_line(map_buf,4+i,7);
		// print_line(map->map[i],4+i,7);
		i++;
	}while(i < line);
}

void del_board(board *map){
	int i;
	for(i=0 ; i< map->x ; i++)
		free(map->map[i]);
	free(map->map);
	free(map);
}