/**
 * ECE 3400 Team 20. Main maze navigation code.
 * September 8, 2018.
 *
 * Copyright Team 20 2018.
 */
#include <Servo.h>

#define RIGHT_COLOR_PIN A1
#define LEFT_COLOR_PIN A2
#define RIGHT_SERVO_PIN 10
#define LEFT_SERVO_PIN 11
#define SENSOR_THRESHOLD 650  // Determined experimentally.

#define LEFT_VELOCITY_MIN 0
#define LEFT_VELOCITY_MAX 180
#define RIGHT_VELOCITY_MIN 180
#define RIGHT_VELOCITY_MAX 0
#define MAX_SPEED 0.1

Servo right_servo;
Servo left_servo;
int figure_eight_step = 0;

void setup() {
  right_servo.attach(RIGHT_SERVO_PIN);
  left_servo.attach(LEFT_SERVO_PIN);
  pinMode(RIGHT_COLOR_PIN, INPUT);
  pinMode(LEFT_COLOR_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  bool left_on_white = sensor_on_white(LEFT_COLOR_PIN);
  bool right_on_white = sensor_on_white(RIGHT_COLOR_PIN);
  if (right_on_white && left_on_white) {
    // If both the right and left sensors are on white,
    // then we are at an intersection.
    if (figure_eight_step % 8 > 3)
      turn_left();
    else
      turn_right();
    figure_eight_step = (figure_eight_step + 1) % 8;
  } else if (right_on_white && !left_on_white) {
    // If the right line sensor reads white but the left sensor is not on white,
    // then turn right slighly to adjust
    adjust_right(0);
    while (sensor_on_white(RIGHT_COLOR_PIN)) {
      // Wait until neither on white.
    }
    adjust_right(MAX_SPEED);
  } else if (!right_on_white && left_on_white) {
    // If the left line sensor reads white but the right sensor is not on white,
    // then turn left slighly to adjust
    adjust_left(0);
    while (sensor_on_white(LEFT_COLOR_PIN)) {
      // Wait until neither on white.
    }
    adjust_left(MAX_SPEED);
  } else {
    // Move straight.
    adjust_left(MAX_SPEED);
    adjust_right(MAX_SPEED);
  }
  delay(10);
}

/**
 * Write the given speed to the left servo.
 * @param velocity_left The target velocity scaled between -1 and 1.
 */
void adjust_left(double velocity_left) {
  double scaled_velocity = get_velocity_left(velocity_left);
  left_servo.write(scaled_velocity);
}

/**
 * Write the given speed to the right servo.
 * @param velocity_right The target velocity scaled between -1 and 1.
 */
void adjust_right(double velocity_right) {
  double scaled_velocity = get_right_speed(velocity_right);
  right_servo.write(scaled_velocity);
}

/**
 * Turn the robot left 90 degrees by slowing down the left servo.
 */
void turn_left() {
  adjust_left(0.1);
  adjust_right(1);
  delay(1200);
  adjust_left(1);
}

/**
 * Turn the robot right 90 degrees by slowing down the right servo.
 */
void turn_right() {
  adjust_left(1);
  adjust_right(0.1);
  delay(1200);
  adjust_right(1);
}

/**
 * Read the given sensor and return whether it is above the white tape.
 * @param pin_name The pin name of the sensor. Ex: A1.
 * @return True if the sensor is above the white tape, false otherwise.
 */
bool sensor_on_white(int pin_name) {
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
double get_right_speed(double normalized_vel) {
  return map(normalized_vel, -1, 1, RIGHT_VELOCITY_MIN, RIGHT_VELOCITY_MAX);
}

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
