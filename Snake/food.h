
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
