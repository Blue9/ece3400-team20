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
correct = (read_register_value(0x12) & 0x80) == 0x80
```
The setup is only deemed correct if all registers have the correct value.

#### Communicating with the FPGA
### Team FPGA
