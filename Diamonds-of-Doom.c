#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>
#include <time.h>
#include <curses.h>


//Declare the functions
void help_dialogue();
void draw_border();
void draw_separator();
void status_display();
void game_end();
void ship_setup();
void diamondL_setup();
void diamondM_setup();
void diamondS_setup();
void diamondM_spawn();
void diamondS_spawn();
void missile_setup();
void process();
void collision_checker();
void timer();
bool collision_SnDLARGE();
bool collision_SnDMEDIUM();
bool collision_SnDSMALL();
bool collision_MnDLARGE();
bool collision_MnDMEDIUM();
bool collision_MnDSMALL();


//Global variables for life, score, min, and sec
int lives = 6;
int score = 0;
int min = 0;
int sec = 55;
int diamondCount;

//Global bools for
bool update_screen = true;
bool game_over = false;

////////////////// SPRITES
//Ship
sprite_id ship;

#define SHIP_HEIGHT 6
#define SHIP_WIDTH 7

static char * ship_image =
/**/   "   !   "
/**/   "  (*)  "
/**/   " (-*-) "
/**/   "|_____|"
/**/   "  ***  "
/**/   "   *   ";

//Diamond - LARGE
#define DIAMONDL_HEIGHT 5
#define DIAMONDL_WIDTH 5
#define MAX_D 10

sprite_id diamondL[MAX_D];

char diamondL_image[] =
/**/   "  x  "
/**/   " xxx "
/**/   "xxxxx"
/**/   " xxx "
/**/   "  x  ";

//Diamond - MEDIUM
#define DIAMONDM_HEIGHT 3
#define DIAMONDM_WIDTH 3
#define MAX_Dm 10

sprite_id diamondM[MAX_Dm];

char diamondM_image[] =
/**/   " x "
/**/   "xxx"
/**/   " x ";

//Diamond - SMALL
#define DIAMONDS_HEIGHT 1
#define DIAMONDS_WIDTH 1
#define MAX_Ds 10

sprite_id diamondS[MAX_Ds];

char diamondS_image[] =
/**/   "x";

//Missile
#define MAX_M 100

sprite_id missile[MAX_M];

#define MISSILE_HEIGHT 2
#define MISSILE_WIDTH 1

char missile_image[] =
/**/   "|"
/**/   "|";

//Delay + timer
#define DELAY (10)

timer_id watch;

bool new_timer = true;

////////////////// MAIN
int main(void){
sec = 0;

  if (true){
    setup_screen();
    help_dialogue();

    srand(get_current_time());

    draw_border();
    draw_separator();
    timer();

    ship_setup();
    diamondL_setup();
    diamondM_setup();
    diamondS_setup();
    missile_setup();

    while(!game_over){
      process();
      timer_pause(DELAY);
    }
  }
  cleanup_screen();
}

////////////////// DRAW_BOARDER
void draw_border() {
  int h = screen_height(), w = screen_width();
  char sym = '+';

  draw_line( 0, 0, w - 1, 0, sym);
	draw_line( 0, h - 1, w - 1, h - 1, sym);
	draw_line( 0, 0, 0, h - 1, sym);
	draw_line( w - 1, 0, w - 1, h - 1, sym);

  show_screen();
}

////////////////// DRAW_SEPARATOR
void draw_separator() {
  int h = screen_height(), w = screen_width();
  char sym = '-';

  draw_line(0, h * 0.1, w - 1, h * 0.1, sym);

  show_screen();
}

////////////////// STATUS_DISPLAY
void status_display() {
  int h = screen_height(), w = screen_width();

  draw_formatted(w * 0.02, h * 0.05 + 1, " Lives: %d ", lives);
  draw_formatted(w * 0.4, h * 0.05 + 1, " Time:  0%d:%d", min, sec);
  draw_formatted(w * 0.83, h * 0.05 + 1, " Score: %d", score);
  //draw_formatted(w * 0.6, h * 0.05 + 1, " Diamonds Left: %d", diamondCount);

  show_screen();
}

