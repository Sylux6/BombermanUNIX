#include "map.h"

//print the map and launch the game 

void launch_game(char* folder){
	struct board* map;
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
		perror("impossible d'ouvrir le fichier de deroulement de la partie");
		my_print_err(string1);
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
		map_init(map,string2);
		// print_map(map);

		// -----------launch the game here------------------
		player p1 = create_player(1);
		player p2 = create_player(2);
		spawn(&p1, map);
		spawn(&p2, map);
		print_map(map,p1,p2);
		print_line("on app mainGame",19,1);
		mainGame(initGameplay(), &p1, &p2, map);
		// set_pos(17,3);
		// printf("%s %d - %d","j1",p1.pos.x,p1.pos.y );
		// set_pos(18,3);
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

void print_map(struct board *map, const struct player p1, const struct player p2){
	if(map->changed != 0){
		map->changed = 0;
		//print the map
		int i;
		//calcul of the position to the upper left cornerof the map
		char* buffer = malloc(100);
		sprintf(buffer,"%d - %d ",map->up_left_corner.x,map->up_left_corner.y);
		print_line(buffer,2,2);
		for (i = 0; i < map->x; ++i)
		{
			print_line(map->map[i],map->up_left_corner.x + i,map->up_left_corner.y);

		}
		//print les 2 player
		print_player(p1, map);
		print_player(p2, map);

	}

}
void print_player(const struct player p,struct board *map){
	char* color = malloc(20);
	strcpy(color,p.color);
	strcat(color,p.view);
	strcat(color,"\x1b[0m");
	print_line(color,map->up_left_corner.x + p.pos.x, map->up_left_corner.y + p.pos.y);

}


void map_init(struct board* map,char* file/*,int x,int y*/){
	char *buffer2 = malloc(100);
	char* tmp;
	print_line(file,2,2);
	int lvl1 = open(file,O_RDONLY); // open the map's file
	int n=0;
	unsigned int line,columns;
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
	//find the number of line and columns of the file
	if((tmp = strpbrk(buffer2," ")) != NULL){
		*tmp = '\0';
		tmp++;
		line = atoi(buffer2);
		columns = atoi(tmp);
	}

	// copy this map to the memory 
	int i =0;
	map_buf = malloc(columns+1);
	map->x = line;
	map->y = columns;

	// if(x+line > atoi(getenv("LINES")) || y+columns > atoi(getenv("COLUMNS")))
	if(map->y > atoi(getenv("COLUMNS")) || map->x >atoi(getenv("LINES"))-2 )
	{
		my_print_err("map to big to be print");
		exit(-1);
	}
	int x = (atoi(getenv("LINES"))+2-map->x)/2;
	int y = (atoi(getenv("COLUMNS"))-map->y)/2;
	map->up_left_corner.x = x;
	map->up_left_corner.y = y;

	map->changed = 1;
	map->map = malloc(sizeof(char*)*map->x);
	do{
		map->map[i] = malloc(sizeof(char)*(map->y+1));
		myread(lvl1,map_buf,columns+1);
		map_buf[columns]='\0';
		map->map[i] = mystrcpy(map->map[i], map_buf);
		i++;
	}while(i < line);
}

void del_board(struct board *map){
	int i;
	for(i=0 ; i< map->x ; i++)
		free(map->map[i]);
	free(map->map);
	free(map);
}