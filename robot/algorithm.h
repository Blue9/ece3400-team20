#ifndef algorithm_h
#define algorithm_h

#include "intersect_globals.h"
#include "array_update.h"
#include "constants.h"
#include "states.h"
#include "StackArray.h"

extern byte visited_array[x_max + 1][y_max + 1]; //short
extern bool  dfs_visited[x_max + 1][y_max + 1];

struct Coord{byte x, y;};
//class Coord{
//  public:
//    byte x;
//    byte y;
//    Coord(byte x_coord, byte y_coord){ 
//      x = x_coord; 
//      y = y_coord;
//    } 
//};
extern StackArray <Coord> dfs_next;
extern StackArray <Coord> dfs_prev_moves;

void get_prev_move_direction(int x_curr, int y_curr, int x_prev, int y_prev);
byte get_next_move_direction(int x_curr, int y_curr, int x_next, int y_next);


bool check_if_neighbors(int x_curr, int y_curr, int x_dest, int y_dest); 

#endif
