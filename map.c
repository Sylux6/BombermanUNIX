#include "map.h"

//print the map and launch the game 
void launch_game(char* folder){
	struct board* map;
	print_line2(CLEAR_TERM);
	DIR* mod = opendir(folder); // tester avec un access peut etre plutot
	if(mod == NULL){
		my_print_err(folder);
		my_print_err("ouverture du dossier de mod impossible\n");
		sleep(5);
		exit(-1);
	}
	closedir(mod);
	//se serai la fin du test avec access


	char* string1 = malloc(100);//contiendra le path du deroulement
	
	//make the link of "deroulement"

	string1 = my_str_cpy_cat(string1,folder,"/deroulement",NULL);
	//end of deroulement's link
	int game_line = open(string1,O_RDONLY);
	if(game_line == -1){
		perror("impossible d'ouvrir le fichier de deroulement de la partie");
		my_print_err(string1);
		exit(-1);
	}
	free(string1);

	string1 = read_line(game_line);//name of the game
	print_line(string1,1,(atoi(getenv("COLUMNS"))-strlen(string1))/2);
	free(string1);
	//now string1 est egale au nom du niveau
	char* string2;//contiendra le path du lvl
	// char playerslife;
	while((string1 = read_line(game_line)) != NULL){
		map = malloc(sizeof(struct board));
		string2 = malloc(100);
		string2 = my_str_cpy_cat(string2,folder,"/niveaux/",string1,NULL);		
		map_init(map,string2);
		// print_map(map);

		// -----------launch the game here------------------
		player p1 = create_player(1);
		player p2 = create_player(2);
	
		print_carac(*p1, *p2);		
		spawn(p1, map);
		spawn(p2, map);
		listBomb bombs = initList();
		print_map(map,p1,p2);
		mainGame(p1, p2, map,bombs);
		
		
		p1->life = 3;
		p2->life = 3;
		//-------------------end---------------------------
		sleep(2);
		// print who carried the game
		print_line2(CLEAR_TERM);
		free(string2);
		del_board(map);
	}
	// sleep(5);
}

void print_map(struct board *map,struct player *p1,struct player *p2){
	int i;
	for (i = 0; i < map->x; ++i)
	{
		print_line(map->map[i],map->up_left_corner.x + i,map->up_left_corner.y);
		for(int j = 0 ; j < map->y ; j++){
			if(map->map[i][j] == 'X'){
				char* color = malloc(20);
				strcpy(color,"\033[22;31m");
				strcat(color,"X");
				strcat(color,"\x1b[0m");
				print_line(color,map->up_left_corner.x + i, map->up_left_corner.y + j);
			}else if(map->map[i][j] == ' '){
				// map->powerup[i][j].symbol
				print_line(&(map->powerups[i][j].symbol),map->up_left_corner.x + i, map->up_left_corner.y + j);
			}
		}
	}
	//print les 2 player
	print_player(p1, map);
	print_player(p2, map);
	// is_touch(p1,p2,map);

}
void print_player(struct player *p,struct board *map){
	char* color = malloc(20);

	strcpy(color,p->effet);
	strcat(color,p->color);
	strcat(color,p->view);
	strcat(color,"\x1b[0m");
	print_line(color,map->up_left_corner.x + p->pos.x, map->up_left_corner.y + p->pos.y);// afficher le joueur

	// afficher les bombe 
	// for(int i = 0; i < p->nb_bomb ; i++){
	// 	if(p->bomb_own[i].state == 1){
	// 		char* bomb = malloc(30);
	// 		strcpy(bomb,"\033[22;31m\x1b[5m");
	// 		strcat(bomb,"@");
	// 		strcat(bomb,"\x1b[0m");
	// 		print_line(bomb,map->up_left_corner.x + p->bomb_own[i].x,map->up_left_corner.y + p->bomb_own[i].y);
	// 		free(bomb);
	// 	}
	// 	else if(p->bomb_own[i].state == 2){
	// 		//print explosion
	// 		// p->bomb_own[i].time_explode = 1000;
	// 		explode(p->bomb_own[i].x, p->bomb_own[i].y,*p,map);
	// 		// char* bomb = malloc(15);
	// 		// strcat(bomb," ");
	// 		// strcat(bomb,"\x1b[0m");
	// 		// print_line(bomb,map->up_left_corner.x + p->bomb_own[i].x,map->up_left_corner.y + p->bomb_own[i].y);
	// 		// free(bomb);	
	// 		p->bomb_own[i].state = 3;
	// 	}
	// 	else if(p->bomb_own[i].state == 3){
	// 		//print explode 
	// 		if(p->bomb_own[i].time_explode <= 0){
	// 			p->bomb_own[i].state = 0;
	// 			p->bomb_own[i].time_explode = -1;
	// 			clear_range_bomb(p->bomb_own[i].x, p->bomb_own[i].y, *p, map);
	// 		}
		// }
	// }
	free(color);
}


