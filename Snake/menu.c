#include <ncurses.h>

//Two types of food
//Either Increase or Decrease length of snake
enum Type {Increase, Decrease};
enum Type2{Scored,Unscored};

//Structure to hold properties of food
struct Food {
  int x;
  int y;
  char type;
  struct Food* next;
};

typedef struct Food Food;

//Function prototypes
void add_new_food(Food* foods, Food* new_food);
bool food_exists(Food* foods, int x, int y);
Food* create_food(int x, int y, enum Type type, enum Type2 type2);
enum Type remove_eaten_food(Food* foods, int x, int y);
void draw_food(Food *food);
jhkonty@silo:~/C291-Fall1-2020/snake_game$ ls
food.c  game.c  game_window.c  key.c  Makefile  menu.o    obstic.o  snake_game    snake.h
food.h  game.h  game_window.h  key.h  menu.c    obstic.c  saves     snake_game.c  snake.o
food.o  game.o  game_window.o  key.o  menu.h    obstic.h  snake.c   snake_game.o
jhkonty@silo:~/C291-Fall1-2020/snake_game$ cat menu.c
#include "menu.h"
#include <ncurses.h>
#include "key.h"
char *choices[] = {
    "Load Saved File",
    "Show 10 best scores",
    "Resume",
    "Quit",
    };
void swap(int *x, int *y);
void swap(int *x, int *y){
    int temp = *x;
    *x = *y;
    *y = temp;
}
int game_menu(){
    ITEM **my_items;
        int c;
        int quit;
        MENU *my_menu;
        int n_choices, i;
        ITEM *cur_item;
        initscr();
        start_color();
    cbreak();
    noecho();
        keypad(stdscr, TRUE);
        init_pair(21, COLOR_BLUE, COLOR_BLACK);
        init_pair(22, COLOR_YELLOW, COLOR_BLACK);
        init_pair(23, COLOR_MAGENTA, COLOR_BLACK);
        n_choices = ARRAY_SIZE(choices);
        my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
        for(i = 0; i < n_choices; ++i)
        my_items[i] = new_item(choices[i],"|");
        my_items[n_choices] = (ITEM *)NULL;
        my_menu = new_menu((ITEM **)my_items);
        set_menu_fore(my_menu, COLOR_PAIR(21) | A_REVERSE);
        set_menu_back(my_menu, COLOR_PAIR(22));
        set_menu_grey(my_menu, COLOR_PAIR(23));

        mvprintw(LINES - 3, 0, "Press <ENTER> to see the option selected");
        post_menu(my_menu);
        refresh();
        int item_sel = 0;
        int exit = 0;
        while(!exit){
            c = getch();
            switch(c){
                case KEY_DOWN:
                    menu_driver(my_menu, REQ_DOWN_ITEM);
                    if(item_sel!=3){item_sel++;}
                    break;
                case KEY_UP:
                    menu_driver(my_menu, REQ_UP_ITEM);
                    if(item_sel!=0){item_sel--;}
                    break;
                case 10:
                    switch(item_sel){
                        case 0:
                            move(10,0);
                            printw("Functionality not yet implemented");
                            break;
                        case 1:
                            move(0,40);
                            int count = 0;
                            int scores[100];
                            printw("10 best Scores");
                            FILE *cfPtr;
                            cfPtr = fopen("saves/10_best_scores","r");
                            while(!feof(cfPtr)){
                                int num = getw(cfPtr);
                                scores[count] = num;
                                count++;
                            }
                            int i,j;
                            for(i=0;i<count-1;i++){for(j=0;j<count-i-1;j++){if(scores[j]<scores[j+1]){swap(&scores[j],&scores[j+1]);}}}
                            for(i=1;i<=10;i++){mvprintw(i,40,"%d",scores[i-1]);}
                            break;
                        case 2:
                            exit = 1;
                            quit = 0;
                            break;
                        case 3:
                            exit = 1;
                            quit = 1;
                            break;
                    }
                break;
                }
        }
        unpost_menu(my_menu);
        for(i = 0; i < n_choices; ++i)
                free_item(my_items[i]);
        free_menu(my_menu);
        endwin();
        return quit;
}
