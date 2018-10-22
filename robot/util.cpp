#include <Arduino.h>  // needed for analogRead
#include <Servo.h>
#include "constants.h"
#include "servos.h"

/**
 * Scale the given value to a new range.
 * @param value_to_map The value to scale.
 * @param in_min The lower bound of the original range.
 * @param in_max The upper bound of the original range.
 * @param out_min The lower bound of the new range.
 * @param out_max The upper bound of the new range.
 * @return The scaled value.
 */
double map(double value_to_map, double in_min, double in_max, double out_min,
           double out_max) {
  return (value_to_map - in_min) * (out_max - out_min) / (in_max - in_min) +
         out_min;
}

/**
 * Read the given sensor and return whether it is above the white tape.
 * @param pin_name The pin name of the sensor. Ex: A1.
 * @return True if the sensor is above the white tape, false otherwise.
 */
int sensor_on_white(int pin_name) {
  return analogRead(pin_name) < SENSOR_THRESHOLD;
}

/**
 * Convert a normalized velocity to the left servo value.
 * @param normalized_vel The normalized velocity.
 * @return The scaled left servo value.
 */
double get_velocity_left(double normalized_vel) {
  return map(normalized_vel * MAX_SPEED, -1, 1, LEFT_VELOCITY_MIN, LEFT_VELOCITY_MAX);
}

/**
 * Convert a normalized velocity to the right servo value.
 * @param normalized_vel The normalized velocity.
 * @return The scaled right servo value.
 */
double get_velocity_right(double normalized_vel) {
  return map(normalized_vel * MAX_SPEED, -1, 1, RIGHT_VELOCITY_MIN, RIGHT_VELOCITY_MAX);
}

/**
 * Write the given speed to the left servo.
 * @param velocity_left The target velocity scaled between -1 and 1.
 */
void set_left(double velocity_left) {
  double scaled_velocity = get_velocity_left(velocity_left);
  left_servo.write(scaled_velocity);
}

/**
 * Write the given speed to the right servo.
 * @param velocity_right The target velocity scaled between -1 and 1.
 */
void set_right(double velocity_right) {
  double scaled_velocity = get_velocity_right(velocity_right);
  right_servo.write(scaled_velocity);
}

/**
 * Return whether the left pin is on white.
 * @return True if the left pin is on white.
 */
int left_on_white() {
  return sensor_on_white(LEFT_COLOR_PIN);
}

/**
 * Return whether the left pin is on white.
 * @return True if the left pin is on white.
 */
int right_on_white() {
  return sensor_on_white(RIGHT_COLOR_PIN);
}

int only_left_on_white() {
  return left_on_white() && !right_on_white();  // left && !right
}

int only_right_on_white() {
  return right_on_white() && !left_on_white();  // !left && right
}

int both_on_white() {
  return left_on_white() && right_on_white();  // left && right
}

int neither_on_white() {
  return !left_on_white() && !right_on_white();  // !left && !right
}

/**
 * Read the given sensor and return whether it is pointed at a wall.
 * @param sensor_pin_name The pin name of the sensor. Ex: A1.
 * @param threshold The threshold pin value (return true if sensor value >
 * threshold).
 * @return True if the sensor is pointed at a wall, false otherwise.
 */
int sensor_at_wall(int mux_out, int threshold) {
  digitalWrite(WALL_MUX_PIN_OUT1, mux_out & 0b01);
  digitalWrite(WALL_MUX_PIN_OUT2, (mux_out & 0b10) >> 1);
  //delay(10); // Give time for the mux to update
  return analogRead(WALL_MUX_PIN_IN) > threshold;
}

int right_wall() {
  return sensor_at_wall(RIGHT_DISTANCE_MUX, RIGHT_DISTANCE_THRESHOLD);  // see constants.h
}

int front_wall() {
  return sensor_at_wall(FRONT_DISTANCE_MUX, FRONT_DISTANCE_THRESHOLD);  // see constants.h
}

int left_wall() {
  return sensor_at_wall(LEFT_DISTANCE_MUX, LEFT_DISTANCE_THRESHOLD); // see constants.h
}

bool override_pressed() {
  return digitalRead(OVERRIDE_SWITCH_PIN);
}