void map_init(struct board* map,char* file/*,int x,int y*/){
	char *buffer2 = malloc(100);
	char* tmp;
	// print_line(file,2,2);
	int lvl1 = open(file,O_RDONLY); // open the map's file
	int n = 0;
	unsigned int line,columns;
	char* buf;
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
	int i = 0, j;
	buf = malloc(columns+1);
	map->x = line;
	map->y = columns;

	// if(x+line > atoi(getenv("LINES")) || y+columns > atoi(getenv("COLUMNS")))
	if(map->y > atoi(getenv("COLUMNS")) || map->x >atoi(getenv("LINES"))-2 )
	{
		my_print_err("the map is too big!");
		exit(-1);
	}
	int x = (atoi(getenv("LINES"))+2-map->x)/2;
	int y = (atoi(getenv("COLUMNS"))-map->y)/2;
	map->up_left_corner.x = x;
	map->up_left_corner.y = y;

	// map->changed = 1;
	map->map = malloc(sizeof(char*)*map->x);
	map->powerups = malloc(sizeof(struct powerup*)*map->x);
	map->bombs = malloc(sizeof(struct bomb*)*map->x);
	do{
		map->map[i] = malloc(sizeof(char)*(map->y+1));
		map->powerups[i] = malloc(sizeof(struct powerup)*map->y);
		map->bombs[i] = malloc(sizeof(struct bomb)*map->y);
		myread(lvl1,buf,columns+1);
		buf[columns]='\0';
		map->map[i] = mystrcpy(map->map[i], buf);
		i++;
	}while(i < line);
	free(buf);

	///////////////////////////
	//GETTING POWERUPS
	///////////////////////////
	buf = malloc(sizeof(char));
	for(i = 0; i < line; i++) {
		for(j = 0; j < columns; j++) {
			n = myread(lvl1, buf, 1);
			if(n == 0) {
				//Remplir le reste de la ligne du tableau de powerup vide
				while(j < columns) {
					my_print_err(" ");
					map->powerups[i][j] = createPowerup(EMPTY, 0);
					j++;
				}
			}
			else {
				switch(*buf) {
					case ' ':
						map->powerups[i][j] = createPowerup(EMPTY, 0);
						break;
					case '+':
						map->powerups[i][j] = createPowerup(SPEED, -100);
						break;
					case '*':
						map->powerups[i][j] = createPowerup(BOMB_RADIUS, 1);
						break;
					case '@':
						map->powerups[i][j] = createPowerup(BOMB_MAX, 1);
						break;
					case '\n':
						//Remplir le reste de la ligne du tableau de powerup vide
						while(j < columns) {
							my_print_err(" ");
							map->powerups[i][j] = createPowerup(EMPTY, 0);
							j++;
						}
						break;
					default:
						map->powerups[i][j] = createPowerup(EMPTY, 0);
				}
			}
		}
	}
	///////////////////////////
	//BOMBS ARRAY INIT
	///////////////////////////
	// for(i = 0; i < line; i++)
	// 	for(j = 0; j < columns; j++)
	// 		map->bombs[i][j] = NULL;
}

void del_board(struct board *map){
	int i;
	for(i = 0; i < map->x; i++) {
		free(map->map[i]);
		free(map->powerups[i]);
	}
	free(map->map);
	free(map->powerups);
	free(map);
}

void print_carac(struct player p1,struct player p2){
	//name player 1
	print_line_(p1.name,3,1,1,BROWN);
	//name player 2
	print_line_(p2.name,3,1,atoi(getenv("COLUMNS"))-strlen(p2.name)+1,CYAN_CLAIRE);

	//--- end of printf name ------------

	//their life 
	char *playerslife = calloc(10,1);

	// player1's life
	print_line("life :",2,1);
	sprintf(playerslife,"%d",p1.life);
	print_line(playerslife,2,8);

	//player2's life
	print_line("life :",2,atoi(getenv("COLUMNS"))-1-strlen("life :"));
	sprintf(playerslife,"%d",p2.life);
	print_line(playerslife,2,atoi(getenv("COLUMNS")));
}
