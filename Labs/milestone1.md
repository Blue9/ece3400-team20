***Milestone 1:***

***Visual Demonstration:***

<a href="http://www.youtube.com/watch?feature=player_embedded&v=Rsmbcb27Gc8
" target="_blank"><img src="https://www.youtube.com/embed/Rsmbcb27Gc8/0.jpg" 
alt="Robot Figure Eight Pattern" width="240" height="180" border="10" /></a>


***Lab Procedure:***

*Objective:* The objective of this milestone is to be able to add sensors to the robot as well as program the robot such that the robot will be able to line follow and turn along a grid in a figure eight fashion.

The first challenge was being able to determine the placement of the line sensors to optimize line following and turning, knowing whether we are following a line or have reached an intersection. We decided to use two infrared sensors with spacing slightly greater than the tape’s width placed in front of the robot approximately 5 mm above the floor. This placement was chosen so that when the robot detects an intersection both line sensors would read white and while the robot is going straight along a path, both line sensors would read black. If any of the line sensors read white exclusively, i.e. the left sensor reads white and the right sensor reads black or vice-versa, we know that the robot is angled against the line and should adjust accordingly. This adjustment forms the basis for our line-following procedure. We then calibrated the threshold value of each sensor by reading the value on the analog pins along lighter and darker regions of the grid and averaged accordingly. 

***Loop Iteration States:***

To implement line following and turning, on each loop iteration, we checked the value of the line sensors with respect to the threshold and classified/adjusted according to the following four states:
	
**Left on Right off**: If the left sensor reads white and the right sensor reads black, then we know that the robot is angled against the line, i.e. it has turned too far to the right while line following. As a result, we adjust accordingly by stopping the right wheel (turn left) until both line sensors detect black. We then continue both wheels full speed ahead from there. The following code demonstrates what would happen if this conditional is met:


```
adjust_right(0);
while (sensor_on_white(RIGHT_COLOR_PIN) && !sensor_on_white(LEFT_COLOR_PIN)) {
// Wait until neither on white.
}
adjust_right(MAX_SPEED);
```

**Left off right on**: If the right sensor reads white and the right sensor reads black, then we know that the robot is angled against the line in the other direction, i.e. it has turned too far to the left while line following. As a result, we adjust accordingly by stopping the left wheel (turn right) until both line sensors detect black. We then continue both wheels full speed ahead from there. The following code demonstrates what would happen if this conditional is met:

```
adjust_left(0);
while (sensor_on_white(LEFT_COLOR_PIN) && !sensor_on_white(RIGHT_COLOR_PIN)) {
// Wait until neither on white.
}
adjust_left(MAX_SPEED);
```

**Both On**:  Based off of the sensor alignment we chose, as aforementioned, if both sensors are on, we must be at an intersection. In order to complete a figure eight in the configuration we chose, the robot must turn left four times and then turn right four times. We kept a variable that kept track of which turn number we are on and used the mod function to decide whether to turn left or right. For example, if the counter variable records the 9th turn, the robot must be at an equivalent position as for the 1st turn (9 % 8 = 1) and turn left. Once we determined which way to turn, we called our turning functions accordingly. The following code demonstrates what would happen if this conditional is met:

```
// If both the right and left sensors are on white,
// then we are at an intersection.
if (figure_eight_step % 8 > 3) {
Serial.println("Turning left");
turn_left();
} else {
Serial.println("Turning right");
turn_right();
}
figure_eight_step = (figure_eight_step + 1) % 8;

```

**Both off**: If both sensors are off, then the robot is aligned with the tape.  In this case, we continue forward and have both wheels go at their full speeds. The following code demonstrates what would happen if this conditional is met:

```
// Move straight.
adjust_left(MAX_SPEED);
adjust_right(MAX_SPEED);
```


**Turning Procedures**

As aforementioned, since the sensors are slightly farther apart then the line is wide, when both sensors detect a line, we know that an intersection has been found. We developed functions to turn in each direction: turn_left() and turn_right(). The function turn_left() stops the left wheel, sets the right wheel to full speed, delays for 900 msec, and restarts the left wheel. The function turn_right() does the same with the wheels switched. We arrived on the timing of 900 msec through rigorous testing. If the turn is not exact, our line following procedure will ensure the robot returns to being centered on the line for increased robustness. 

```
/**
 * Turn the robot left 90 degrees by slowing down the left servo.
 */
void turn_left() {
  adjust_left(0);
  adjust_right(1);
  delay(900);
  adjust_left(1);
}

/**
 * Turn the robot right 90 degrees by slowing down the right servo.
 */
void turn_right() {
  adjust_left(1);
  adjust_right(0);
  delay(900);
  adjust_right(1);
}
```

**Adjust Functions:**

In order to ease the confusion that results from writing values on the servos from 0 to 180 where 0 refers to full speed ahead on one wheel and 0 refers to full speed backwards on the other, we developed adjust_left and adjust_right functions that take
in values from -1 to 1 that map to the corresponding servos values. For example, adjust_left(0) and adjust_right(0) refer to stopping, adjust_left(1) and adjust_right(1) refer to full speed ahead, and adjust_left(-1) and adjust_right(-1) refer to full speed backwards. These functions rely on a map function we wrote that utilizes the Servos range of 0 to 180 as the out_min and out_max for the left wheel, and 180 to 0 as the out_min and out_max for the right wheel. 

```

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
```
