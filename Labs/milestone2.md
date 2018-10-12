# Milestone 2

## Maze exploration using Right Hand Rule
 
 To add maze exploration functionality using the right hand rule, we incorporated the following logic into our finite state machine:
```python
  if no_right_wall:
      turn_right()
  elif no_front_wall:
      go_straight()
  else:
      turn_left()
```
We essentially try to turn right at every opportunity possible, and if the right side is blocked, we explore what is forward.
In terms of hardware, we added two short distance sensors for wall detection on the right and front side of the robot.  We measured the threshold levels for wall-detection experimentally.  In the edge case that a robot reaches a dead end, the robot will first turn left (evaluate the "else" statement) and then turn left again, which in aggregate, is the same as turning around.

## Avoiding other Robots
