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

struct menu {
    struct menu* parent;
    char* name;
    struct item* items[9] ;
};
typedef struct menu* menu;

struct item {
    int kind;                   //see below
    union item_kind {
        struct menu* submenu;   //kind = 1
        struct action* action;  //kind = 2
    }item_kind;
};
typedef struct item* item;

struct action {
    char* name;
    void (*f)();
};
typedef struct action* action;

void itemInit(item[9]);

menu createMenu(const char*);

void addMenuAction(menu, const char*, void (*)());

void addSubMenu(menu , menu);

void deleteMenu(menu );

void launchMenu(menu );

void print_title(int x,int y);

#endif