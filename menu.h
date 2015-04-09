#ifndef MENU
#define MENU

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


struct menu_choice
{
	int choice;
	char* folder;
};

struct menu_choice print_menu(int pos_x,int pos_y);
void print_title(int x,int y);

#endif