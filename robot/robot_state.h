#ifndef robot_state_h
#define robot_state_h
#include <Arduino.h>

typedef uint8_t byte;

extern byte x_robot;
extern byte y_robot;
extern bool north, south, east, west;

extern byte current_direction;

void update_robot_position();
int shift_direction(int shift);
void update_direction(int turn_direction);
void reverse_direction();

void update_walls();

#endif
