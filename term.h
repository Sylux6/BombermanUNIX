#ifndef TERM
#define TERM

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

#include "mylib.h"

void print_line(char* line,int x,int y);
void print_line2(char* line);
void printf_line(char* line,int x,int y);
void term_cannonique();
void save_old();
void set_pos(int x,int y);
void term_raw();
void leave();

#endif