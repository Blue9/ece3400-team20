# Final Robot Summary

Our final robot was a culmination of all of the previous labs and milestones on this site. The purpose of building this robot was to compete in a maze mapping competition in which we earn points for following white lines in the maze, detecting and avoiding walls in it, performing image processing in order to identify objects around it (i.e treasures), and then transmiting  information about what the maze looks like to a base-station located several feet away. We summarize how our previous work led to our final robot system as follows:

*Lab 1:* We learned how to work with Arduinos, manipulating the orientation of the Servos, and work with the basic parts of 
building the robot. Our final robot ran on an Arduino Uno and moved using the Parallax Orientation Rotation Servos found
here: http://www.parallax.com/downloads/continuous-rotation-servo-documentation

*Milestone 1:*: Our final robot utilized two line following sensors located at the front of the robot. We developed 
line following algorithms that implied the nature of the orientation of the robot. For example, if the 
left line sensor is indicating that it is on white and the right line sensor is indicating that it is on black, then our robot must be turning too far to the right and must adjust accordingly via the servos.

*Lab 2:* We learned the basics of FFT analysis and how to determine the frequency of an incoming signal. We developed an audio circuit as follows and had audio FFT analysis for 660 Hz within the code. Our circuit on the robot is as follows:

<img src=https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/portfolio/MicrophoneCircuit.png width=500>

Similarly we added an IR Hat near the top and an IR sensor on the front in order to perform FFT analysis to detect other robots at 6080 Hz for obstacle avoidance. 

*Lab 3:* Our robot had an nRF24L01 transceiver on it in order to transmit information about its orientation and what is going on around it, i.e. walls, other robots, and if there are treasures located around it. We developed a data scheme in order to transmit this information once we obtained it and then updated a GUI at a base station that showed what the maze looked like based on the  transmission. 

*Milestone 2:* We had three short distance wall sensors in the front, right, and left sides of our robot. 

*Lab 4 / Milestone 4:* We learned the basics of image processing for shapes and color by working with the OV7670 camera and an FPGA. We decided that the tradeoff between lost points for false detection was too much and decided not to place it on the robot. We made this decision utilizing the frameworks provided by the conflict resolution lectures.

*Milestone 3:* Taking into account sensor values for close obstacles, our robot performed DFS traversal within the maze at the final competition. 

With all the sensors on the robot (wall, line, IR, Audio) and the radios, we found ourselves running out of pins. And so our final robot also contained a *mux*. In addition, we added a *push button* in the event that our audio FFT was no longer working that could start the maze traversal when pressed.

We found creating a finite state machine to be immensely useful for both debugging and performance. Please go to https://github.com/Blue9/ece3400-team20/blob/master/robot/states.cpp to get an understanding of all the different states that went into this robot and how they interconnect.

Our final robot is shown below:

*insert picture*






