#include "map.h"
#include "term.h"

#include "lib.h"


void map1(int x,int y){
	char *buffer2 = malloc(100);
	char* tmp;
	int lvl1 = open("mod1/niveau/1",O_RDONLY);
	int n=0;
	unsigned int line,colomnus;//,all;
	char* map_buf;
	if(lvl1 == -1){
		perror("omg");
		sleep(2);
	}
	do{
		n += read(lvl1,buffer2 + n ,1);

	}while(buffer2[n-1] != '\n');
	buffer2[n]='\0';
	
	if((tmp = strpbrk(buffer2," ")) != NULL){
		*tmp = '\0';
		tmp++;
		line = atoi(buffer2);
		colomnus = atoi(tmp);
	}
	int i =1;
	map_buf = malloc(colomnus+1);
	do{
		realRead(lvl1,map_buf,colomnus+1);
		map_buf[colomnus]='\0';
		print_line(map_buf,x+i,y);
		i++;
	}while(i <= line);
}
