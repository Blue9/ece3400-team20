# Lab 4
## FPGA & Shape Detection
*Objective:* The goal of this lab was to develop software and hardware to interface with the OV7670 Camera and the DE0-NANO FPGA. To accomplish this the team split up into two groups, one to focus on the communication between the camera and FPGA with the Arduino, and the other to focus on streaming the video between the camera and FPGA.

#### Prelab
Before splitting up into teams, we worked together to finish prelab, finding the key registers on the OV7670 to configure its settings. The following table shows the required registers and their corresponding values to initialize the camera:

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


#### PLL
Before spliting up in

### Team Arduino

### Team FPGA
