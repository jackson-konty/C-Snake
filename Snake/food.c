#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include "food.h"


//Create new food
Food* create_food(int x, int y, enum Type type, enum Type2 type2){
    Food* new_food = malloc(sizeof(Food));

    new_food->x = x;
    new_food->y = y;
    if (type == Increase){
        if(type2 == Scored){new_food->type = 'O';}
        else{new_food->type = '+';}
    }
    else if(type == Decrease){
        if(type2 == Scored){new_food->type = 'X';}
        else{new_food->type = '-';}

    }
    new_food->next = NULL;

    return new_food;
}

//Check if food exists at coordinates
bool food_exists(Food* foods, int x, int y){
    Food* temp = foods;
    while(temp){
        if(temp->x == x && temp->y == y)
            return true;
        temp = temp->next;
    }
    return false;
}

//Add new food to end of food list
void add_new_food(Food* foods, Food* new_food){
    Food* temp = foods;
    while(temp->next) {
        temp = temp->next;
    }
    temp->next = new_food;
}


enum Type remove_eaten_food(Food* foods, int x, int y){
    Food* temp = foods;
    Food* preTemp =NULL;
    int prevent = 0;
    while(temp){
        if(temp->x == x && temp->y == y){
           if(prevent){
                preTemp->next = temp->next;
           }
           else{foods = temp->next;}
           return temp->type;
        }
        preTemp = temp;
        temp = temp->next;
        prevent = 1;
    }

}

// Display all the food
void draw_food (Food *foods)
{   Food* temp = foods;
    while(temp) {
        int N;
        init_pair(15,COLOR_YELLOW,COLOR_BLACK);
        init_pair(16,COLOR_GREEN,COLOR_BLACK);
        if(temp->type=='O' || temp->type=='+'){N=16;}else{N=15;}
        attron(COLOR_PAIR(N));
        mvprintw(temp->y, temp->x, "%c", temp->type);
        attroff(COLOR_PAIR(N));
        temp = temp->next;
    }
}
