# Final Robot Summary

Our final robot was a culmination of all of the previous labs and milestones on this site. We summarize how these previous labs worked into our final robot system as follows:

*Lab 1:* We learned how to work with Arduinos, manipulating the orientation of the Servos, and work with the basic parts of 
building the robot. Our final robot ran on an Arduino Uno and moved using the Parallax Orientation Rotation Servos found
here: http://www.parallax.com/downloads/continuous-rotation-servo-documentation

*Milestone 1:*: Our final robot utilized two line following sensors located at the front of the robot. We developed 
line following algorithms that implied the nature of the orientation of the robot. For example, if the 
left line sensor is indicating that it is on white and the right line sensor is indicating that it is on black, then our robot must be turning too far to the right and must adjust accordingly via the Servos.

*Lab 2:* We learned the basics of FFT analysis and how to determine the frequency of an incoming signal. We developed an audio circuit as follows and had audio FFT analysis within the code. Our circuit is as follows:

<img src=https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/portfolio/MicrophoneCircuit.png width=500>

Similarly we added an IR Hat and IR sensor in order to perform FFT analysis to detect other robots within the maze. 
