# Downsampling

We started by writing a downsampler based upon the timing diagram in the OV7670 datasheet: pages (7-10) in 

http://web.mit.edu/6.111/www/f2016/tools/OV7670_2006.pdf .

After reading the timing diagrams we developed the following pseduo-coded downsampler solution as follows:

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

In order to test whether this downsampling was accurate, we needed to enable the Color Bar Test on the Arduino. We added the following registers to do so in the OV7670 sketch:

```
OV7670_write_register(0x12, 0x0E);//set camera pixel format and enable color bar test with 0xE disable with 0xC
OV7670_write_register(0x42, 0x08)
```

We obtained the following result:


 


 
 
 
   
    

    
