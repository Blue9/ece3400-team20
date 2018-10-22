# Lab 3 #

*Objective*: The goal of this lab was to integrate all of the compoonents of previous labs and milestones into a single working system, as well as develop new functionality to transmit maze data between the robot and a basestation that will interface with a GUI, displaying the maze.

## Lab Procedure ##

### Setup ###

The team was split up into two groups, one to work on development of the radio communication and corresponding protocols, and the other to integrate the previous components together. 

### Radio ###


### Robot ###

To begin we incorportated the analog audio circuit from Lab 2 onto the primary breadboard, and connecting it to one of the analog input pins. For more information on how the audio circuit is implemented please see ***<Lab 2 Hyperlink>***.

An additional state in our FSM was created as the initial state which waits, continuously checking the `audioFFT()`, also described in Lab 2, looking for the 660Hz start signal. Upon detecting the signal, the FSM transitions to moving forward and begins traversing the maze via the right hand rule.

