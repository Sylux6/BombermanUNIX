#include "term.h"
#include "mylib.h"


void print_line(char* line,int x,int y){
	char* buffer = malloc(strlen(line) + 21);
	sprintf(buffer, "\x1b[%d;%dH%s\x1b[0m", x, y, line);
	write(1,buffer,strlen(buffer));
	free(buffer);
}

void set_pos(int x,int y){
	print_line("",x,y);
}



void term_raw(){
	struct termios new;
	tcgetattr(0, &new);
	cfmakeraw(&new);
	tcsetattr(0,TCSANOW,&new);
}

void leave(){
	_exit(1);
}