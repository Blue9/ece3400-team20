# Team FPGA Setup:

After opening up the Verilog project code from the ECE 3400 website and setting up the clocks as instructed, we declared red, green, and blue as localparameters as 8'b11100000, 8'b00011100, and 8'b00000111 respectively. We also attached the VGA adapter to GPIO_0_5 -> GPIO_0_23 using page 18 of the DEO_NANO user manual. We display the diagram for future reference:

<img src=https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/portfolio/FPGADiagram.png width=700>

We set the pixel that we are writing values to by modifing X_ADDR and Y_ADDR. In order to write a test image to memory, on every clock cycle, we increment either X_ADDR or Y_ADDR and write a corresponding color based on the value of X_ADDR and Y_ADDR. We attempted a different color bar to write to memory as our test image:

<img src=https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/portfolio/Screen%20Shot%202018-11-30%20at%202.52.07%20PM.png width=700>


# Team FPGA Downsampling:

We wrote our downsampler based upon the timing diagram in the OV7670 datasheet: pages (7-10) in 

http://web.mit.edu/6.111/www/f2016/tools/OV7670_2006.pdf .

After reading the timing diagrams we developed the following pseduo-coded downsampler solution:

```
When PCL goes high: // trigger when PCL goes high

    if(VSYNC is high):
        // new frame comes in
        Reset X_Address and Y_Address
        Reset pixel_data
        Set Write_Enable to 0
        Set Toggle to 0
    else if (HREF is on falling edge):
        // new row data comes in
        Reset X_Address
        Increment Y_Address
        Reset pixel_data
        Set Write_Enable to 0
        Set Toggle to 0
   else if (HREF is high and toggle is 0): # Reference Point 1
        Set pixel data based on first_input_byte
        Set Write_Enable to 0
   else if (HREF is high and toggle is 1): # Reference Point 2
        Set pixel data based on second_input_byte
        Set Write_Enable to 1 # Write to Memory
 ```
 
To clarify the pseudo-code, we introduce the toggle variable because color transmission occurs by sending two bytes. Depending on the color scheme utilized, we may obtain the green color data from one byte and the red/blue color data from another byte. We first utilized an RGB565 scheme and so obtained the R/G data upon the first byte and the G/B data upon the second byte. And so we set pixel_data[7], pixel_data[6], pixel_data[5], pixel_data[4], pixel_data[3], pixel_data[2] to first_input_byte[7], first_input_byte[6], first_input_byte[5], first_input_byte[2], first_input_byte[1], first_input_byte[0] respectively in the Reference Point 1 section. And we set pixel_data[1], pixel_data[0] to second_input_byte[4], second_input_byte[3] respectively in the Reference Point 2 section. 

The reason for this ordering can be found on page 9 in the OV7670 manual. We take the three most significant bits from the red transmission, the three most significant bits from the green transmission, and the two most significant bits from the blue transmission. 

# Team FPGA Color Bar Test:

In order to test whether this downsampling was accurate, we needed to enable the Color Bar Test on the Arduino. We added the following registers to do so in the OV7670 sketch from Team Arduino:

```
OV7670_write_register(0x12, 0x0E); // enable color bar testes
OV7670_write_register(0x42, 0x08)
```

We obtained the following result:

<img src=https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/portfolio/ColorBar.PNG width=700>

# Team FPGA Color Detection Settings:

After obtaining a color bar test, we attempted to work on obtaining image quality such that we would be able to detect shape colors. Initial image quality of the camera was very poor and so we replaced all of our wires. After that did not work well, we attempted to play around with the register settings and utilized RGB 444 instead of RGB 565 for our tranmission setting. We found the following register settings to work well:

```

OV7670_write_register(0x12, (read_register_value(0x12) | 0x80)); // Reset All Registers
OV7670_write_register(0x0C, (read_register_value(0x0C) | 0x08)); // Enable Scaling
OV7670_write_register(0x11, (read_register_value(0x11) | 0x40)); // Enable External Clock
OV7670_write_register(0x12, (read_register_value(0x12) | 0x0C)); // Camera Resolution
OV7670_write_register(0x8C,  (read_register_value(0x8C) | 0x2); // Enable RGB444
OV7670_write_register(0x40, (read_register_value(0x40) | 0xD0); // Set the bit for RGB and Enable a Bit for 444
OV7670_write_register(0x1E, (read_register_value(0x1E) | 0x30));  // Mirror Flip
OV7670_write_register(0x14, (read_register_value(0x14) | 0x01)); // Gain setting
```

Since we decided to change to RGB 444, we also needed to change how we would down-sample in Reference Point 1 and Reference Point 2. And so we changed the ordering of pixel_data to {input_data_1[3], input_data_1[2], input_data_1[1], input_data_2[7], input_data_2[6], input_data_2[5], input_data_2[3], input_data_2[2]}. 

With this setting, we obtained better image quality but still had issues with differentiating blue and red colors. And so we decided to fully saturate red, remove green, and threshold blue by utilizing the following scheme on the most significant bits: pixel_data = {input_data_1[3], input_data_1[3], input_data_1[3], 0, 0, 0, input_data_2[3] | input_data_2[2], input_data_2[3] | input_data_2[2]}. And so if the most significant bit of the red data was high, then we fully saturate red. And if either of the two most significant bits of blue were high, then we fully saturate blue. We found this scheme to work well for color detection.

# Team FPGA Color Detection, Image Processor:

In order to differentiate between red and blue treasures, we kept counts on the number of blue and red pixels and thresholded whether the frame was blue or red. The code looked as the following pseudo-code:

```
blueCount = 0
redCount = 0 
On every rising clock edge:
    if HREF is high:
        if incoming pixel has blue values all high (00000011): 
            increment blue count
        if incoming pixel has red values all high (11100000)
            increment red count
    if VSYNC is on rising edge:
        compare blueCount and redCount against threshold
        set the LEDs according whichever is higher than threshold
    if VSYNC is on falling edge:
        reset blueCount
        reset redCount
```
 
Note that in order to determine rising and falling edges, we kept track of the previous values and compared against the current value. We empirically tested for the threshold and found 70 to be a good threshold. 

# Color Detection, Demonstration:

Utilizing the register settings and the image processor in the previous two sections, we were able to obtain color detection as demonstrated in the following videos:

https://www.youtube.com/watch?v=X9Ipn2ejz8U





 
   
    

    
