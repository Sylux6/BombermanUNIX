#include "menu.h"
#include "term.h"
#include "mylib.h"
#include "map.h"


#define CLEAR_AFTER_CUR "\x1b[0J"
#define CLEAR_BEFOR_CUR "\x1b[1J"
#define CLEAR_TERM "\x1b[2J\x1b[;H"
#define CUR_ON "\x1b[?25h"
#define CUR_OFF "\x1b[?25l"

#define ENTER 13
#define DEL 127

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

void print_commande(){
    print_line("joueur 1 : deplacement -> zqsd , drop bombe -> a\n", 14, 5);
    print_line("joueur 2 : deplacement -> pad arrow  , drop bombe -> \"end\"\n", 16, 5);
    sleep(5);
}

void wich_folder(){
    // let him chose a folder to start the game
    //call launch_game if all is good
    // i 27906 , o 5 c 1215 l 35387
    // struct termios not_raw;
    // not_raw.c_iflag = 27906;
    // not_raw.c_oflag = 5    ;
    // not_raw.c_cflag = 1215 ;
    // not_raw.c_lflag = 35387;
    term_cannonique();
    print_line(CUR_ON,10,5);
    char* buff = malloc(100);
    int i = read(0,buff,100);
    term_raw();
    buff[i] = '\0';
    launch_game(buff);
    free(buff);


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

void addMenuAction1(menu m, const char* text, void (*f)()) {
    int i;
    for(i = 0; i < 9; i++) {
        if(m->items[i] == NULL) {
            m->items[i] = malloc(sizeof(struct item));
            m->items[i]->kind = 2;
            action newAction = malloc(sizeof(struct action));
            newAction->name = mystrcpy(newAction->name, text);
            newAction->kind = 1;
            newAction->action_kind.f = f;
            // newAction->f = f;
            m->items[i]->item_kind.action = newAction;
            return;
        }
    }
    printf("\nerror: menu is already full...\n\n");
}

void addMenuAction2(menu m, const char* text, void (*f)(char*)) {
    int i;
    for(i = 0; i < 9; i++) {
        if(m->items[i] == NULL) {
            m->items[i] = malloc(sizeof(struct item));
            m->items[i]->kind = 2;
            action newAction = malloc(sizeof(struct action));
            newAction->name = mystrcpy(newAction->name, text);
            newAction->kind = 2;
            newAction->action_kind.f2 = f;
            // newAction->f = f;
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

int numberOfItems(menu m){
    int i=0;
    while(m->items[i] != NULL){
        i++;
    }
    // for(int i=0; i < 9 ; i++){
    //     if(m->items[i] == NULL) break;
    // }
    return i;
}
void launchMenu(menu m, int pos_x, int pos_y) {
    
    int nb_item = numberOfItems(m),ret,x = 0,y = 0;
    char direction[1];
    // selection de l'item
    print_menu(m,pos_x,pos_y);

    print_line("->", pos_x, pos_y);
    while(1){
        ret = read(0, direction, 1);
        if(ret < 0){
            exit(-1);
        }
        print_line("  ",pos_x + x, pos_y + y);
        if(*direction == UP_ARROW){
            x = mymodulo(x-1,nb_item);
        }
        if(*direction == DOWN_ARROW){
            x = mymodulo(x+1,nb_item);
        }
        print_line("->", pos_x + x, pos_y + y);
        if(*direction == ENTER){
            print_line(CLEAR_AFTER_CUR,pos_x,pos_y);
            if(m->items[x]->kind == 1){
                //clear and launch the new menu
                launchMenu(m->items[x]->item_kind.submenu,pos_x,pos_y);

            }else{
                if(m->items[x]->item_kind.action->kind == 1){
                    m->items[x]->item_kind.action->action_kind.f();
                }else{
                    m->items[x]->item_kind.action->action_kind.f2(m->items[x]->item_kind.action->name);
                }
            }
            for(int i = 0 ; i < nb_item ; i++){
                //if menu ->
                if(m->items[i]->kind == 1){
                    print_line(m->items[i]->item_kind.submenu->name, pos_x+i, pos_y+2);
                }else{
                    print_line(m->items[i]->item_kind.action->name, pos_x+i, pos_y+2);
                }
            }
            // print_line(CUR_OFF,pos_x,pos_y);
            print_menu(m,pos_x,pos_y);
            print_line("->",pos_x+x,pos_y+y);
        }
        if(*direction == DEL){
            if(m->parent != NULL){
                print_line(CLEAR_AFTER_CUR,pos_x,pos_y);
                launchMenu(m->parent,pos_x,pos_y);
                return;
            }
        }
    }
}

void print_menu(menu m, int pos_x, int pos_y){
    print_line2(CLEAR_TERM);
    print_line2(CUR_OFF);
    print_title(1,10);
    int nb_item = numberOfItems(m);
    if(atoi(getenv("LINES")) < pos_x+nb_item){
        my_print_err("impossible to print menu : height too small ");
    }
    //affichage des sous menu et action
    for(int i = 0 ; i < nb_item ; i++){
        //if menu ->
        if(m->items[i]->kind == 1){
            print_line(m->items[i]->item_kind.submenu->name, pos_x+i, pos_y+2);
        }else{
            print_line(m->items[i]->item_kind.action->name, pos_x+i, pos_y+2);
        }
    }
}