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
#include "term.h"
#include "mylib.h"


int main(int argc,char** argv){
	if(argc == 1){
			setenv("continue","true",1);
			setenv("LINES","20",1);
			setenv("COLUMNS","80",1);
			char size[10];
			sprintf(size,"%sx%s",getenv("COLUMNS"),getenv("LINES"));
			execlp("xterm","xterm","-bcf","100","-geometry",size,"-e",argv[0],"lancement_du_jeu",NULL);
	}
	else if(strcmp(getenv("continue"),"true") == 0){
		//Creating stderr.log to save errors
		new_stderr();
		//Saving old terminal values
		save_old();
		//Switching to RAW mode
		term_raw();
		//Launch main menu
		launchMenu(menuINIT(), 7, 4);
	}
	return 1;
}
