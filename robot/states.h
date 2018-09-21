#ifndef states_h
#define states_h

extern int (*next_state)(void);

int (*get_state())(void);

int (*move_forward())(void);

int (*adjust_left())(void);

int (*adjust_right())(void);

int (*forward_until_past_intersection())(void);

int (*start_turn())(void);

int (*wait_until_turn_end())(void);


#endif
