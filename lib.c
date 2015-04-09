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

void realRead(int fd,void* buf,int size){
	ssize_t lu = 0, aLire = size,lu2 = 1;

	while(aLire > 0 && lu2 != 0){
		lu2 = read(fd, buf+lu, aLire);
		if(lu2 == -1){
			perror("read  ");
			sleep(2);
			exit(-1);
		}
		lu += lu2; 
		
		aLire -= lu;
	}

}


unsigned int mod(int num,int mod){
	if(num >= 0){
		return num%mod;
	}else{
		return (num%mod + mod)%mod;
	}
}
