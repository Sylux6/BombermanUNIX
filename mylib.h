#ifndef _MYLIB_INCLUDED
#define _MYLIB_INCLUDED
 
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

#include <stdarg.h>
#include <math.h>

#define WHITE 		"\033[01;37m"			
#define BLACK 		"\033[22;30m"			

#define RED 		"\033[22;31m"			
#define GREEN 		"\033[22;32m"			
#define BLUE 		"\033[22;34m"
#define BROWN	    "\033[22;33m"      
#define GRAY 		"\033[22;37m"			
#define CYAN_CLAIRE "\033[22;36m"

#define YELLOW 		"\033[01;33m"		
#define CYAN 		"\033[01;36m"			
#define DARKGRAY 	"\033[01;30m"		

#define NORMAL		"\x1B[0m"               
#define BOLD		"\x1B[1m"               
#define UNDERLINE	"\x1B[4m"               
#define BLINK		"\x1B[5m"               

#define MIN(x,y) (x<y)?x:y

unsigned int mymodulo();

ssize_t mywrite(int fd, void *buf, size_t count);

ssize_t myread(int fd, void *buf, size_t count);

void *realloc(void *ptr, size_t size);

char* mystrcpy(char *dest, const char *src);

void my_print_err(char* message);

void new_stderr();

char* read_line(int fd);

int my_rand(int min,int max);

char* my_str_cpy_cat(char* dest,char *from,...);

int isNeg(int nb);
 
#endif