#include "term.h"

struct termios old;

void print_line(char* line,int x,int y){
	char* buffer = malloc(strlen(line) + 21);
	sprintf(buffer, "\x1b[%d;%dH%s\x1b[0m", x, y, line);
	mywrite(1,buffer,strlen(buffer));
	free(buffer);
}


void print_line_(char* line,int nb_arg,...){
	va_list ap;
	va_start(ap,nb_arg);
	//ONLY COLOR
	if(nb_arg == 1) {
		char* color =malloc(strlen(line)+10);
		strcpy(color,va_arg(ap,char*));
		strcat(color,line);
		print_line2(color);
		free(color);
	}
	//ONLY POSITION
	else if(nb_arg == 2) {
		int x = va_arg(ap,int);
		int y = va_arg(ap,int);
		print_line(line,x,y);
	}
	//COLOR + POSITION
	else if(nb_arg == 3) {
		int x = va_arg(ap,int);
		int y = va_arg(ap,int);
		char* color =malloc(strlen(line)+10);
		strcpy(color,va_arg(ap,char*));
		strcat(color,line);
		print_line(color,x,y);
		free(color);
	}
	//COLOR + POSITION + EFFECT
	else if(nb_arg == 4) {
		int x = va_arg(ap,int);
		int y = va_arg(ap,int);
		char* end = malloc(100);
		char* color =malloc(10);
		char* effet =malloc(10);

		strcpy(color,va_arg(ap,char*));
		strcpy(effet,va_arg(ap,char*));
		
		my_str_cpy_cat(end,color,effet,line,NULL);
		print_line(end,x,y);
		free(color);
		free(effet);
		free(end);
	}
	//CLASSIC PRINT
	else {
		print_line2(line);
	}
	va_end(ap);
}

void print_line2(char* line){
	mywrite(1,line,strlen(line));
}
void print_number(int nb,int x,int y){
	char* buffer = malloc(50);
	sprintf(buffer, "\x1b[%d;%dH(%d)\x1b[0m", x, y, nb);
	mywrite(1,buffer,strlen(buffer));
	free(buffer);	
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

void set_pos(int x,int y) {
	print_line("",x,y);
}

void term_cannonique() {
	tcsetattr(0,TCSANOW,&old);
}

void save_old() {
	struct termios term;
	tcgetattr(0, &term);
	old = term;
}

void term_raw() {
	struct termios new;
	tcgetattr(0, &new);
	cfmakeraw(&new);
	tcsetattr(0,TCSANOW,&new);
}

void leave() {
	_exit(1);
}