////////////////// HELP_DIALOGUE
void help_dialogue() {
  clear_screen();
  int h = screen_height(), w = screen_width();

  int upper_screen = h * 0.2;
  int middle_screen = h * 0.4;
  int lower_screen = h * 0.7;

  int middle_width = w * 0.35;

  while (get_char() >= 0 ) {}

  char * title = "WELCOME TO DIAMONDS OF DOOM";

  char * creator_info1 = "Creator: Mary Millar";
  char * creator_info2 = "Student Number: n9698337";

  char * instructions1 = "Use 'A' to move left";
  char * instructions2 = "Use 'D' to move right";

  char * instructions3 = "Use 'J' to shoot the left bullet";
  char * instructions4 = "Use 'K' to shoot the middle bullet";
  char * instructions5 = "Use 'L' to shoot the right bullet";

  char * instructions6 = "You will have 6 lives.";
  char * instructions7 = "Your objective is to survive as long as you can.";

  char * instructions8 = "Press 'H' at any time to access this screen.";
  char * instructions9 = "Press 'Q' at any time to quit the game.";

  char * close = "Press any key to continue.";

  draw_formatted(middle_width - 2, upper_screen - 5, title);
  draw_line(middle_width - 6, upper_screen - 7, middle_width - 6, upper_screen - 3, '*');
  draw_line(middle_width + 28, upper_screen - 7, middle_width + 28, upper_screen - 3, '*');

  draw_line(middle_width - 6, upper_screen - 7, middle_width + 28, upper_screen - 7, '*');
  draw_line(middle_width - 6, upper_screen - 3, middle_width + 28, upper_screen - 3, '*');

  draw_formatted(middle_width, upper_screen, creator_info1);
  draw_formatted(middle_width - 2, upper_screen + 2, creator_info2);

  draw_formatted(middle_width, middle_screen - 4, instructions1);
  draw_formatted(middle_width, middle_screen - 2, instructions2);

  draw_formatted(middle_width - 3, middle_screen + 2, instructions3);
  draw_formatted(middle_width - 4, middle_screen + 4, instructions4);
  draw_formatted(middle_width - 4, middle_screen + 6, instructions5);

  draw_formatted(middle_width, middle_screen + 10, instructions6);
  draw_formatted(middle_width - 9, middle_screen + 12, instructions7);

  draw_formatted(middle_width - 7, lower_screen + 1, instructions8);
  draw_formatted(middle_width - 5, lower_screen + 3, instructions9);

  draw_formatted(middle_width, lower_screen + 8, close);

  show_screen();

  wait_char();

  clear_screen();

  while ( get_char() >= 0 ) {}

  show_screen();
}

////////////////// SHIP_SETUP
void ship_setup(){
  int width = SHIP_WIDTH, height = SHIP_HEIGHT;

  int ship_xcor = screen_width() / 2, ship_ycor = screen_height() - height - 1;

  ship = sprite_create(ship_xcor, ship_ycor, width, height, ship_image);

  sprite_draw(ship);

  show_screen();
}

////////////////// DIAMONDL_setup
void diamondL_setup(){
  diamondCount = MAX_D;
  int width = DIAMONDL_WIDTH;
  int height = DIAMONDL_HEIGHT;

  int xrange = screen_width() - width - 1;
  int yrange = screen_height() * 0.1 + height / 2;

  for (int i = 0; i < MAX_D; i++) {
    int diamondL_xcor = rand() % xrange + 1;
    int diamondL_ycor = yrange;

    diamondL[i] = sprite_create(diamondL_xcor, diamondL_ycor, width, height, diamondL_image);

    sprite_draw(diamondL[i]);

    //sprite_turn(diamondL[i], rand() % 360);
    sprite_turn_to(diamondL[i], 0.1, 0.1);
  }
}

