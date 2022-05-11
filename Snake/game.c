#include <math.h>
#include "menu.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game_window.h"
#include "key.h"
#include "game.h"
#include "obstic.h"
void generate_points(int *food_x, int *food_y, int width, int height, int x_offset, int y_offset){
    *food_x = rand() % (width-2) + x_offset+1;
    *food_y = rand() % (height-2) + y_offset+1;
}

void game(){
    srand((unsigned int)time(NULL));
    enum State state = INIT; // Set the initial state
    static int x_max, y_max; //Max screen size variables
    static int x_offset, y_offset; // distance between the top left corner of your screen and the start of the board
    gamewindow_t *window; // Name of the board
    Snake *snake; // The snake
    Snake *enemy; //The OTHER snake
    Food *foods,*new_food; // List of foods (Not an array)
    Obstic *obstics;
    char food_eaten;
    const int height = 30;
    const int width = 70;
    int window_change = 0;
    char ch;
    char pch;
    char diff;
    int life = 3;
    int init_length;
    int death_pause = 0;
    int pdirection = 1;
    struct timespec timeret;
    timeret.tv_sec = 0;
    timeret.tv_nsec = 999999999/4;
    initscr();
    noecho();
    move(0,0);
    printw("This is snake!");
    move(1,0);
    printw("Basic rules:");
    move(2,0);
    printw("Dont run into a wall, yourself, or the obsticals marked by /");
    move(3,0);
    printw("If you eat the O food you will grow and your score will increase by 20");
    move(4,0);
    printw("If you eat + food will grow but WILL NOT increase your score, avoid this food");
    move(5,0);
    printw("If you eat the X food you will shrink and your score will decrease by 10");
    move(6,0);
    printw("if you eat - food you will shrick but you score WILL NOT decrease, this is good in some situations");
    move(7,0);
    printw("There is an enemy snake, if you touch this you will die and it will grow");
    move(8,0);
    printw("Controls:");
    move(9,0);
    printw("Use the arrow keys to move");
    move(10,0);
    printw("While in game you can press Q/q to quit and P/p to pause");
    move(11,0);
    printw("While paused you can press F/f to save to state of your game to come back to later");
    move(12,0);
    printw("Use + and - to increase or decrease the size of the window");
    move(13,0);
    printw("Enter e for easy h for hard and a for advanced");
    move(14,0);
    while(ch!='e'&&ch!='h'&&ch!='a'){
        ch = get_char();
        if(ch == 'Q'||ch == 'q'){state = EXIT;break;}
            diff=ch;
    }
    endwin();
    int food_count, ob_count, ob_size;
    float speed0,speed;
    switch(diff){
        case 'e':
            food_count = 10;
            ob_count = 3;
            ob_size = 7;
            speed0 = 1;
            break;
        case 'h':
            food_count = 20;
            ob_count = 6;
            ob_size = 14;
            speed0 = 2;
            break;
        case 'a':
            food_count = 40;
            ob_count = 10;
            ob_size = 28;
            speed0 = 4;
        break;
        default:
        food_count = 10;
        ob_count = 3;
        ob_size = 7;
            speed0 = 4;
            break;

    }
    while(state != EXIT){
        switch(state){
        case INIT:
            initscr();
            start_color();
            init_pair(50,COLOR_RED, COLOR_BLACK);
            nodelay(stdscr, TRUE); //Dont wait for char
            noecho(); // Don't echo input chars
            getmaxyx(stdscr, y_max, x_max);
            keypad(stdscr, TRUE); // making keys work
            curs_set(0); // hide cursor
            timeout(100);

            // Setting height and width of the board
            x_offset = (x_max / 2) - (width / 2);
            y_offset = (y_max / 2) - (height / 2);

            //Init board
            window = init_GameWindow(x_offset, y_offset, width, height);
            bkgd(COLOR_PAIR(50));
            draw_Gamewindow(window);

            // Init snake
            snake = init_snake(x_offset + (width / 2), y_offset + (height / 2)-1);
            enemy = init_snake(10 + x_offset + (width/2), 10 + y_offset + (height/2));
            // Init foods
            int food_x, food_y, i, j;
            enum Type type;
            enum Type2 type2;

            //Generate 10 foods
            generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
            type = (rand() > RAND_MAX/2) ? Increase : Decrease; // Randomly deciding type of food
            type2 = Scored;
            foods = create_food(food_x, food_y, type, type2);
            for(i = 1; i < food_count; i++){
                generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                while (food_exists(foods,food_x, food_y)){
                    generate_points(&food_x, &food_y, width, height, x_offset, y_offset);}
                type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                new_food = create_food(food_x, food_y, type, type2);
                add_new_food(foods, new_food);
            }



            // Init Obsticals
            int obstic_x, obstic_y;
            int sym = 1;
            generate_points(&obstic_x,&obstic_y,width,height,x_offset,y_offset);
            obstics = create_Obstic(obstic_x, obstic_y,sym%2);sym++;
                Obstic* temp = obstics;
            for(i=0;i<ob_count;i++){
                generate_points(&obstic_x,&obstic_y,width,height,x_offset,y_offset);
                    temp->next = create_Obstic(obstic_x,obstic_y,sym%2);sym++;
                        temp = temp->next;
                int size = 1+(rand()%ob_size);
                for(j=0;j<size;j++){
                            int direction = rand()%4;
                            switch(direction){
                                    case 1: obstic_x--;break;
                        case 2: obstic_x++;break;
                        case 3: obstic_y--;break;
                        default: obstic_y++;break;
                            }
                        temp->next = create_Obstic(obstic_x,obstic_y,sym%2);sym++;
                        temp = temp->next;
                }
            }

            int score = 0;
            state = ALIVE;
            break;
        case ALIVE:
            init_length = len(snake);
            pch = ch;
            ch = get_char();
            if (ch == ERR){ch = pch;}
            if (ch == 'p'||ch == 'P'||death_pause){
                ch = ERR;
                int quit = game_menu();
                if(quit == 1){state = EXIT;break;}
                ch = pch;
            }
            death_pause = 0;
                /*   if(ch == 'f'||ch == 'F'){
                            FILE *cfPtr;
                            int N = 1;
                            char filename[] = "saves/save_1.game";
                            //while((cfPtr = fopen(("saves/save_%d.game",N),"rb"))!= NULL){N++;}
                            //close(cfPtr);
                            cfPtr = fopen(filename,"w");
                            fprintf(cfPtr,"Success");
                            fwrite(&score,sizeof(int),1,cfPtr);
                            fwrite(&window_change,sizeof(int),1,cfPtr);
                            Snake *temp = snake;
                            while(temp){
                                Snake *temp = snake;
                                fwrite(&temp,sizeof(struct Snake),1,cfPtr);
                                temp = temp->next;
                            }
                Obstic *temp = obstics;
                while(obstics){
                    fwrite(&temp,sizeof(struct Obstic),1,cfPtr);
                    temp = temp->next;
                }
                        close(cfPtr);
            ch = 'p'
                    }*/


            if(ch=='Q'||ch=='q'){
                state = EXIT;
                break;
            }
            if((ch == '+'||ch == '=')&&window_change<3){
                window_change++;
                window = init_GameWindow(x_offset, y_offset, width+(5*window_change), height+(5*window_change));
                ch = pch;
            }
            else if((ch == '-'||ch =='_')&&window_change>-3){
                window_change--;
                window = init_GameWindow(x_offset, y_offset, width+(5*window_change), height+(5*window_change));
                ch = pch;
            }
            if((ch==DOWN&&pch==UP)||(ch==UP&&pch==DOWN)||(ch==LEFT&&pch==RIGHT)||(ch==RIGHT&&pch==LEFT)){ch=pch;}
            snake = move_snake(snake,ch);
            if(food_exists(foods,snake->x,snake->y)){
                food_eaten = remove_eaten_food(foods,snake->x,snake->y);
                while (food_exists(foods,food_x, food_y)){
                    generate_points(&food_x, &food_y, width+(5*window_change), height+(5*window_change), x_offset, y_offset);
                }
                type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                type2 = (rand() > RAND_MAX/4) ? Scored : Unscored;
                new_food = create_food(food_x, food_y, type, type2);
                add_new_food(foods, new_food);
                if(food_eaten == 'O'||food_eaten == '+'){
                            if(food_eaten == 'O'){score += 20;}
                    Snake* temp = snake;
                    int tailx,taily;
                    int j;
                    for(j=1;j<=len(snake);j++){
                        if(j == len(snake)){
                            tailx = temp->x;
                            taily = temp->y;
                            if(ch==UP){taily--;}
                            else if(ch==DOWN){taily++;}
                            else if(ch==RIGHT){tailx--;}
                            else if(ch==LEFT){tailx++;}
                            temp->next = create_tail(tailx,taily);
                        }
                        else{temp=temp->next;}
                    }
                }
                else if(food_eaten == 'X'||food_eaten == '-'){
                    if(len(snake) == 1){state = DEAD;break;}
                    if(food_eaten == 'X'){score -= 10;};
                    snake = remove_tail(snake);

                }
            }
            if(snake->x<=x_offset||snake->y<=y_offset||snake->x>=(x_offset+width+window_change*5)||snake->y>=(y_offset+height+window_change*5)){
                state = DEAD;
                break;
            }
            if(len(snake)>3&&eat_itself(snake)){
                state = DEAD;
                break;
            }
            if(obstic_exists(obstics,snake->x,snake->y)){
                state = DEAD;
                break;
            }
            speed = speed0*pow(1.5,floor(score/100));
            int direction;
            int has_moved = 0;
            direction = (rand() > RAND_MAX/7) ? pdirection : rand()%4;
            pdirection = direction;
            switch(direction){
                case 0:
                    if(y_offset+1 < enemy->y){
                        enemy = move_snake(enemy,UP);
                        break;}
                    else{enemy = move_snake(enemy,DOWN);pdirection=1;break;}
                case 1:
                    if(y_offset+height+(5*window_change)-1 > enemy->y){
                        enemy = move_snake(enemy,DOWN);
                        break;}
                    else{enemy = move_snake(enemy,UP);pdirection = 0;break;}
                case 2:
                    if(x_offset+3 < enemy->x){
                        enemy = move_snake(enemy,LEFT);
                        break;}
                    else{enemy = move_snake(enemy,RIGHT);pdirection = 3;break;}
                case 3:
                    if(x_offset+width+(5*window_change)-3 > enemy->x){
                        enemy = move_snake(enemy,RIGHT);
                        break;}
                    else{enemy = move_snake(enemy,LEFT);pdirection = 2;break;}
                }
            if(food_exists(foods,enemy->x,enemy->y)){
                food_eaten = remove_eaten_food(foods,enemy->x,enemy->y);
                while (food_exists(foods,food_x, food_y)){
                    generate_points(&food_x, &food_y, width+(5*window_change), height+(5*window_change), x_offset, y_offset);
                }
                type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                type2 = (rand() > RAND_MAX/4) ? Scored : Unscored;
                new_food = create_food(food_x, food_y, type, type2);
                add_new_food(foods, new_food);
                if(food_eaten == 'O'||food_eaten == '+'){
                    Snake* temp = enemy;
                    int tailx,taily;
                    int j;
                    for(j=1;j<=len(enemy);j++){
                        if(j == len(enemy)){
                            tailx = temp->x;
                            taily = temp->y;
                            if(pdirection==0){taily--;}
                            else if(pdirection==1){taily++;}
                            else if(pdirection==3){tailx--;}
                            else if(pdirection==2){tailx++;}
                            temp->next = create_tail(tailx,taily);
                        }
                        else{temp=temp->next;}
                    }
                }
                else if(food_eaten == 'X'||food_eaten == '-'){
                    if(len(enemy) == 1){break;}
                    enemy = remove_tail(enemy);
                }
            }
            Snake* etemp = enemy;
            for(j=1;j<=len(enemy);j++){
                if(etemp->y==snake->y&&etemp->x==snake->x){
                    state = DEAD;
                    Snake* temp = enemy;
                    int tailx,taily;
                    int j;
                    for(j=1;j<=len(enemy);j++){
                        if(j == len(enemy)){
                            tailx = temp->x;
                            taily = temp->y;
                            if(pdirection==0){taily--;}
                            else if(pdirection==1){taily++;}
                            else if(pdirection==3){tailx--;}
                            else if(pdirection==2){tailx++;}
                            temp->next = create_tail(tailx,taily);
                        }
                        else{temp=temp->next;}
                        }
                    break;
                }
                etemp = etemp->next;
            }
            if(len(enemy)>20){remove_tail(enemy);}
            init_pair(1,COLOR_WHITE,COLOR_BLACK);
            init_pair(4,COLOR_CYAN,COLOR_CYAN);
            init_pair(2,COLOR_RED,COLOR_RED);
            init_pair(3,COLOR_RED,COLOR_BLACK);
            clear();
            nodelay(stdscr, TRUE); //Dont wait for char
            noecho(); // Don't echo input chars
            attron(COLOR_PAIR(1));
            mvprintw(20,20, "Key entered: %c", ch);
            mvprintw(18,20, "Score: %d", score);
            mvprintw(16,20, "Lives: %d", life);
            attroff(COLOR_PAIR(1));
            attron(COLOR_PAIR(50));
            draw_Gamewindow(window);
            attroff(COLOR_PAIR(50));
            draw_food(foods);
            attron(COLOR_PAIR(3));
            draw_Obstics(obstics);
            attroff(COLOR_PAIR(3));
            attron(COLOR_PAIR(2));
            draw_snake(enemy);
            attroff(COLOR_PAIR(2));
            attron(COLOR_PAIR(4));
            draw_snake(snake);
            attroff(COLOR_PAIR(4));
            break;
        case DEAD:
            if(life>1){
                int starty = y_offset + (height/2) - 1;
                int startx = x_offset + (width/2);
                snake = create_tail(startx, starty);
                Snake* temps = snake;
                for(i=1;i<init_length;i++){
                    temps->next = create_tail(startx-i,starty);
                    temps = temps->next;
                }
                state = ALIVE;
                life = life -1;
                clear();
                nodelay(stdscr, TRUE); //Dont wait for char
                noecho(); // Don't echo input chars
                attron(COLOR_PAIR(1));
                mvprintw(20,20, "Key entered: %c", ch);
                mvprintw(18,20, "Score: %d", score);
                mvprintw(16,20, "Lives: %d", life);
                attron(COLOR_PAIR(50));
                draw_Gamewindow(window);
                attroff(COLOR_PAIR(50));
                attroff(COLOR_PAIR(1));
                attron(COLOR_PAIR(4));
                draw_snake(snake);
                attroff(COLOR_PAIR(4));
                attron(COLOR_PAIR(2));
                draw_snake(enemy);
                attroff(COLOR_PAIR(2));
                draw_food(foods);
                attron(COLOR_PAIR(3));
                draw_Obstics(obstics);
                attroff(COLOR_PAIR(3));
                death_pause = 1;
                ch = RIGHT;
                break;
            }
            else{
                FILE *cfPtr;
                cfPtr = fopen("saves/10_best_scores","a+");
                putw(score,cfPtr);
                fclose(cfPtr);
                initscr();
                noecho();
                move(0,0);
                printw("GAME OVER");
                move(1,0);
                printw("Press q to quit");
                while(true){
                    ch = get_char();
                    if(ch == 'Q'||ch == 'q'){state = EXIT;break;}
                }
            }
        }
    refresh();
    timeret.tv_nsec = 999999999/(4*speed);
    nanosleep(&timeret, NULL);
    }
    endwin();
}
