#ifndef _MYLIB_INCLUDED
#define _MYLIB_INCLUDED
 
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <unistd.h>

#define MIN(x,y) (x<y)?x:y

ssize_t mywrite(int fd, void *buf, size_t count);

ssize_t myread(int fd, void *buf, size_t count);

void *realloc(void *ptr, size_t size);

unsigned int mymodulo();
 
#endif