////////////////// diamondM RELATED CODE
void diamondM_setup(){
  int width = DIAMONDM_WIDTH;
  int height = DIAMONDM_HEIGHT;

  int diamondM_xcor = sprite_x(ship) + SHIP_WIDTH / 2;
  int diamondM_ycor = sprite_y(ship) - SHIP_HEIGHT / 2;

  for (int i = 0; i < MAX_Dm; i++){
    diamondM[i] = sprite_create(diamondM_xcor, diamondM_ycor, width, height, diamondM_image);

    sprite_draw(diamondM[i]);
    sprite_move_to(diamondM[i], -100 * 100, -100 * 100);

    sprite_hide(diamondM[i]);

    //sprite_turn(diamondM[i], (rand() % 90) - 45);
    sprite_turn_to(diamondM[i], 0.05, 0.05);

  }
}

void diamondM_spawn(sprite_id diamond){
  for (int i = 0; i < MAX_Dm; i++){
    if (!(diamondM[i] -> is_visible)){
      sprite_show(diamondM[i]);
      int xcor = sprite_x(diamond) + DIAMONDL_WIDTH / 2;
      int ycor = sprite_y(diamond) + DIAMONDL_HEIGHT / 2;
      sprite_move_to(diamondM[i], xcor, ycor);
      //sprite_turn(diamondM[i], (rand() % 90) - 45);
      break;
    }
  }
  // show_screen();
}

////////////////// diamondS RELATED CODE
void diamondS_setup(){
  int width = DIAMONDS_WIDTH;
  int height = DIAMONDS_HEIGHT;

  int diamondS_xcor = sprite_x(ship) + SHIP_WIDTH / 2;
  int diamondS_ycor = sprite_y(ship) - SHIP_HEIGHT / 2;

  for (int i = 0; i < MAX_Ds; i++){
    diamondS[i] = sprite_create(diamondS_xcor, diamondS_ycor, width, height, diamondS_image);

    sprite_draw(diamondS[i]);

    sprite_move_to(diamondS[i], -100 * 100, -100 * 100);

    sprite_hide(diamondS[i]);

    //sprite_turn(diamondS[i], (rand() % 90) - 45);
    sprite_turn_to(diamondS[i], 0.025, 0.025);

  }
}

void diamondS_spawn(sprite_id diamond){
  for (int i = 0; i < MAX_Ds; i++){
    if (!(diamondS[i] -> is_visible)){
      sprite_show(diamondS[i]);
      int xcor = sprite_x(diamond) + DIAMONDM_WIDTH / 2;
      int ycor = sprite_y(diamond) + DIAMONDM_HEIGHT / 2;
      sprite_move_to(diamondS[i], xcor, ycor);
      sprite_turn(diamondS[i], (rand() % 90) - 45);
      break;
    }
  }
  // show_screen();
}


////////////////// MISSILE_SETUP
void missile_setup(){
  int mis_xcor = sprite_x(ship) + SHIP_WIDTH / 2;
  int mis_ycor = sprite_y(ship) - SHIP_HEIGHT / 2;

  for (int i = 0; i < MAX_M; i++){
    missile[i] = sprite_create(mis_xcor, mis_ycor, MISSILE_WIDTH, MISSILE_HEIGHT, missile_image);
    sprite_move_to(missile[i], -100 * 100, -100 * 100);

    sprite_hide(missile[i]);

    sprite_turn_to(missile[i], 0, -1);

    show_screen();
  }
}

