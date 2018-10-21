# Getting Started with the Radios #

We started out by connecting the radios to the special green board and then to the 3.3 Volt Power Supply. 
We then downloaded the RF24 Library off of "https://github.com/maniacbug/RF24". Once we downloaded the ZIP file, we
included the library by navigating to the Arduino application and then to "Sketch" -> "Include Library" -> "Add .Zip File".

We then downloaded the "Getting Started" sketch off of the ECE 3400 Github at 
"https://github.com/CEI-lab/ece3400-2018/blob/master/docs/Solutions/lab4/GettingStarted/GettingStarted.ino". What we immediately 
noticed is that it would not compile as a result of the "printf" statements. Thus, we replaced each of the "printf" statements
with "Serial.println" and we were able to get the code to compile and upload.

At first, we had difficulties with transmitting and receiving utilizing the "Getting Started" sketch. Utilizing a Voltmeter,
we recognized that there was a short within the radio that was causing issues and so we had to borrow another groups' radio in order
to continue. 

We then set our pipe values by utilizing the formula: 2(3D + N) + X where D = 3 and N = 20 represented our lab date and group number.
This yielded 58 and 59 which in hexadecimal are 3A And 3B. And so we changed the pipe values to 0x000000003ALL 
and 0x000000003BLL respectively.

This allowed us to get the "Getting Started" sketch to work with accuracy of approximately 8 feet from each other. 

# Transmitting Robot Information #

At this point, we needed to develop a method of transmitting data about our surroundings and a way of encoding our surroundings.
We decided on the following 2 byte system:

Bits: 

15-13: X Coordinate <br/>
12-10: Y Coordinate <br/>
9: iamhere (robot is here) <br/>
8: west (whether there is a west wall) <br/>
7: north (whether there is a north wall) <br/>
6: east (whether there is a east wall) <br/>
5: south (whether there is a south wall) <br/>
4: robot (whether there is a robot) <br/>
3-1: treasure (which treasure we detected) <br/>
0: Unused

Thus, we created the global variables x_cord, y_cord, iamhere, west, north, east, south, and robot and depending on 
whether these values were true or not, we encoded an appropriate 2 byte value (short) to send. We created the following 
method get_msg to convert global variables to the encoded message. We also created an enum for treasures that contained 
"R_C, R_T, R_S, B_C, B_T, B_S, and NaN" that tsure would take on the values of. The method works as follows:

```
unsigned short get_msg(){
  unsigned short t;
  switch (tsure){
    case R_C : t = 0b110; break;
    case R_T : t = 0b101; break;
    case R_S : t = 0b100; break;
    case B_C : t = 0b011; break;
    case B_T : t = 0b010; break;
    case B_S : t = 0b001; break;
    case NaN : t = 0b000; break; }
    
  unsigned short msg = (x_coord << 13) + (y_coord << 10) +
              (iamhere  << 9) +
              (west     << 8) +
              (north    << 7) +
              (east     << 6) +
              (south    << 5) +
              (robot    << 4) +
              (t        << 1);

  return t;
}

```
This allowed us to encode messages to write to our radios that would be able to transmit to each other radio.

# Receiving Information: #

Once a radio received a packet in the data scheme above, we decoded the information utilizing bit shifting. 
For example, in order to obtain the x and y coordinates, we used the following masks and bit-shifting.

```
#define x_mask 0xe000
#define x_shift 13
#define y_mask 0x1c00
#define y_shift 10
#define get_x(a) (a&x_mask) >> x_shift
#define get_y(a) (a&y_mask) >> y_shift
```

This allowed us to communicate information from one radio to another and decode it appropriately. The following 
images demonstrates the output when we sent encoded information from one radio to another:

<img src=https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/IMG_5380.jpg width=500 height = 250>
<img src=https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/IMG_5381.jpg width=500 height = 250>

# Updating GUI from the base station #

We were able to update the GUI from the base station using hardcoded values.  See the below video:

// TODO


# Updating GUI from virtual robot wirelessly
The following video shows us updating the GUI from a virtual robot on a separate Arduino which is separately connected to the base station.  There is slight lagging mostly due to the browser's refreshing rate.

https://youtu.be/IlqivRq3zKw


# Robot starting on a 660Hz tone

See the below video:

https://www.youtube.com/watch?v=J3oYayUAUiY

We also implemented a manual override switch:

https://www.youtube.com/watch?v=3r8BT0jyPSM

# Robot starting on a 660Hz tone and exploring maze


See the below video:
https://youtu.be/U-msFs_uLQE

# 






