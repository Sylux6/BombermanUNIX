#include "menu.h"
#include "term.h"
#include "mylib.h"
#include "map.h"

#define ENTER 13
#define UP_ARROW 65
#define DOWN_ARROW 66
#define RIGHT_ARROW 67
#define LEFT_ARROW 68

void print_title(int x,int y){
	char* line[5] = {"    ____                  __                                  ",
					"   / __ )____  ____ ___  / /_  ___  _________ ___  ____ _____ ",
					"  / __  / __ \\/ __ `__ \\/ __ \\/ _ \\/ ___/ __ `__ \\/ __ `/ __ \\",	
					" / /_/ / /_/ / / / / / / /_/ /  __/ /  / / / / / / /_/ / / / /",
					"/_____/\\____/_/ /_/ /_/_.___/\\___/_/  /_/ /_/ /_/\\__,_/_/ /_/ "};		

	int i;
	char* buffer = malloc(100);
	for(i = 0; i < 5 ; i++){
		sprintf(buffer, "\x1b[%d;%dH%s", x, y,line[i]);
		write(1,buffer,strlen(buffer));
		x++;
	}
	free(buffer);
}

//////////////////////////////////////////////////////////
//							MENU						//
//////////////////////////////////////////////////////////

menu createMenu(const char* text) {
    menu newMenu = malloc(sizeof(struct menu));
    newMenu->parent = NULL;
    newMenu->name = mystrcpy(newMenu->name, text);
    itemInit(newMenu->items);
    return newMenu;
}

void itemInit(item i[9]) {
    int j;
    for(j = 0; j < 9; j++)
        i[j] = NULL;
}

void addMenuAction(menu m, const char* text, void (*f)()) {
    int i;
    for(i = 0; i < 9; i++) {
        if(m->items[i] == NULL) {
            m->items[i] = malloc(sizeof(struct item));
            m->items[i]->kind = 2;
            action newAction = malloc(sizeof(struct action));
            newAction->f = f;
            newAction->name = mystrcpy(newAction->name, text);
            m->items[i]->item_kind.action = newAction;
            return;
        }
    }
    printf("\nerror: menu is already full...\n\n");
}

void addSubMenu(menu m, menu sm) {
    if(sm->parent != NULL) {
        printf("\nerror: the submenu you want to add has already a parent...\n\n");
        return;
    }
    sm->parent = m;
    int i;
    for(i = 0; i < 9; i++) {
        if(m->items[i] == NULL) {
            m->items[i] = malloc(sizeof(struct item));
            m->items[i]->kind = 1;
            m->items[i]->item_kind.submenu = sm;
            return;
        }
    }
    printf("\nerror: menu is already full...\n\n");
}

void deleteMenu(menu m) {
    if(m->parent != NULL) {
        printf("\nerror: you can not delete a menu which has a parent...\n\n");
        return;
    }
    int i;
    for(i = 0; i < 9; i++) {
        if(m->items[i] == NULL)
            break;
        else if(m->items[i]->kind == 1) {
            free(m->items[i]);
        }
        else if(m->items[i]->kind == 2) {
            free(m->items[i]->item_kind.action);
            free(m->items[i]);
        }
        m->items[i] = NULL;
    }
    free(m->name);
    free(m);
    m->name = NULL;
    m = NULL;
}

int choiceToInt(char c) {
    //the value of choice is reduced by 1 to match the values of items[]
    switch(c) {
    case '1':
        return 0;
        break;
    case '2':
        return 1;
        break;
    case '3':
        return 2;
        break;
    case '4':
        return 3;
        break;
    case '5':
        return 4;
        break;
    case '6':
        return 5;
        break;
    case '7':
        return 6;
        break;
    case '8':
        return 7;
        break;
    case '9':
        return 8;
        break;
    default:
        return -1;
    }
}

void launchMenu(menu m) {
    //TODO
}