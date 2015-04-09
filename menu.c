#include "menu.h"
#include "term.h"
#include "lib.h"
#include "map.h"
#define ENTER 13
#define UP_ARROW 65
#define DOWN_ARROW 66
#define RIGHT_ARROW 67
#define LEFT_ARROW 68



struct menu_choice print_menu(int pos_x,int pos_y){//lui envoyer un struct menu
	int x=0;//,y=0;
	struct menu_choice ret;
	// char* buffer = malloc(100);
	char* buffer2 = malloc(100);
	print_line("\x1b[?25l",0,0);
	print_line("Démarrer une partie", pos_x, pos_y);
	print_line("Aide", 1+pos_x, pos_y);
	print_line("Quitter", 2+pos_x, pos_y);
	while(1){
		
		
		// sprintf(buffer, "\x1b[%d;%dH\x1b[91m\x1b[33;5m%s", x + pos_x, pos_y - 3,"->");
		// write(1,buffer,strlen(buffer));
		print_line("\x1b[91m\x1b[33;5m->", x + pos_x, pos_y - 3);
		
		read(0,buffer2,1);
		
		// sprintf(buffer, "\x1b[%d;%dH\x1b[91m%s", x + pos_x, pos_y - 3,"  ");
		// write(1,buffer,strlen(buffer));
		print_line("\x1b[91m  ", x + pos_x, pos_y - 3);
		
		if(buffer2[0] == 'z' || buffer2[0] == UP_ARROW)
		{
			x = mod(x-1,3);	
			// x = (x-1)%3;
		}
		if(buffer2[0] == 's' || buffer2[0] == DOWN_ARROW)
		{
			x = mod(x+1,3);//+1;
			// x = x%3+1;
		}

		if(buffer2[0] == ENTER && x==0) {
			print_line("\x1b[2J\x1B[;H",0,0);
			map1(5,5);//lancement du jeu
			ret.choice = 0;
			return ret;
		}else if(buffer2[0] == ENTER && x==1){
			ret.choice = 1;
			return ret;
		}
		else if(buffer2[0] == ENTER && x==2){
			ret.choice = 2;
			return ret;
		}
	}
}

void print_title(int x,int y){
	char* line[5] = {"    ____                  __                                  ",
					"   / __ )____  ____ ___  / /_  ___  _________ ___  ____ _____ ",
					"  / __  / __ \\/ __ `__ \\/ __ \\/ _ \\/ ___/ __ `__ \\/ __ `/ __ \\",	
					" / /_/ / /_/ / / / / / / /_/ /  __/ /  / / / / / / /_/ / / / /",
					"/_____/\\____/_/ /_/ /_/_.___/\\___/_/  /_/ /_/ /_/\\__,_/_/ /_/ "};		

	int i;
	char* buffer = malloc(100);
	for(i = 0; i < 5 ; i++){
		sprintf(buffer, "\x1b[%d;%dH%s", x, y,line[i]);
		write(1,buffer,strlen(buffer));
		x++;

	}
	free(buffer);
}	