# Team FPGA: Downsampling

We started by writing a downsampler based upon the timing diagram in the OV7670 datasheet: pages (7-10) in 

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

# Team FPGA: Color Bar Test

In order to test whether this downsampling was accurate, we needed to enable the Color Bar Test on the Arduino. We added the following registers to do so in the OV7670 sketch:

```
OV7670_write_register(0x12, 0x0E); // enable color bar testes
OV7670_write_register(0x42, 0x08)
```

We obtained the following result:

<img src=https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/portfolio/ColorBar.PNG width=250>

# Team FPGA: Color Detection

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

Since we decided to change to RGB 444, we needed to also change how we would down sample in Reference Point 1 and Reference Point 2. And so we 

 
 
 
   
    

    
