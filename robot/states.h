#ifndef states_h
#define states_h

#define WAIT_FOR_TONE 0
#define MOVE_FORWARD 1
#define ADJUST_LEFT 2
#define ADJUST_RIGHT 3
#define FORWARD_UNTIL_PAST_INTERSECTION 4
#define HANDLE_INTERSECTION 5
#define EARLY_TURN 6
#define MID_TURN 7
#define LATE_TURN 8
#define START_180 9
#define MID_180 10
#define LATE_180 11

typedef int (*function_t)(void);

extern function_t states[];

extern int next_state;

extern int turn_direction;
extern function_t on_white[];
extern function_t on_black[];

int handle_next_state();

int wait_for_tone();

int move_forward();

int adjust_left();

int adjust_right();

int forward_until_past_intersection();

int handle_intersection();

#endif