////////////////// COLLISION_CHECKER
void collision_checker(){

//Collision between diamonds and ships
  for (int i = 0; i < MAX_D; i++){
    if (diamondL[i] -> is_visible){
      if (collision_SnDLARGE(ship, diamondL[i])){
          if (lives > 1){
            lives -= 1;
            timer_pause(DELAY);
          	sprite_destroy(diamondL[i]);
            for (int j = 0; j < MAX_M; j++){
              sprite_destroy(missile[j]);
            }
            missile_setup();
            diamondL_setup();
            diamondM_setup();
            diamondS_setup();
            ship_setup();
          }
          else if (lives == 1){
            game_end();
          }
      }
    }
  }

  for (int i = 0; i < MAX_Dm; i++){
    if (diamondM[i] -> is_visible){
      if (collision_SnDMEDIUM(ship, diamondM[i])){
          if (lives > 1){
            lives -= 1;
            timer_pause(DELAY);
            sprite_destroy(diamondM[i]);
            for (int j = 0; j < MAX_M; j++){
              sprite_destroy(missile[j]);
            }
            missile_setup();
            diamondL_setup();
            diamondM_setup();
            diamondS_setup();
            ship_setup();
          }
          else if (lives == 1){
            game_end();
          }
      }
    }
  }

  for (int i = 0; i < MAX_Ds; i++){
    if (diamondS[i] -> is_visible){
      if (collision_SnDSMALL(ship, diamondS[i])){
          if (lives > 1){
            lives -= 1;
            timer_pause(DELAY);
            sprite_destroy(diamondS[i]);
            for (int j = 0; j < MAX_M; j++){
              sprite_destroy(missile[j]);
            }
            missile_setup();
            diamondL_setup();
            diamondM_setup();
            diamondS_setup();
            ship_setup();
          }
          else if (lives == 1){
            game_end();
          }
      }
    }
  }




//Collision between diamonds and missiles
  for (int i = 0; i < MAX_D; i++){
    if (diamondL[i] -> is_visible){
      for (int j = 0; j < MAX_M; j++){
        if (collision_MnDLARGE(missile[j], diamondL[i])){
          score += 1;
          sprite_move_to(missile[j], -100 * 10, -100 * 10);
          diamondM_spawn(diamondL[i]);
          timer_pause(DELAY);
        	sprite_hide(diamondL[i]);
        }
      }
    }
  }

  for (int i = 0; i < MAX_Dm; i++){
    if (diamondM[i] -> is_visible){
      for (int j = 0; j < MAX_M; j++){
        if (collision_MnDMEDIUM(missile[j], diamondM[i])){
          score += 1;
          diamondS_spawn(diamondM[i]);
          sprite_move_to(missile[j], -100 * 10, -100 * 10);
          timer_pause(DELAY);
          sprite_hide(diamondM[i]);
        }
      }
    }
  }

  for (int i = 0; i < MAX_Ds; i++){
    if (diamondS[i] -> is_visible){
      for (int j = 0; j < MAX_M; j++){
        if (collision_MnDSMALL(missile[j], diamondS[i])){
          score += 1;
          diamondCount -= 1;
          sprite_move_to(missile[j], -100 * 10, -100 * 10);
          timer_pause(DELAY);
          sprite_hide(diamondS[i]);
          if (diamondCount == 0){
            diamondL_setup();
            diamondM_setup();
            diamondS_setup();
          }
        }
      }
    }
  }




}
////SHIP COLLISION_LARGE
bool collision_SnDLARGE(sprite_id ship, sprite_id diamond){

  bool collided = true;

  int ship_top = round(sprite_y(ship));
  int ship_bottom = ship_top + SHIP_HEIGHT - 1;
  int ship_left = round(sprite_x(ship));
  int ship_right = ship_left + SHIP_WIDTH - 1;

  int diamond_top = round(sprite_y(diamond));
  int diamond_bottom = diamond_top + DIAMONDL_HEIGHT - 1;
  int diamond_left = round(sprite_x(diamond));
  int diamond_right = diamond_left + DIAMONDL_WIDTH - 1;

  if (diamond_bottom < ship_top) collided = false;
  else if (diamond_top > ship_bottom) collided = false;
  else if (diamond_right < ship_left) collided = false;
  else if (diamond_left > ship_right) collided = false;

  return collided;

}

////SHIP COLLISION_MEDIUM
bool collision_SnDMEDIUM(sprite_id ship, sprite_id diamond){

  bool collided = true;

  int ship_top = round(sprite_y(ship));
  int ship_bottom = ship_top + SHIP_HEIGHT - 1;
  int ship_left = round(sprite_x(ship));
  int ship_right = ship_left + SHIP_WIDTH - 1;

  int diamond_top = round(sprite_y(diamond));
  int diamond_bottom = diamond_top + DIAMONDM_HEIGHT - 1;
  int diamond_left = round(sprite_x(diamond));
  int diamond_right = diamond_left + DIAMONDM_WIDTH - 1;

  if (diamond_bottom < ship_top) collided = false;
  else if (diamond_top > ship_bottom) collided = false;
  else if (diamond_right < ship_left) collided = false;
  else if (diamond_left > ship_right) collided = false;

  return collided;

}

