#include "term.h"

struct termios old;

void print_line(char* line,int x,int y){
	char* buffer = malloc(strlen(line) + 21);
	sprintf(buffer, "\x1b[%d;%dH%s\x1b[0m", x, y, line);
	mywrite(1,buffer,strlen(buffer));
	free(buffer);
}

void print_line2(char* line){
	mywrite(1,line,strlen(line));
}

void printf_line(char* line,int x,int y){
	char* tmp = NULL;
	char* save_tmp;
	char* tmp2 = NULL;
	tmp = mystrcpy(tmp,line);
	save_tmp = tmp;
	if((tmp2 = strpbrk(tmp,"\n")) == NULL){
		print_line(line,x,y);
		free(tmp);
		return;	
	}

	while((tmp2 = strpbrk(tmp,"\n")) != NULL ){
		*tmp2 = '\0';

		print_line(tmp,x,y);
		x++;
		tmp = tmp2+1;
	}
	print_line(tmp,x,y);


	free(save_tmp);
}

void set_pos(int x,int y){
	print_line("",x,y);
}
void term_cannonique(){
	tcsetattr(0,TCSANOW,&old);
}
void save_old(){
	struct termios term;
	tcgetattr(0, &term);
	old = term;
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