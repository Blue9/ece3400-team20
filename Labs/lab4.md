# Lab 4
## FPGA & Shape Detection
*Objective:* The goal of this lab was to develop software and hardware to interface with the OV7670 Camera and the DE0-NANO FPGA. To accomplish this the team split up into two groups, one to focus on the communication between the camera and FPGA with the Arduino, and the other to focus on streaming the video between the camera and FPGA.

#### Prelab
Before splitting up into teams, we worked together to finish prelab, finding the key registers on the OV7670 to configure its settings. To know what values to write, we first looked at the user manual for the [DE0-NANO FPGA](http://www.ti.com/lit/ug/tidu737/tidu737.pdf). The buffer needs to hold an entire frame of 8b pixels so at 594kb there can be a total of ~76,000 total pixels per frame. This led us to picking the format of the camera to be QVGA leading to a resolution of 320x240, equaling ~76,000 pixels per frame.

Once the resolution was known we could continue to find all of registers to initialize the camera. The following table shows the required registers and their corresponding values:

| Description           | Register  | Value |
| -----------           | :------:  | :---: |
| Reset Regs            | 0x12      | 0x80  |
| Enable Scaling        | 0x0C      | 0x04  |
| Use External CLK      | 0x11      | 0x40  |
| Set Resolution        | 0x12      | 0x14  |
| Enable Color Bar Test | 0x12      | 0x02  |
| Vertical Flip         | 0x1E      | 0x10  |
| Mirror Flip           | 0x1E      | 0x20  |

*Note: Some use the same register so the required value cannot simply overwrite the old, but more consideration is required. Implementation is described in the **Team Arduino** section.*

The values for the above table were found in the [OV7670 Datasheet](https://www.voti.nl/docs/OV7670.pdf).

#### PLL
Before spliting up in teams we generate a PLL module to create multiple clocked signals for use both internal and external to the FPGA. The PLL module was generate using Quartus's interal IP generation. We followed the [Lab 4 handout](https://github.com/CEI-lab/ece3400-2018/blob/master/docs/lab4.md#pll) PLL section initially, however, we added a fourth clock signal at 8MHz for use with the serial communication between the FPGA and Arduino. The use of this will be described more in the **Team Arduino** section.

### Team Arduino
The goal of team Adruino was to wire the camera, initialize all of its registers, and interface with the FPGA to read treasure data. To begin we downloaded the [OV7670 setup sketch](https://cei-lab.github.io/ece3400-2018/Lab4_Arduino_Template.zip) as a baseline for implementation.
#### Internal Pull-Up Disable
The first step in initializing the camera is wiring the Arduino to the OV7670, however, the Arduino operates on 5V and the camera operates on 3.3V, so the internal pull-ups to the Arduino's `SDA` and `SCL` pins needed to be disable to avoid burning out the camera.

*Note: This must be done **EVERY** time **BEFORE** code is uploaded.*

To disable the settings on a Windows PC: 

  1. Go to `C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\src\utility`
  2. Right click on *twi.c* and select *Properties>Securities>Edit*
  * Click *Users*
  * Click *Full Control*
  * Click *OK* and *Close*
  3. Open *twi.c* 
  4. Comment out the following lines:
```C
//activate internal pullups for twi
  digitalWrite(SDA,1);
  digitalWrite(SCL,1);
```
5. Save and close


We found that even after changing the file permissions, sometimes users were not able to modify *twi.c*. To work around this we copied the contents of the *twi.c* to a new file in a different directory and named the new file *twi.c*. Then, the new *twi.c* was copied into `C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\src\utility`, replacing the old *twi.c* with the new modified version.

#### Physical Setup
After internal pull-ups were disabled the OV7670 and Arduino were wired based on the labs suggestion:

![OV7670_Wiring](https://cei-lab.github.io/ece3400-2018/images/Lab3CameraWiringDiagram.png)

The wire names in the above picture do *NOT* directly map to the pinout on the OV7670. The conversions are as follows:

| OV7670 Pin Name | Signal Name |
| :-------------: | :---------: |
| SIOC            | SCL         |
| SIOD            | SCA         |
| MCLK            | XCLK        |

To generate the 24MHz signal from the FPGA, the output of the PLL module described above was mapped to a GPIO output pin:
```Verilog
assign GIPO_0_D[0] = CLK_24;
```
#### Communicating with the Camera
To setup an I2C connection the first step is finding the address of the slave, in this case the OV7670. In the datasheet it states for writing the addres is `0x42` and `0x43` for reading, leading to a base address of `0x21` and an appended `0` for write and `1` for read.

To initialize the camera, the results from the prelab were used, writing the required values to the identified registers. To write the data the provided function `OV7670_write_register(reg_address, data)` was used, however, just setting the bits we need would overwrite the values of all other bits in the registers with `0`. Instead, we OR'd the current value of the register written to with the desired value preserving the other bits. For example:
```C
OV7670_write_register(0x01, (read_register_value(0x02) | 0x03));
```
This was particularly relevant for certain registers were we needed to set multiple bits for initialization.

After all registers were written, the bits that were intended to be set were checked to ensure proper setup. This was done by reading the value of a register, ANDing it wih the bit in question, and comparing the value to the expected: 
```C
correct = ((read_register_value(0x12) & 0x80) == 0x80);
```
The setup is only deemed correct if all registers have the correct value.

#### Communicating with the FPGA
To begin a protocol needed to be designed to communicate with between the FPGA and Arduino. Due to the contrainst of digital pins on the Ardunio, we attempted to create a serial communication protocol. 

##### FPGA - Serial
To ensure no bits are ever missed the 8MHz signal generated in the PLL module was output to a digital pin for the clock signal for serial communication:
```Verilog
assign GPIO_0_D[1] = CLK_8;
```
This is half the frequency of the Arduino so there signal will always be detected unless there is outside interference.

The protocol transmits a single byte with bits (2:0) representing the treasures as follows:

| Treasure      | Value |
| :------------ | :---: |
| None          | 0x0   |
| Red Circle    | 0x1   |
| Red Triangle  | 0x2   |
| Red Square    | 0x3   |
| Blue Circle   | 0x4   |
| Blue Triangle | 0x5   |
| Blue Square   | 0x6   |

Only three bits are needed to represent the treasure leaving 5 bits for a preamble for the Arduino to accurately find the value. After much consideration the preamble `0b00111` was chosen as it is always detectable and will never be acciedentally transmitted because of the treasure value.

The preamble is concatenated with the treasure data and transmited one bit at a time over a digital GPIO pin: 
```Verilog
assign GPIO_0_D[3] = cnt[2] ? 
                      (cnt[1] ? 
                        (cnt[0] ? arduino_output[7] : arduino_output[6])  : 
                        (cnt[0] ? arduino_output[5] : arduino_output[4])) : 
                      (cnt[1] ? 
                        (cnt[0] ? arduino_output[3] : arduino_output[2])  : 
                        (cnt[0] ? arduino_output[1] : arduino_output[0]));
```
In the above snippet `cnt` is a counter which counts from `0 -> 7` with wrap around, clocked on the 8MHz signal.

##### Arduino - Serial
On the Arduino we utilized the builtin `shiftIn()` function which is documented [here](https://www.arduino.cc/reference/en/language/functions/advanced-io/shiftin/). In essence, it read a value from a digital pin based on a rising clock edge read from a second digital pin. The data pin for `shiftIn()` was connected to the GPIO output for data on the FPGA, and the clock pin was connected to the GPIO tied to the 8MHz signal.

To avoid using another digital pin on the Arduino we opted to avoid a *start* signal which signifies the start of a byte, and instead opted to detect the preamble and align in software. To do this, two bytes of data are needed from `shiftIn()` and the preamble is found. Once the location of the preamble is detected the treasure is found by the looking at the three bits next lowest, with wrap around.

For exmaple:
**BIT**

|  15  |  14  |  13  |  12  |  11  |  10  |  09  |  08  |  07   |  06   |  05   |  04  |  03  |  02  |  01  |  00  |
| :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :---: | :---: | :---: | :--: | :--: | :--: | :--: | :--: |
|  X   |  X   |  X   |  *0* |  *0* |  *1* |  *1* |  *1* | **0** | **0** | **0** |  X   |  X   |  X   |  X   |  X   |

The Arduino's `shiftIn()` and the FPGA's `cnt` are not alligned, so the first bit read by the Arduino is not the start of a byte, however, when reading the two bytes the preamble is detected in bits [12:8] so the treasure info must be stored in [7:5].


#### Testing - Serial
To ensure the serial communication was working properly we first looked at the output from the FPGA on an oscilliscope. In the followign image, the blue signal is clock and the yellow is the signal.

![FPGA_SERIAL](https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/portfolio/FPGA_SERIAL_OUTPUT.jpg)

The preamble is can be seen and the data is `0b010` so the entire byte is `0b00111010` as seen in the above image.

This showed us the FPGA portion of the system was working properly. However, when we attempted to read in the values from the Arduino with `shiftIn()` allignement and incorrect reads were too significant to overcome. 

After hours of attempting to recitify these error, changing the clock frequency, using different read functions, and using amplifier 3V to 5V circuits, with no success we opted to attempt a parallel solution instead.

#### FPGA - Parallel
The parallel implementation was much simpler than the serial, outputting each of the three treasure bits to a different pin.
```Verilog
assign GPIO_0_D[1] = treasure[0];
assign GPIO_0_D[3] = treasure[1];
assign GPIO_0_D[5] = treasure[2];
```
The representation for treasures was the same as for serial:

| Treasure      | Value |
| :------------ | :---: |
| None          | 0x0   |
| Red Circle    | 0x1   |
| Red Triangle  | 0x2   |
| Red Square    | 0x3   |
| Blue Circle   | 0x4   |
| Blue Triangle | 0x5   |
| Blue Square   | 0x6   |

### Arduino - Parallel
The Arduino parallel implementation again was much easier than its serial counterpart.

The value of the three digital pins the FPGA was connected to were read, representing `treasure[i]`. From there, the three bits read in were converted to decimal:
```C
4*digitalRead(5) + 2*digitalRead(6) + digitalRead(7);
```
Then the table from the **FPGA - Parallel** section was used to translate the decimal to treasures. The final component was requiring the treasure value to stay static to avoid any interference and potential incorrect signals on the line.
```C
if(trs == prev_trs)
  count++;
else
  count = 0;
if(count == trs_threshold)
  /* display/report treasure value */
```

#### Testing - Parallel
The following shows the FPGA communicating with Arduino with arbitrary treasure values:

<YOUTUBE OF COMMS>

### Team FPGA
