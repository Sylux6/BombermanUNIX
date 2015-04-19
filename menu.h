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

#include "term.h"
#include "mylib.h"
#include "map.h"


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
    int kind;                   //see below
    union action_kind{
        void (*f)();            //kind = 1
        void (*f2)(char*);      //kind = 2
    }action_kind;
};
typedef struct action* action;

void itemInit(item[9]);

menu createMenu(const char*);

void addMenuAction1(menu, const char*, void (*)());

void addMenuAction2(menu, const char*, void (*)(char*));

void addSubMenu(menu, menu);

void deleteMenu(menu);

void launchMenu(menu, int, int);

void print_title(int, int);

void print_commande();

void wich_folder();

void print_menu(menu m, int pos_x, int pos_y);

#endif