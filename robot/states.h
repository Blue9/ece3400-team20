#ifndef states_h
#define states_h

#define MOVE_FORWARD 0
#define ADJUST_LEFT 1
#define ADJUST_RIGHT 2
#define FORWARD_UNTIL_PAST_INTERSECTION 3
#define START_TURN 4
#define WAIT_UNTIL_TURN_END 5
#define HANDLE_WALL 6

typedef int (*function_t)(void);

extern function_t states[];

extern int next_state;

int handle_next_state();

int move_forward();

int adjust_left();

int adjust_right();

int forward_until_past_intersection();

int start_turn();

int wait_until_turn_end();


#endif
