# Team Arduino

As stated in the lab report, we started by disabling the pull-up registers on the Arduino.
This involved commenting out the following lines in the given twi.c file located at 
C:\ProgramFiles (x86)\Arduino\Hardware\Arduino\avr\libraries\Wire\src\utility


```
  digitalWrite(SDA,1);
  digitalWrite(SCL,1);
```

We then wired the camera to the Arduino as in the diagram. 

<img src=https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/Lab4Diagram.png width=250>

In order to generate the 24 MHz signal on the FPGA, we added the following lines of code to the FPGA template code
after setting up CLK_24 as a 24 MHz signal. 

```
assign GPIO_0_D[0] = CLK_24;
```

We then examined page 18 of the DE0-Nano User Manual located at http://www.ti.com/lit/ug/tidu737/tidu737.pdf
in order to determine where GPIO pin 0 is on the FPGA and connected it to the Arduino as in the diagram.

We then 




