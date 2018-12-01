#include "util.h"
#include "robot_state.h"

byte x_robot = 0; // int
byte y_robot = -1; // int
byte current_direction = 0b10; // South
// Directions:
// - north: 00
// - west: 11
// - south: 10
// - east: 01

bool north, south, east, west = false;

void update_robot_position() {
  Serial.println("currently at: ");
  Serial.print(x_robot);
  Serial.print(y_robot);
  Serial.println("direction: ");
  Serial.print(current_direction);
  Serial.println(" --");
  
  switch (current_direction) {
    case 0b00: // North
      Serial.println(" north ");
      y_robot--;
      break;
    case 0b10: // South
      Serial.println("south");
      y_robot++;
      break;
    case 0b11: // West
      Serial.println("west");
      x_robot--;
      break;
    case 0b01: // East
      Serial.println("east");
      x_robot++;
      break;
    default:
      break;
  }
 Serial.println(" updated position : ");
 Serial.print(x_robot);
 Serial.print(y_robot);
 Serial.println(" --- x ---- ");
  
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
  Serial.println("printin current direcitonfuckinthisclasss");
  Serial.println(current_direction);
  switch (current_direction) {
    case 0b00: // West ?? | north
      north = front_wall();
      west = left_wall();
      south = false;
      east = right_wall();
      break;
    case 0b10: // East ?? | south
      north = false;
      west = right_wall();
      south = front_wall();
      east = left_wall();
      break;
    case 0b11: // North ??? | west
      north = right_wall();
      west = front_wall();
      south = left_wall();
      east = false;
      break;
    case 0b01: // North ?? | east
      north = left_wall();
      west = false;
      south = right_wall();
      east = front_wall();
      break;
    default:
      break;
  }
}