////SHIP COLLISION_SMALL
bool collision_SnDSMALL(sprite_id ship, sprite_id diamond){

  bool collided = true;

  int ship_top = round(sprite_y(ship));
  int ship_bottom = ship_top + SHIP_HEIGHT - 1;
  int ship_left = round(sprite_x(ship));
  int ship_right = ship_left + SHIP_WIDTH - 1;

  int diamond_top = round(sprite_y(diamond));
  int diamond_bottom = diamond_top + DIAMONDS_HEIGHT - 1;
  int diamond_left = round(sprite_x(diamond));
  int diamond_right = diamond_left + DIAMONDS_WIDTH - 1;

  if (diamond_bottom < ship_top) collided = false;
  else if (diamond_top > ship_bottom) collided = false;
  else if (diamond_right < ship_left) collided = false;
  else if (diamond_left > ship_right) collided = false;

  return collided;

}

////MISSILE COLLISION_LARGE
bool collision_MnDLARGE(sprite_id missile, sprite_id diamond){
  bool collided = true;

  int missile_top = round(sprite_y(missile));
  int missile_bottom = missile_top + MISSILE_HEIGHT - 1;
  int missile_left = round(sprite_x(missile));
  int missile_right = missile_left + MISSILE_WIDTH - 1;

  int diamond_top = round(sprite_y(diamond));
  int diamond_bottom = diamond_top + DIAMONDL_HEIGHT - 1;
  int diamond_left = round(sprite_x(diamond));
  int diamond_right = diamond_left + DIAMONDL_WIDTH - 1;

  if (diamond_bottom < missile_top) collided = false;
  else if (diamond_top > missile_bottom) collided = false;
  else if (diamond_right < missile_left) collided = false;
  else if (diamond_left > missile_right) collided = false;

  return collided;

}

////MISSILE COLLISION_LARGE
bool collision_MnDMEDIUM(sprite_id missile, sprite_id diamond){
  bool collided = true;

  int missile_top = round(sprite_y(missile));
  int missile_bottom = missile_top + MISSILE_HEIGHT - 1;
  int missile_left = round(sprite_x(missile));
  int missile_right = missile_left + MISSILE_WIDTH - 1;

  int diamond_top = round(sprite_y(diamond));
  int diamond_bottom = diamond_top + DIAMONDM_HEIGHT - 1;
  int diamond_left = round(sprite_x(diamond));
  int diamond_right = diamond_left + DIAMONDM_WIDTH - 1;

  if (diamond_bottom < missile_top) collided = false;
  else if (diamond_top > missile_bottom) collided = false;
  else if (diamond_right < missile_left) collided = false;
  else if (diamond_left > missile_right) collided = false;

  return collided;

}

////MISSILE COLLISION_SMALL
bool collision_MnDSMALL(sprite_id missile, sprite_id diamond){
  bool collided = true;

  int missile_top = round(sprite_y(missile));
  int missile_bottom = missile_top + MISSILE_HEIGHT - 1;
  int missile_left = round(sprite_x(missile));
  int missile_right = missile_left + MISSILE_WIDTH - 1;

  int diamond_top = round(sprite_y(diamond));
  int diamond_bottom = diamond_top + DIAMONDS_HEIGHT - 1;
  int diamond_left = round(sprite_x(diamond));
  int diamond_right = diamond_left + DIAMONDS_WIDTH - 1;

  if (diamond_bottom < missile_top) collided = false;
  else if (diamond_top > missile_bottom) collided = false;
  else if (diamond_right < missile_left) collided = false;
  else if (diamond_left > missile_right) collided = false;

  return collided;

}

