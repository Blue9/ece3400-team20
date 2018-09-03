**Lab 1**

*Objective*: The objective of this lab was to gain familiarity with the Arduino UNO microcontroller and its IDE. 

**Lab Procedure**

*Communicating with the Arduino and the IDE*

We first opened the Arduino IDE and clicked under Files/Examples/01.Basic/Blink. In order to make sure the Arduino UNO was properly connected to the IDE, we clicked under Tools/Board/ and selected Arduino/Genuino UNO. We also verified that the IDE is connected to the right port by looking under Tools/Port. It should be as in the following images:

![ArduinoBlinkSketch](https://github.com/Blue9/ece3400/tree/gh-pages/Labs/Blink.png)

![ArduinoToolsImage](https://github.com/Blue9/ece3400/tree/gh-pages/Labs/Tools.png)

We clicked the check mark to compile the sketch and uploaded it to the Arduino UNO. We obtained the following result:

[//]: # (<iframe width="560" height="315" src="https://www.youtube.com/embed/GjLLtRx1XvA" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>)

<a href="http://www.youtube.com/watch?feature=player_embedded&v=GjLLtRx1XvA
" target="_blank"><img src="https://www.youtube.com/embed/GjLLtRx1XvA/0.jpg" 
alt="Arduino Internal LED Blink" width="240" height="180" border="10" /></a>

*Modifying the Blink Sketch*

We modified the Blink sketch to communicate with an external LED powered by Pin 0 in series with a 300 Ohm resistor on the breadboard. We also made sure to connect the ground of the Arduino the ground of the LED. The wiring and operation is shown in the following video and the modification to the code is as follows:

* Added `#define LED_PIN 0` at the top of the sketch
* Replaced `LED_BUILTIN` with `LED_PIN`

<a href="http://www.youtube.com/watch?feature=player_embedded&v=NNcXywDYe_s
" target="_blank"><img src="https://www.youtube.com/embed/NNcXywDYe_s/0.jpg" 
alt="Arduino External LED Blink" width="240" height="180" border="10" /></a>

[//]: # (<iframe width="560" height="315" src="https://www.youtube.com/embed/NNcXywDYe_s" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>)

*The Serial Monitor and the Analog Pins*

The first goal was to be able to see the output of the potentiometer on the Serial monitor on the Arduino UNO. The potentiometer has three pins and so we connected one end to ground, one end to the 5 Volt output on the Arduino Uno for power, and the middle pin to an Analog Pin on the Arduino for reading. We used A0. 

For the code, we set up the serial monitor and then used AnalogRead in order to read the input. The code is as follows:

```
void setup() {

 Serial.begin(9600); // setting up the Serial monitor

}

void loop() {

  int a = analogRead(A0); // Read the Input
  Serial.println(a); // Print the output to the serial monitor and create a new line

}
```

Our result for this part is as follows:

<a href="http://www.youtube.com/watch?feature=player_embedded&v=0JiTJ-EnS_w
" target="_blank"><img src="https://www.youtube.com/embed/0JiTJ-EnS_w/0.jpg" 
alt="Arduino Analog Input" width="240" height="180" border="10" /></a>


*Analog Output*

The next goal was to be able to take the potentiometer value and map it to a PWM output that goes to the LED. For reference on PWM, we found the following article helpful: https://en.wikipedia.org/wiki/Pulse-width_modulation.

We used the wiring from the previous part for the potentiometer and connected an LED in series with a 300 Ohm resistor to digital pin 5 (PWM pin) all with common grounds. 
Our code for this was as follows:

```
#define POT_PIN A0
#define LED_PIN 5

// the setup function runs once when you press reset or power the board
void setup() {  
  Serial.begin(9600);           // initialize the serial monitor
  pinMode(LED_PIN, OUTPUT);     // setup the LED_PIN as output
}

// the loop function runs over and over again forever
void loop() {
  int a = analogRead(POT_PIN);  // read from the analog pin
  analogWrite(LED_PIN, a/4);    // write to the PWM pin - scaling 1024 -> 256
}
```
*Note:* `analogRead()` produces values from `0` to `1023`, but `analogWrite()` only accepts values from `0` to `255` so the input was scaled down by a factor of four.

What we observed was as follows:


<a href="http://www.youtube.com/watch?feature=player_embedded&v=IoDwbJaUMtU
" target="_blank"><img src="https://www.youtube.com/embed/IoDwbJaUMtU/0.jpg" 
alt="Arduino Analog Output" width="240" height="180" border="10" /></a>

*Parallax Servos*

Instead of the output being written to an LED, we now wrote the output to a Parallax Servos. The involved connecting the red wire to the 5 Volt Arduino output, the black wire to ground, and the white wire to a PWM pin (Pin 5). The code for it is similar; however involved now using the Servos.h library. And so we included this library at the top of the sketch and read its documentation for setting up the Servo from the following link: https://www.arduino.cc/en/Reference/Servo. 

We used the following setup and code:

```
#include <Servo.h>  // include the Servo library

#define POT_PIN A0
Servo myServo;  // create servo object to control a servo

void setup() {
  myServo.attach(9);            // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);           // initialize the serial monitor
}

void loop() {
  
  int a = analogRead(POT_PIN);  // read from the analog pin
  a = map(a, 0, 1023, 0, 180);  // scale 1024 to 180
  myServo.write(a);             // sets the servo speed
}
```

What we observed is as follows:

<a href="http://www.youtube.com/watch?feature=player_embedded&v=YBj1wgy29BY
" target="_blank"><img src="https://www.youtube.com/embed/YBj1wgy29BY/0.jpg" 
alt="Arduino Analog Output" width="240" height="180" border="10" /></a>

*Assembling the Robot*

We assembled the robot and wrote code involving writing to the Servos in order to make an S shape. The following video 
demonstrates our fully autonomous moving robot:

<a href="http://www.youtube.com/watch?feature=player_embedded&v=D5O05hFTHSU
" target="_blank"><img src="https://www.youtube.com/embed/D5O05hFTHSU/0.jpg" 
alt="Robot Moving" width="240" height="180" border="10" /></a>









