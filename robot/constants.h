#ifndef constants_h
#define constants_h
#include "pins_arduino.h"

#define RIGHT_COLOR_PIN A2
#define LEFT_COLOR_PIN A3
#define RIGHT_SERVO_PIN 3
#define LEFT_SERVO_PIN 5
#define SENSOR_THRESHOLD 900  // Determined experimentally.

#define LEFT_VELOCITY_MIN 0
#define LEFT_VELOCITY_MAX 180
#define RIGHT_VELOCITY_MIN 180
#define RIGHT_VELOCITY_MAX 0
#define MAX_SPEED 0.08

#define FRONT_DISTANCE_PIN A4
#define RIGHT_DISTANCE_PIN A5
#define FRONT_LED_PIN 8
#define RIGHT_LED_PIN 2
#define FRONT_DISTANCE_THRESHOLD 100 // Determined experimentally.
#define RIGHT_DISTANCE_THRESHOLD 150 // Determined experimentally.

#endif
