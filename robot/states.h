#ifndef states_h
#define states_h

#define WAIT_FOR_TONE 0
#define MOVE_FORWARD 1
#define ADJUST_LEFT 2
#define ADJUST_RIGHT 3
#define FORWARD_UNTIL_PAST_INTERSECTION 4
#define HANDLE_INTERSECTION 5
#define START_TURN 6
#define WAIT_UNTIL_TURN_END 7
#define UNDO_TURN 8

typedef int (*function_t)(void);

extern function_t states[];

extern int next_state;

int handle_next_state();

int wait_for_tone();

int move_forward();

int adjust_left();

int adjust_right();

int forward_until_past_intersection();

int handle_intersection();

int start_turn();

int wait_until_turn_end();

int undo_turn();

#endif
