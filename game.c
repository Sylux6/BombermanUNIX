#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <termios.h>
#include <math.h>

#include "menu.h"
#include "map.h"
#include "term.h"
#include "lib.h"



int main(int argc,char** argv){
	if(argc == 1){
		if(fork() == 0){
			setenv("continue","true",1);
			setenv("LINES","20",1);
			setenv("COLUMNS","80",1);
			char* size = malloc(10);
			sprintf(size,"%sx%s",getenv("COLUMNS"),getenv("LINES"));
			execlp("xterm","xterm","-bcf","1000","-geometry",size,"-e",argv[0],"lancement_du_jeu",NULL);
		}
	}else if(strcmp(getenv("continue"),"true") == 0){
		struct menu_choice ret;
		term_raw();
		print_title(1,10);
		ret = print_menu(15,20);
		print_line("\x1b[?25l",1,1);
		if(ret.choice == 2){
			return 2;
		}
		if(ret.choice == 1){
			return 1;
		}
		if(ret.choice == 0){
			
			char* buffer = malloc(100);
			sprintf(buffer, "\x1b[2;2H %s %s\x1b[0m", getenv("COLUMNS"), getenv("LINES"));
			write(1,buffer,strlen(buffer));
			free(buffer);
			while(1){
			}
		}
	}

	return 1;
}