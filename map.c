#include "map.h"

//Print the map and launch the game 
void launch_game(char* folder){
	struct board* map;
	print_line2(CLEAR_TERM);
	DIR* mod = opendir(folder);
	if(mod == NULL){
		my_print_err(folder);
		my_print_err("ouverture du dossier de mod impossible\n");
		sleep(5);
		exit(-1);
	}
	closedir(mod);

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

	char* name = malloc(30);
	name = read_line(game_line);//name of the game
	char* string2;//contiendra le path du lvl
	int win1=0;
	int win2=0;

	while((string1 = read_line(game_line)) != NULL){
		print_line(name,1,(atoi(getenv("COLUMNS"))-strlen(string1))/2);
	
		map = malloc(sizeof(struct board));
		string2 = malloc(100);
		string2 = my_str_cpy_cat(string2,folder,"/niveaux/",string1,NULL);		
		map_init(map,string2);

		// -----------launch the game here------------------
		player p1 = create_player(1);
		player p2 = create_player(2);
	
		spawn(p1, p2, map);
		print_map(map,p1,p2);
		mainGame(p1, p2, map);

		//-------------------end---------------------------
		// print who won the game
		print_line2(CLEAR_TERM);
		if(p1->life == 0 && p2->life != 0){
			win2++;
			print_line_("Player 2 won this round",3,5,20,GREEN);
			sleep(4);
		}else if(p1->life != 0 && p2->life == 0){
			win1++;
			print_line_("Player 1 won this round",3,5,20,GREEN);
			sleep(4);
		}else{
			print_line_("Nobody won this round",3,5,20,RED);
			sleep(4);
		}
		free(string2);
		del_board(map);
		print_line2(CLEAR_TERM);
	}
	if(win1 > win2){
		print_line_("Player 1 won the game",4,10,20,GREEN,BLINK);
        sleep(4);
	}else if(win1 < win2){
		print_line_("Player 2 won the game",4,10,20,GREEN,BLINK);
        sleep(4);
	}else{
		print_line_("Nobody won the game",4,10,20,RED,BLINK);
        sleep(4);
	}
}

void printEndOfMap(board map){
	int x,y;
	struct timespec* rec = malloc(sizeof(struct timespec));
	rec->tv_sec = 0;
	rec->tv_nsec = 10000;
	for(x=0; x < map->x ; x++){
		for(y = 0; y < map->y ; y++){
			print_line(" ",map->up_left_corner.x  +x , map->up_left_corner.y +y);
			nanosleep(rec,NULL);
			rec->tv_sec = 0;
			rec->tv_nsec = 10000000;
		}
	}
}

void print_map(board map, player p1, player p2){
	int i;
	for (i = 0; i < map->x; ++i)
	{
		for(int j = 0 ; j < map->y ; j++){
			if(map->map[i][j] == 'X'){
				print_line_("X",3,map->up_left_corner.x + i, map->up_left_corner.y + j,RED);
			}else if(map->map[i][j] == '0'){
				print_line_("0",3,map->up_left_corner.x + i, map->up_left_corner.y + j,PURPLE);
			}
			else if(map->map[i][j] == ' '){

				print_line_(&(map->powerups[i][j].symbol),3,map->up_left_corner.x + i, map->up_left_corner.y + j,BLUE);
			}else{
				print_line_(&(map->map[i][j]),2,map->up_left_corner.x +i,map->up_left_corner.y +j);
			}
		}
	}
	//Displaying players on map
	print_player(p1, map);
	print_player(p2, map);
	if(p1->pos.x == p2->pos.x && p1->pos.y == p2->pos.y){
		print_line_("P",4,map->up_left_corner.x + p1->pos.x, map->up_left_corner.y + p1->pos.y,GREEN,BOLD);// afficher le joueur		
	}
	printBomb(map);
	print_carac(*p1,*p2);

}
void print_player(struct player *p,struct board *map){
	char* color = malloc(20);

	print_line_(p->view,4,map->up_left_corner.x + p->pos.x, map->up_left_corner.y + p->pos.y,p->effet,p->color);// afficher le joueur
	free(color);
}

