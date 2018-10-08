#include <Servo.h>
#include <Arduino.h> // needed for analogRead
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
  return map(normalized_vel, -1, 1, LEFT_VELOCITY_MIN, LEFT_VELOCITY_MAX);
}

/**
 * Convert a normalized velocity to the right servo value.
 * @param normalized_vel The normalized velocity.
 * @return The scaled right servo value.
 */
double get_velocity_right(double normalized_vel) {
  return map(normalized_vel, -1, 1, RIGHT_VELOCITY_MIN, RIGHT_VELOCITY_MAX);
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
  return sensor_on_white(LEFT_COLOR_PIN);  // From util.ino
}

/**
 * Return whether the left pin is on white.
 * @return True if the left pin is on white.
 */
int right_on_white() {
  return sensor_on_white(RIGHT_COLOR_PIN);  // From util.ino
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
 * @param pin_name The pin name of the sensor. Ex: A1.
 * @return True if the sensor is pointed at a wall, false otherwise.
 */
int sensor_at_wall(int pin_name) {
  if (pin_name == frontDistancePin) {
    return analogRead(pin_name) > FRONT_DISTANCE_THRESHOLD;
  } else if (pin_name == RIGHT_DISTANCE_PIN) {
    return analogRead(pin_name) > RIGHT_DISTANCE_THRESHOLD;
  } else {
    return 0;
  }
}

int right_wall() {
  return sensor_at_wall(RIGHT_DISTANCE_PIN);
}

int front_wall() {
  return sensor_at_wall(FRONT_DISTANCE_PIN);
}

int both_wall() {
  return right_wall() && front_wall();
}

int neither_wall() {
  return !right_wall() && !front_wall();
}

void led_on(int pin_name) {
  digitalWrite(pin_name, HIGH);
}

void led_off(int pin_name) {
  digitalWrite(pin_name, LOW);
}

void right_led_on() {
  led_on(RIGHT_LED_PIN);
}

void front_led_on() {
  led_on(FRONT_LED_PIN);
}

void right_led_off() {
  led_off(RIGHT_LED_PIN);
}

void front_led_off() {
  led_off(FRONT_LED_PIN);
}