////////////////// GAME_END
void game_end(){
  for ( int i = 0; i < MAX_D; i++) {
		sprite_hide(diamondL[i]);
	}

  sprite_hide(ship);

  for ( int i = 0; i < MAX_M; i++) {
		sprite_hide(missile[i]);
	}

  clear_screen();

  bool key_press = false;

  int h = screen_height(), w = screen_width();

  char * question = "Would you like to play again?";
  char * choice = "Y or N";

  draw_formatted(w * 0.5 - 15, h * 0.3, "Game over. Your score was %d.", score);
  draw_formatted(w * 0.5 - 15, h * 0.5 - 6, question);
  draw_formatted(w * 0.5 - 10, h * 0.6 - 5, "Press %s",choice);

  while ( get_char() >= 0 ) {}

  while(!key_press){
    int key = get_char();
    show_screen();

    if (key == 'y' || key == 'Y'){
      help_dialogue();
      lives = 6;
      score = 0;
      game_over = false;
      key_press = true;
      main();
    }

    else if (key == 'n' || key == 'N'){
      game_over = true;
      key_press = true;
    }
  }
}

void timer(){
  if (new_timer){
    watch = create_timer(1000);
    new_timer = false;
  }

  if (timer_expired(watch)){
    sec++;
  }


  if (sec == 60){
    sec = 0;
    min++;
  }



}


