#include "algorithm.h"
#include "intersect_globals.h"
#include "array_update.h"
#include "constants.h"
#include "states.h"
#include "StackArray.h"


#define array_wv_mask 0x80
#define array_w_mask 0x40
#define array_nv_mask 0x20
#define array_n_mask 0x10
#define array_ev_mask 0x08
#define array_e_mask 0x4
#define array_sv_mask 0x2
#define array_s_mask 0x1
#define array_tcolor_mask 0x100
#define array_tshape_mask 0x600




StackArray <Coord> dfs_next;
StackArray <Coord> dfs_prev_moves;
bool dfs_visited[x_max + 1][y_max + 1];


// Return GLOBAL direction of turn to go back to previous move 
byte get_next_move_direction(int x_curr, int y_curr, int x_next, int y_next) {
	// Serial.println("turning from");
	// Serial.println(x_curr);
	// Serial.println(y_curr);
	// Serial.println(" to ");
	// Serial.println(x_next);
	// Serial.println(y_next);

	byte dir;

	if (x_curr < x_next) dir = 0b01;
	else if (x_curr > x_next) dir = 0b11;
	if (y_curr < y_next) dir = 0b10;  //MIGHT HAVE TO CHANGE -- fixerd . 
	else if (y_curr > y_next) dir = 0b00;
	// Serial.println(dir);
	return dir;
}

// check if you can move from current position to specified position
bool check_if_neighbors(int x_curr, int y_curr, int x_dest, int y_dest){

  // West
	if ((x_curr - x_dest == 1) && ((((array_wv_mask & intersect_array[x_curr][y_curr]) == 0x80) 
		&& ((array_w_mask && intersect_array[x_curr][y_curr]) == 0)) ||( ((array_ev_mask & intersect_array[x_curr - 1][y_curr]) == 0x08)  
   && ((array_e_mask & intersect_array[x_curr - 1][y_curr]) == 0)) ) && (y_curr == y_dest)) {
      // Serial.println("neighbor west");
		return true;
	} 
  // east
  
	if ((x_dest - x_curr == 1) && ((  ((array_ev_mask & intersect_array[x_curr][y_curr]) == 0x08)  
		&& ((array_e_mask & intersect_array[x_curr][y_curr]) == 0) ) || (((array_wv_mask & intersect_array[x_curr + 1][y_curr]) == 0x80) 
    && ((array_w_mask && intersect_array[x_curr + 1][y_curr]) == 0)  ) ) && (y_curr == y_dest)) {
      // Serial.println("neighbor east");
		return true;
	}
  // north
	if ((y_curr - y_dest == 1) && ( ( ((array_nv_mask & intersect_array[x_curr][y_curr]) == 0x20)
		&& ((array_n_mask & intersect_array[x_curr][y_curr]) == 0) ) || ( (  ((array_sv_mask & intersect_array[x_curr][y_curr + 1]) == 0x02) 
    && ((array_s_mask & intersect_array[x_curr][y_curr + 1]) == 0) ) ) )  && (x_curr == x_dest)) {
      // Serial.println("neighbor north");
		return true;
	} 
  // Serial.println( " begin tests ;" );
  // Serial.println(y_dest - y_curr == 1);
  // Serial.println((array_sv_mask & intersect_array[x_curr][y_curr]) == 0x02);
  // Serial.println((array_s_mask & intersect_array[x_curr][y_curr]) == 0);
  // Serial.println(x_curr == x_dest);
  // south
	if ((y_dest - y_curr == 1) && ((  ((array_sv_mask & intersect_array[x_curr][y_curr]) == 0x02) 
		&& ((array_s_mask & intersect_array[x_curr][y_curr]) == 0) ) || ( ( array_nv_mask & intersect_array[x_dest][y_dest] == 0x20) && (array_n_mask & intersect_array[x_dest][y_dest]) == 0)  )   && (x_curr == x_dest)) {
      // Serial.println("neighbor south");
		return true;
	}
  // Serial.println("neighbor false");
	return false;
}
