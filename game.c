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
#include "mylib.h"



int main(int argc,char** argv){
	if(argc == 1){
		if(fork() == 0){
			setenv("continue","true",1);
			setenv("LINES","20",1);
			setenv("COLUMNS","80",1);
			char size[10];
			sprintf(size,"%sx%s",getenv("COLUMNS"),getenv("LINES"));
			execlp("xterm","xterm","-bcf","1000","-geometry",size,"-e",argv[0],"lancement_du_jeu",NULL);
		}
	}
	else if(strcmp(getenv("continue"),"true") == 0){
		new_stderr(); // create a strerr.log to save all the error

		save_old(); // save the term cannonique value
		term_raw(); // change the terminal to the mode raw
		launchMenu(menuINIT(), 7, 4); // launch the main menu 
	}
	return 1;
}