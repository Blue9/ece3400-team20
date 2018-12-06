# Final Robot Design Lab Review:

Our final robot was a culmination of all of the previous labs and milestones on this site. The purpose of building this robot was to compete in a maze mapping competition in which we earn points for following white lines in the maze, detecting and avoiding walls in it, performing image processing in order to identify objects around it (i.e treasures), and then transmitting information about what the maze looks like to a base-station located several feet away. We summarize how our previous work led to our final robot system as follows:

*Lab 1:*
 
We learned how to work with Arduinos, manipulating the orientation of the Servos, and work with the basic parts of building the robot. Our final robot ran on an Arduino Uno and moved using two Parallax Orientation Rotation Servos found
here: http://www.parallax.com/downloads/continuous-rotation-servo-documentation

*Milestone 1:*:

Our final robot utilized two line following sensors located at the front of the robot. Using the readings from these sensors, we developed line following algorithms that implied the nature of the orientation of the robot. For example, if the left line sensor is indicating that it is on white and the right line sensor is indicating that it is on black, then our robot must be turning too far to the right and must adjust accordingly via the servos.

*Lab 2:* We learned the basics of FFT analysis and how to determine the frequency of an incoming signal. We developed an audio circuit as follows and had audio FFT analysis for 660 Hz within the code. Our circuit on the robot is as follows:

<img src=https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/portfolio/MicrophoneCircuit.png width=500>

Similarly we added an IR Hat near the top and an IR sensor on the front in order to perform FFT analysis to detect other robots at 6080 Hz for obstacle avoidance. 

*Lab 3:* Our robot had an nRF24L01 transceiver on it in order to transmit information about its orientation and what is going on around it, i.e. walls, other robots, and if there are treasures located around it. We developed our own data scheme in order to transmit this information once we obtained it and then updated a GUI at a base station that showed what the maze looked like based on the transmission. 

*Milestone 2:* We had three short distance wall sensors in the front, right, and left sides of our robot, creating state transitions in our finite state machine code based on their values. 

*Lab 4 / Milestone 4:* We learned the basics of image processing for shapes and color by working with the OV7670 camera and an FPGA. We decided that the tradeoff between lost points for false detection was too much and decided not to place it on the robot. We made this decision utilizing the frameworks provided by the conflict resolution guest lectures.

*Milestone 3:* Taking into account sensor values for close obstacles, our robot performed a DFS traversal within the maze at the final competition. 

With all the sensors on the robot (wall, line, IR, Audio) and the radios, we found ourselves running out of pins. And so our final robot also contained a multiplexer. In addition, we added a push button in the event that our audio FFT was no longer working that could start the maze traversal when pressed.

We found creating a finite state machine to be immensely useful for both debugging and performance. A diagram is also as follows:

*Robot Budget:*
2 line sensors - $6
3 IR distance sensors - $21
2 parallax servos - $26
1 Arduino Uno - $16
Total: $69

# Challenges Encountered Through The Semester:

Our main challenges revolved around the reliability of the hardware components. At the beginning of this class, we spent several hours perfecting our line following method from Lab 1.  We initially incorporated delays into our turning methods, but then realized that this was not robust enough and what we really needed to have was a finite state machine, so that at every moment the robot would make an appropriate decision. The combination of this and short time delays approximately equal to the propagation delay helped perfect our line following. We were also able to implement and account for situations where the robot moves off track and needs to return back to a white line.

Another main source of errors came from the audio FFT circuit which required close proximity to the sound source.  We fixed this by increasing the gain of the Op Amp and filtering with a cutoff frequency close to the desired frequency.

The short distance sensors posed problems during the DFS maze traversal. With different light settings we had problems calibrating the sensors and finding their threshold. It was critical to get the wall/no-wall threshold close enough to the exact value as our maze traversal algorithm depended on it. We initially thought to self-calibrate before the robot first starts, by examining the surroundings and then mapping its thresholds accordingly, but this proved too difficult to do.  We instead had to hardcode the values at each start. In hindsight, it would have been easiest to just purchase digital short distance sensors.

We had a few problems with transmitting information from sender to receiver.  More specifically, when we first started working on it, there were times where packets were dropped.  To fix this, we thought of implementing a TCP type scheme where we have an acknowledgement check and transmit again if it the check is missing, but this proved to be difficult to implement.  Instead, we found that when we swapped out our radio, communication improved drastically. This fixed our problem.

We had a lot of problems with keeping the dynamic memory under the memory cap.  After integrating DFS with the existing code, we found that we were over dynamic memory usage.  To fix this, we examined the memory usage of each component and saw that the audio FFT part used the most memory. The night before the final competition day, we were unable to get the FFT working, so we made a critical design decision to forego points on the tone starting so that we had a comfortable amount of dynamic memory to work with.  We decided that having enough space to ensure the robustness of the maze traversal algorithm was more important than starting on the tone.

The chassis of the robot was an iteration of a series of chasses we made. In our first chassis, we did not have good parts and the Arduino sat close to the ground. This is good in terms of center of mass and stability, but caused a lot of problems in raw noise when line following.  We redid the chassis to have the Arduino off the ground and mounted line sensors below the Arduino. This fixed our yaw problem but made the robot sometimes flip over due to not enough weight in the front of the robot.  In our final design, we kept this but accounted for this by putting the IR battery in the front. This helped the stability as well as allowed us to save space.  

With regards to the FPGA image processing and camera interaction, we found it incredibly challenging to obtain a stable image on the screen even though our downsampler passed the color bar test. Since we ran out of ribbon wires, we had to crimp our own and consistently found poor image quality. Replacing the wires with ribbon wires we found, the image became much better but still unstable with regards to color and quality as a result of issues with VSYNC transmission. We attempted to make sure that the VSYNC we receive was stable by keeping track of previous VSYNC values but still had issues.

The breadboard we used often caused wires to come loose.  A few days before the final competition, we decided to port the schematic to a solder board and solder wires to ensure that they don’t fall off. This was a huge time investment which unfortunately resulted in an impossible to find short in our circuit.  We spent a while trying to find out where the short was coming from, but eventually found that it was not a short and rather a bigger problem which we did not have the time to solve. The day before the competition we redid the circuitry putting it back onto a breadboard.  If we had more time we would have tried the solder board again or a printed circuit board

# Them vs The Robot They Tell You Not To Worry About
It is simple.  Some robots are designed better than others. We now present several arguments on why our robot is better than the competitors:

Line following:  
Them:  Delays, hardcoded turns, unreliable line following, falling off track
Us:  A robust finite state machine with perfect line following even off track

Chassis:
Them:  Unstable piece of plastic
Us:  Carefully laser cut wood to dampen noise and reduce mechanical perturbations.  	

Audio FFT:
	Them:  Basic Op Amp which amplifies signals rather than filter a desired signal
	Us:  A lowpass filter with cutoff frequency within 10% of desired frequency with boost.

Short Distance Sensors:
	They:  One, unreliable read.
	Us:  High speed redundancy to ensure correctness. 

Algorithm:
	Them:  Textbook DFS
	Us:  DFS with secret optimizations.
