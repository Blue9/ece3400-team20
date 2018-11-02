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
Before spliting up in teams we generate a PLL module to create multiple clocked signals for use both internal and external to the FPGA. The PLL module was generate using Quartus's interal IP generation. We followed the [Lab 4 handout](https://cei-lab.github.io/ece3400-2018/lab4.html) PLL section initially, however, we added a fourth clock signal at 8MHz for use with the serial communication between the FPGA and Arduino. The use of this will be described more in the **Team Arduino** section.

### Team Arduino

### Team FPGA
