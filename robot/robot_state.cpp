#include "util.h"
#include "robot_state.h"

int x_robot = 1;
int y_robot = 0;
byte current_direction = 0b11; // North
// Directions:
// - north: 00
// - west: 11
// - south: 10
// - east: 01

bool north, south, east, west = false;

void update_robot_position() {
  switch (current_direction) {
    case 0b00: // North
      y_robot++;
      break;
    case 0b10: // South
      y_robot--;
      break;
    case 0b11: // West
      x_robot--;
      break;
    case 0b01: // East
      x_robot++;
      break;
    default:
      break;
  }
}

int shift_direction(int initial_direction, int shift) {
  return (initial_direction + shift + 4) % 4;
}


void update_direction(int turn_direction) {
  current_direction = shift_direction(current_direction, turn_direction * 2 - 1);
}

void reverse_direction() {
  current_direction = shift_direction(current_direction, 2);
}

void update_walls() {
  switch (current_direction) {
    case 0b11: // West
      north = front_wall();
      west = left_wall();
      south = false;
      east = right_wall();
      break;
    case 0b01: // East
      north = false;
      west = right_wall();
      south = front_wall();
      east = left_wall();
      break;
    case 0b10: // North
      north = right_wall();
      west = front_wall();
      south = left_wall();
      east = false;
      break;
    case 0b00: // North
      north = left_wall();
      west = false;
      south = right_wall();
      east = front_wall();
      break;
    default:
      break;
  }
}