void printBomb(board map){
	listBomb c = map->listBombs->next;
	while(c != NULL) {
		if(isSet(c->bomb)){
			print_line_("@",4,map->up_left_corner.x + c->bomb->x,map->up_left_corner.y + c->bomb->y,RED,BLINK);
		}
		else if(isExploding(c->bomb)){
			explode(c->bomb,map);
		}
		else if(isEnded(c->bomb)){
			if(c->bomb->time_explode <= 0){
				clear_bomb(c->bomb,map);
				map->map[c->bomb->x][c->bomb->y] = ' ';
				changeState(c->bomb,NONE);
			}
		}
		c = c->next;
	}
}

void print_carac(struct player p1,struct player p2) {
	// set_pos(3,1);
	// print_line2(CLEAR_BEFOR_CUR);

	
	print_line_(p1.name,3,1,1,BROWN);
	print_line_(p2.name,3,1,atoi(getenv("COLUMNS"))-strlen(p2.name)+1,CYAN_CLAIRE);

	//--- end of printf name ------------

	//LIFE
	char *playerslife = calloc(10,1);

	// player1 life
	print_line("life :",2,1);
	sprintf(playerslife,"%d",p1.life);
	print_line(playerslife,2,8);

	//player2 life
	print_line("life :",2,atoi(getenv("COLUMNS"))-1-strlen("life :"));
	sprintf(playerslife,"%d",p2.life);
	print_line(playerslife,2,atoi(getenv("COLUMNS")));
}

