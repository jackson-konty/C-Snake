#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "obstic.h"
#include "key.h"
#include <ncurses.h>

Obstic* create_Obstic(int x, int y,int sym){
  Obstic* obstic = malloc(sizeof(obstic));
  obstic->next = NULL;
  obstic->x = x;
  obstic->y = y;
  obstic->sym = sym;
  return obstic;
}

void draw_Obstics(Obstic *obstics){
    Obstic* temp = obstics;
    while(temp) {
        if(temp->sym == 0){mvprintw(temp->y, temp->x, "/");}
        else{mvprintw(temp->y, temp->x, "\\");}
        temp = temp->next;
    }
}

bool obstic_exists(Obstic* obstics, int x, int y){
    Obstic* temp = obstics;
    while(temp){
        if(temp->x == x && temp->y == y)
            return true;
        temp = temp->next;
    }
    return false;
}

int leno(Obstic* obstics){
    int length = 0;
    while(obstics){
        length++;
        obstics = obstics->next;
    }
    if(length==0){return 1;}
    else{return (length);}
}
/*
void obstic_chain(Obstic* obstics, int* obstic_x, int* obstic_y,Food* foods,Snake *snake){
    Obstic* temp = obstics;
    while(temp){
        if(temp->next == NULL){
            while(obstic_exists(obstics,*obstic_x,*obstic_y)||food_exists(foods,*obstic_x,*obstic_y)||(*obstic_x==snake->x&&*obstic_y==snake->y)){
                (rand() > RAND_MAX/2) ? *obstic_x++ : *obstic_y++;}
            //temp->next = create_Obstic(*obstic_x,*obstic_y);
        }
        else{temp = temp->next;}
    }
}

*/
