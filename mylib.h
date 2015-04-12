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
#include <math.h>

#define MIN(x,y) (x<y)?x:y

unsigned int mymodulo();

ssize_t mywrite(int fd, void *buf, size_t count);

ssize_t myread(int fd, void *buf, size_t count);

void *realloc(void *ptr, size_t size);

char* mystrcpy(char *dest, const char *src);

void my_print_err(char* message);

void new_stderr();

char* read_line(int fd);

 
#endif