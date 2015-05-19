#include "menu.h"

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
    printf_line("press enter to continue",18,5);
    printf_line("PLAYER 1:\tMOVE: Z/Q/S/D\n\tBOMB: A", 10, 5);
    printf_line("PLAYER 2:\tMOVE: UP/LEFT/DOWN/RIGHT\n\tBOMB: \"END\"", 13, 5);
    char* quit = malloc(1);
    int ret;
    do{
        ret = read(0,quit,1);
        if(ret == -1){
            perror("print_commande : read");
            exit(-1);
        }
    }while(*quit != ENTER);
}
//let the player choose the folder to launch the game
void wich_folder(){
    print_line("write the name of the folder : ",10,5);
    term_cannonique();
    print_line2(CUR_ON);
    char* buff = malloc(100);
    int i = read(0,buff,100);
    term_raw();
    print_line2(CUR_OFF);
    buff[i-1] = '\0';
    launch_game(buff);
    free(buff);
}

void RandomlyCreated(){
    player p1 = create_player(1);
    player p2 = create_player(2);
    board map = random_map();
    spawn(p1, map);
    spawn(p2, map);
    print_map(map,p1,p2);
    mainGame(p1, p2, map);
}

//////////////////////////////////////////////////////////
//                          MENU                        //
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

    print_line_("->",4, pos_x, pos_y,RED,BOLD);
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
        print_line_("->",4, pos_x + x, pos_y + y,RED,BOLD);
        if(*direction == ENTER){
            print_line(CLEAR_AFTER_CUR,pos_x,pos_y);
            if(m->items[x]->kind == 1){
                launchMenu(m->items[x]->item_kind.submenu,pos_x,pos_y);
            }else{
                if(m->items[x]->item_kind.action->kind == 1){
                    m->items[x]->item_kind.action->action_kind.f();
                }else{
                    m->items[x]->item_kind.action->action_kind.f2(m->items[x]->item_kind.action->name);
                }
            }
            for(int i = 0 ; i < nb_item ; i++){
                if(m->items[i]->kind == 1){
                    print_line(m->items[i]->item_kind.submenu->name, pos_x+i, pos_y+2);
                }else{
                    print_line(m->items[i]->item_kind.action->name, pos_x+i, pos_y+2);
                }
            }
            print_menu(m,pos_x,pos_y);
            print_line_("->",4,pos_x+x,pos_y+y,RED,BOLD);
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
        if(m->items[i]->kind == 1){
            print_line(m->items[i]->item_kind.submenu->name, pos_x+i, pos_y+2);
        }else{
            print_line(m->items[i]->item_kind.action->name, pos_x+i, pos_y+2);
        }
    }
}

menu menuINIT() {
    menu main_ = createMenu("main");
    menu game = createMenu("Start");
    addSubMenu(main_, game);
    addMenuAction1(main_, "Commands", print_commande);
    addMenuAction1(main_, "Exit", leave);

    addMenuAction2(game, "mod1", launch_game);
    addMenuAction2(game, "mod2", launch_game);
    addMenuAction2(game, "Randomly generated", RandomlyCreated);
    addMenuAction1(game, "Custom", wich_folder);
    return main_;
}