#ifndef util_h
#define util_h

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
           double out_max);

/**
 * Read the given sensor and return whether it is above the white tape.
 * @param pin_name The pin name of the sensor. Ex: A1.
 * @return True if the sensor is above the white tape, false otherwise.
 */
int sensor_on_white(int pin_name);

/**
 * Convert a normalized velocity to the left servo value.
 * @param normalized_vel The normalized velocity.
 * @return The scaled left servo value.
 */
double get_velocity_left(double normalized_vel);

/**
 * Convert a normalized velocity to the right servo value.
 * @param normalized_vel The normalized velocity.
 * @return The scaled right servo value.
 */
double get_velocity_right(double normalized_vel);

/**
 * Write the given speed to the left servo.
 * @param velocity_left The target velocity scaled between -1 and 1.
 */
void set_left(double velocity_left);

/**
 * Write the given speed to the right servo.
 * @param velocity_right The target velocity scaled between -1 and 1.
 */
void set_right(double velocity_right);

/**
 * Return whether the left pin is on white.
 * @return True if the left pin is on white.
 */
int left_on_white();

/**
 * Return whether the left pin is on white.
 * @return True if the left pin is on white.
 */
int right_on_white();

/**
 * Return true if the left pin is on white and right pin is not.
 * @return True if only the left pin is on white.
 */
int only_left_on_white();

/**
 * Return true if the right pin is on white and left pin is not.
 * @return True if only the right pin is on white.
 */
int only_right_on_white();

/**
 * Return true if both the left pin and right pin are on white.
 * @return True if both sensors are on white.
 */
int both_on_white();

/**
 * Return true if neither the left pin nor the right pin are on white.
 * @return True if neither sensors are on white.
 */
int neither_on_white();

/**
 * Read the given sensor and return whether it is pointed at a wall.
 * @param sensor_pin_name The pin name of the sensor. Ex: A1.
 * @return True if the sensor is pointed at a wall, false otherwise.
 */
int sensor_at_wall(int sensor_pin_name);

/**
 * Return whether the right wall sensor detects the wall.
 * @return True if the right wall is detected, false otherwise.
 */
int right_wall();

/**
 * Return whether the front wall sensor detects the wall.
 * @return True if the front wall is detected, false otherwise.
 */
int front_wall();

/**
 * Return whether the left wall sensor detects the wall.
 * @return True if the left wall is detected, false otherwise.
 */
int left_wall();

bool override_pressed();

bool opticalFFT();

bool audioFFT();

#endif
