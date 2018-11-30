# Downsampler

We started by writing a downsampler based upon the timing diagram in the OV7670 datasheet: pages (7-10) in 

http://web.mit.edu/6.111/www/f2016/tools/OV7670_2006.pdf .

After reading the timing diagrams we developed our downsampler as follows:

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
   else if (HREF is high and toggle is 0):
        Set pixel data based on first input byte
        Set Write_Enable to 0
   else if (HREF is high and toggle is 1):
        Set pixel data based on second input byte
        Set Write_Enable to 1 # Write to Memory
 ```
 
 To clarify, we introduce the toggle variable in order to 
   
    

    
