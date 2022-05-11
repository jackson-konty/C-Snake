#include "snake.h"
#include "food.h"



struct Obstic{
    int x;
    int y;
    struct Obstic* next;
    int sym;
};

typedef struct Obstic Obstic;

Obstic* create_Obstic(int x, int y, int sym);
void draw_Obstics(Obstic *obstics);
bool obstic_exists(Obstic* obstics, int x, int y);
int leno(Obstic* obstics);
void obstic_chain(Obstic* obstics, int* obstic_x, int* obstic_y,Food* foods,Snake *snake);