////////////////// PROCESS
void process(){
  int key = get_char();

  //OTHER SCREEN CODE
  if (key == 'h' || key == 'H'){
    help_dialogue();
  }

  else if (key == 'q' || key == 'Q'){
    game_end();
  }

  //TIMER RELATED CODE
  timer();

//SHIP RELATED CODE
  int sx = sprite_x(ship);

  // Moving the ship left
  if ((key == 'a' || key == 'A') && sx > 1){
    sprite_move(ship, -1, 0);
  }

  // Moving the ship right
  else if ((key == 'd' || key == 'D') && sx < screen_width() - 1 - SHIP_WIDTH) {
		sprite_move(ship, +1, 0);
	}

//DIAMOND RELATED CODE
  // Testing the diamond [LARGE] movement
  else if (key == 'z' || key < 0){
    for (int i = 0; i < MAX_D; i++){
      // The diamond takes a step
      sprite_step(diamondL[i]);

      // Getting the current location of the diamond.
      int dia_x = round(sprite_x(diamondL[i]));
  		int dia_y = round(sprite_y(diamondL[i]));

      // Get the displacement vector of the wrecker.
  		double dia_dx = sprite_dx(diamondL[i]);
  		double dia_dy = sprite_dy(diamondL[i]);
  		bool dir_changed = false;

  		// Test to see if the diamond hit the left or right border.
  		if (dia_x == 0 || dia_x == screen_width() - DIAMONDL_WIDTH) {
  			dia_dx = -dia_dx;
  			dir_changed = true;
  		}

  		// Test to see if the diamond hit the top or bottom border.
  		if (dia_y == screen_height() * 0.1  || dia_y == screen_height() - DIAMONDL_HEIGHT) {
  			dia_dy = -dia_dy;
  			dir_changed = true;
  		}

  		// Test to see if the diamond needs to step back and change direction.
  		if (dir_changed) {
  			sprite_back(diamondL[i]);
  			sprite_turn_to(diamondL[i], dia_dx, dia_dy);
  		}
    }
  }



  // Testing the diamond [MEDIUM] movement
  if (key == 'x' || key < 0){
    for (int i = 0; i < MAX_Dm; i++){
      if (diamondM[i] -> is_visible){
        // The diamond takes a step
        sprite_step(diamondM[i]);

        // Getting the current location of the diamond.
        int dia_x = round(sprite_x(diamondM[i]));
    		int dia_y = round(sprite_y(diamondM[i]));

        // Get the displacement vector of the wrecker.
    		double dia_dx = sprite_dx(diamondM[i]);
    		double dia_dy = sprite_dy(diamondM[i]);
    		bool dir_changed = false;

    		// Test to see if the diamond hit the left or right border.
    		if (dia_x == 0 || dia_x == screen_width() - DIAMONDM_WIDTH) {
    			dia_dx = -dia_dx;
    			dir_changed = true;
    		}

    		// Test to see if the diamond hit the top or bottom border.
    		if (dia_y == screen_height() * 0.1  || dia_y == screen_height() - DIAMONDM_HEIGHT) {
    			dia_dy = -dia_dy;
    			dir_changed = true;
    		}

    		// Test to see if the diamond needs to step back and change direction.
    		if (dir_changed) {
    			sprite_back(diamondM[i]);
    			sprite_turn_to(diamondM[i], dia_dx, dia_dy);
    		}
      }
    }
  }

  // Testing the diamond [MEDIUM] movement
  if (key == 'c' || key < 0){
    for (int i = 0; i < MAX_Ds; i++){
      if (diamondS[i] -> is_visible){
        // The diamond takes a step
        sprite_step(diamondS[i]);

        // Getting the current location of the diamond.
        int dia_x = round(sprite_x(diamondS[i]));
        int dia_y = round(sprite_y(diamondS[i]));

        // Get the displacement vector of the wrecker.
        double dia_dx = sprite_dx(diamondS[i]);
        double dia_dy = sprite_dy(diamondS[i]);
        bool dir_changed = false;

        // Test to see if the diamond hit the left or right border.
        if (dia_x == 0 || dia_x == screen_width() - DIAMONDS_WIDTH) {
          dia_dx = -dia_dx;
          dir_changed = true;
        }

        // Test to see if the diamond hit the top or bottom border.
        if (dia_y == screen_height() * 0.1  || dia_y == screen_height() - DIAMONDS_HEIGHT) {
          dia_dy = -dia_dy;
          dir_changed = true;
        }

        // Test to see if the diamond needs to step back and change direction.
        if (dir_changed) {
          sprite_back(diamondS[i]);
          sprite_turn_to(diamondS[i], dia_dx, dia_dy);
        }
      }
    }
  }

//MISSILE RELATED CODE
    // Shoot the missile
    for (int i = 0; i < MAX_M; i++) {

      if (key == 'j' || key == 'J'){
        if (!(missile[i] -> is_visible)){
          sprite_show(missile[i]);
          int xcor = sprite_x(ship);
          int ycor = sprite_y(ship) + SHIP_HEIGHT / 2 - 1;
          sprite_move_to(missile[i], xcor, ycor);
          break;
        }
      }

      else if (key == 'k' || key == 'K'){
        if (!(missile[i] -> is_visible)){
          sprite_show(missile[i]);
          int xcor = sprite_x(ship) + SHIP_WIDTH / 2;
          int ycor = sprite_y(ship) - SHIP_HEIGHT / 2 + 1;
          sprite_move_to(missile[i], xcor, ycor);
          break;
        }
      }

      else if (key == 'l' || key == 'L'){
        if (!(missile[i] -> is_visible)){
          sprite_show(missile[i]);
          int xcor = sprite_x(ship) + SHIP_WIDTH;
          int ycor = sprite_y(ship) + SHIP_HEIGHT / 2 - 1;
          sprite_move_to(missile[i], xcor, ycor);
          break;
        }
      }
  }

  for (int i = 0; i < MAX_M; i++){
    sprite_step(missile[i]);
    sprite_turn_to(missile[i], 0, -0.3);
    if (sprite_y(missile[i]) < screen_height() * 0.1) {
      sprite_hide(missile[i]);
    }
  }

  collision_checker();

  clear_screen();

  sprite_draw(ship);

  for (int i = 0; i < MAX_D; i++) {
		sprite_draw(diamondL[i]);
	}

  for (int i = 0; i < MAX_Dm; i++) {
	 	sprite_draw(diamondM[i]);
	}

  for (int i = 0; i < MAX_Ds; i++) {
	 	sprite_draw(diamondS[i]);
	}

  for (int i = 0; i < MAX_M; i++) {
		sprite_draw(missile[i]);
	}

  draw_border();
  draw_separator();
  status_display();
}
