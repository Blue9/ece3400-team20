#ifndef constants_h
#define constants_h
#include "pins_arduino.h"

#define RIGHT_COLOR_PIN A2
#define LEFT_COLOR_PIN A3
#define RIGHT_SERVO_PIN 3
#define LEFT_SERVO_PIN 5
#define SENSOR_THRESHOLD 830  // Determined experimentally.
#define SENSOR_THRESHOLD_MARGIN 30

#define LEFT_VELOCITY_MIN 0
#define LEFT_VELOCITY_MAX 180
#define RIGHT_VELOCITY_MIN 180
#define RIGHT_VELOCITY_MAX 0
#define MAX_SPEED 0.08

#define FRONT_DISTANCE_MUX 0b00
#define RIGHT_DISTANCE_MUX 0b11
#define LEFT_DISTANCE_MUX 0b01
#define WALL_MUX_PIN_OUT1 6
#define WALL_MUX_PIN_OUT2 7
#define WALL_MUX_PIN_IN A5

#define FRONT_LED_PIN 0
#define RIGHT_LED_PIN 2
#define LEFT_LED_PIN 8
#define FRONT_DISTANCE_THRESHOLD 170 // Determined experimentally.
#define RIGHT_DISTANCE_THRESHOLD 190 // Determined experimentally.
#define LEFT_DISTANCE_THRESHOLD 130 // Determined experimentally.

#define AUDIO_PIN A1
#define OVERRIDE_SWITCH_PIN 4

#endif