void map_init(struct board* map,char* file){
	char *buffer2 = malloc(100);
	char* tmp;
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
	//Read the first line of the file
	do{
		n += myread(lvl1, buffer2+n, 1);

	}while(buffer2[n-1] != '\n');
	buffer2[n]='\0';
	//Getting lines and columns
	if((tmp = strpbrk(buffer2," ")) != NULL){
		*tmp = '\0';
		tmp++;
		line = atoi(buffer2);
		columns = atoi(tmp);
	}

	//Copying map 
	int i = 0, j;
	buf = malloc(columns+1);
	map->x = line;
	map->y = columns;

	if(map->y > atoi(getenv("COLUMNS")) || map->x >atoi(getenv("LINES"))-2)
	{
		my_print_err("the map is too big!");
		exit(-1);
	}
	int x = (atoi(getenv("LINES"))+2-map->x)/2+1;
	int y = (atoi(getenv("COLUMNS"))-map->y)/2+1;
	map->up_left_corner.x = x;
	map->up_left_corner.y = y;

	map->map = malloc(sizeof(char*)*map->x);
	map->powerups = malloc(sizeof(struct powerup*)*map->x);
	do{
		map->map[i] = malloc(sizeof(char)*(map->y+1));
		map->powerups[i] = malloc(sizeof(struct powerup)*map->y);
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
						map->powerups[i][j] = createPowerup(SPEED, -60);
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
	listBomb l = initList();
	map->listBombs = l;
}

void del_board(struct board *map){
	int i;
	removeListBomb(map->listBombs);
	for(i = 0; i < map->x; i++) {
		free(map->map[i]);
		free(map->powerups[i]);
	}
	free(map->map);
	free(map->powerups);
	free(map);
}


//tmp must be initialized !
int area_calcul(char** tmp, board map, int x, int y) {
	if(map->map[x][y] != ' ')
		return 0;
	
	tmp[x][y] = 1;
	
	if(tmp[x+1][y] == 0 && tmp[x-1][y] == 0 && tmp[x][y+1] == 0 && tmp[x][y-1] == 0)
		return 1 + area_calcul(tmp, map, x+1, y) + area_calcul(tmp, map, x-1, y) + area_calcul(tmp, map, x, y+1) + area_calcul(tmp, map, x, y-1);
	
	if(tmp[x+1][y] == 0 && tmp[x-1][y] == 0 && tmp[x][y+1] == 0 && tmp[x][y-1] == 1)
		return 1 + area_calcul(tmp, map, x+1, y) + area_calcul(tmp, map, x-1, y) + area_calcul(tmp, map, x, y+1);
	
	if(tmp[x+1][y] == 0 && tmp[x-1][y] == 0 && tmp[x][y+1] == 1 && tmp[x][y-1] == 0)
		return 1 + area_calcul(tmp, map, x+1, y) + area_calcul(tmp, map, x-1, y) + area_calcul(tmp, map, x, y-1);
	
	if(tmp[x+1][y] == 0 && tmp[x-1][y] == 0 && tmp[x][y+1] == 1 && tmp[x][y-1] == 1)
		return 1 + area_calcul(tmp, map, x+1, y) + area_calcul(tmp, map, x-1, y);
	
	if(tmp[x+1][y] == 0 && tmp[x-1][y] == 1 && tmp[x][y+1] == 0 && tmp[x][y-1] == 0)
		return 1 + area_calcul(tmp, map, x+1, y) + area_calcul(tmp, map, x, y+1) + area_calcul(tmp, map, x, y-1);

	if(tmp[x+1][y] == 0 && tmp[x-1][y] == 1 && tmp[x][y+1] == 0 && tmp[x][y-1] == 1)
		return 1 + area_calcul(tmp, map, x+1, y) + area_calcul(tmp, map, x, y+1);
	
	if(tmp[x+1][y] == 0 && tmp[x-1][y] == 1 && tmp[x][y+1] == 1 && tmp[x][y-1] == 0)
		return 1 + area_calcul(tmp, map, x+1, y) + area_calcul(tmp, map, x, y-1);
	
	if(tmp[x+1][y] == 0 && tmp[x-1][y] == 1 && tmp[x][y+1] == 1 && tmp[x][y-1] == 1)
		return 1 + area_calcul(tmp, map, x+1, y);
	
	if(tmp[x+1][y] == 1 && tmp[x-1][y] == 0 && tmp[x][y+1] == 0 && tmp[x][y-1] == 0)
		return 1 + area_calcul(tmp, map, x-1, y) + area_calcul(tmp, map, x, y+1) + area_calcul(tmp, map, x, y-1);
	
	if(tmp[x+1][y] == 1 && tmp[x-1][y] == 0 && tmp[x][y+1] == 0 && tmp[x][y-1] == 1)
		return 1 + area_calcul(tmp, map, x-1, y) + area_calcul(tmp, map, x, y+1);

	if(tmp[x+1][y] == 1 && tmp[x-1][y] == 0 && tmp[x][y+1] == 1 && tmp[x][y-1] == 0)
		return 1 + area_calcul(tmp, map, x-1, y) + area_calcul(tmp, map, x, y-1);
	
	if(tmp[x+1][y] == 1 && tmp[x-1][y] == 0 && tmp[x][y+1] == 1 && tmp[x][y-1] == 1)
		return 1 + area_calcul(tmp, map, x-1, y);
	
	if(tmp[x+1][y] == 1 && tmp[x-1][y] == 1 && tmp[x][y+1] == 0 && tmp[x][y-1] == 0)
		return 1 + area_calcul(tmp, map, x, y+1) + area_calcul(tmp, map, x, y-1);
	
	if(tmp[x+1][y] == 1 && tmp[x-1][y] == 1 && tmp[x][y+1] == 0 && tmp[x][y-1] == 1)
		return 1 + area_calcul(tmp, map, x, y+1);
	
	if(tmp[x+1][y] == 1 && tmp[x-1][y] == 1 && tmp[x][y+1] == 1 && tmp[x][y-1] == 0)
		return 1 + area_calcul(tmp, map, x, y-1);

	if(tmp[x+1][y] == 1 && tmp[x-1][y] == 1 && tmp[x][y+1] == 1 && tmp[x][y-1] == 1)
		return 1;

	return 0;
}

board random_map() {
	board newBoard = malloc(sizeof(struct board));
	int x = my_rand(5, atoi(getenv("LINES"))-2);
	int y = my_rand(5, atoi(getenv("COLUMNS"))) + 1;
	int area, n, i, j, k, l;
	char **map = malloc(sizeof(char*)*x);
	for(i = 0; i < x; i++)
		map[i] = malloc(sizeof(char)*y);

	newBoard->map = map;
	newBoard->x = x;
	newBoard->y = y - 1;
	newBoard->up_left_corner.x = (atoi(getenv("LINES"))+2-newBoard->x)/2+1;
	newBoard->up_left_corner.y = (atoi(getenv("COLUMNS"))-newBoard->y)/2+1;
	newBoard->powerups = malloc(sizeof(struct powerup*)*newBoard->x);
	for(i = 0; i < newBoard->x; i++)
		newBoard->powerups[i] = malloc(sizeof(struct powerup)*newBoard->y);
	newBoard->listBombs = initList();
	
	//WALL OUTLINE
	for(i = 0; i < newBoard->y; i++) {
		map[0][i] = '0';
		map[newBoard->x-1][i] = '0';
	}
	for(i = 0; i < newBoard->x; i++) {
		map[i][0] = '0';
		map[i][newBoard->y-1] = '0';
		map[i][newBoard->y] = '\0';
	}
	do {
		//GENERATING POWERUPS
		for(i = 0; i < newBoard->x; i++) {
			for(j = 0; j < newBoard->y; j++) {
				if(i == 0 || i == newBoard->x-1 || j == 0 || j == newBoard->y-1)
					newBoard->powerups[i][j] = createPowerup(EMPTY, 0);
				else {
					n = my_rand(1, 100);
					if(n < 5) {
						n = my_rand(1, 4);
						switch(n) {
							case SPEED:
								newBoard->powerups[i][j] = createPowerup(SPEED, -60);
								break;
							case BOMB_RADIUS:
								newBoard->powerups[i][j] = createPowerup(BOMB_RADIUS, 1);
								break;
							case BOMB_MAX:
								newBoard->powerups[i][j] = createPowerup(BOMB_MAX, 1);
								break;
							default:
								newBoard->powerups[i][j] = createPowerup(EMPTY, 0);
						}
					}
				}
			}
		}
		//FILLING MAP
		for(i = 1; i < newBoard->x-1; i++) {
			for(j = 1; j < newBoard->y-1; j++) {
				n = my_rand(0, 100);
				if(n < 20)
					map[i][j] = '0';
				else if(n > 50)
					map[i][j] = ' ';
				else {
					n = my_rand(1, 9);
					map[i][j] = n+48;
				}
			}
		}

		char **tmp = malloc(sizeof(char*)*newBoard->x);
		for(i = 0; i < newBoard->x; i++)
			tmp[i] = malloc(sizeof(char)*newBoard->y);

		for(i = 0; i < newBoard->x-1; i++) {
			for(j = 0; j < newBoard->y-1; j++) {
				if(map[i][j] == ' ') {
					for(k = 0; k < newBoard->x; k++)
						for(l = 0; l < newBoard->y; l++)
							if(newBoard->map[k][l] != ' ')
								tmp[k][l] = 1;
							else
								tmp[k][l] = 0;
					if((area = area_calcul(tmp, newBoard, i, j)) > RADIUS + 2)
						return newBoard;
				}
			}
		}
	}
	while(area <= RADIUS + 2);

	return newBoard;
}

//Check if there is a path to player p from (x, y)
int check_path(char **tmp, board map, int x, int y, int xf, int yf) {
	tmp[x][y] = 1;
	if(x == xf && y == yf)
		return 1;
	if(x+1 < map->x)
		if(map->map[x+1][y] != '0' && tmp[x+1][y] == 0)
			if(check_path(tmp, map, x+1, y, xf, yf))
				return 1;
	if(x-1 > 0)
		if(map->map[x-1][y] != '0' && tmp[x-1][y] == 0)
			if(check_path(tmp, map, x-1, y, xf, yf))
				return 1;
	if(y+1 <= map->y)
		if(map->map[x][y+1] != '0' && tmp[x][y+1] == 0)
			if(check_path(tmp, map, x, y+1, xf, yf))
				return 1;
	if(y-1 > 0)
		if(map->map[x][y-1] != '0' && tmp[x][y-1] == 0)
			if(check_path(tmp, map, x, y-1, xf, yf))
				return 1;
	return 0;
}
