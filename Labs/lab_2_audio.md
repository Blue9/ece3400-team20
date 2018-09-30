# Lab 2

## Analog

### We started out by designing the amplification and filtering circuit. 

The fourier transform maps signals in the time domain into its constituent frequencies in the frequency domain.  In a signal’s frequency representation, we can analyze the magnitude of various frequencies to see which frequencies are more present than others.  Note that the fourier transform can be applied to both continuous and discrete signals.  A fourier transform applied to a discrete function is called the discrete fourier transform (DFT).  In computer science, one algorithm to calculate the discrete fourier transform in an optimized way is the Fast Fourier Transform (FFT).  The FFT samples a signal at varying time steps and then calculates its frequency components.  In this lab we used the FFT to detect when a 660Hz tone is played.

The following formula can be used to calculate the DFT of a signal:


[logo]: https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/portfolio/DFT.png “DFT” 

where x_k is a N-point sequence of complex points and X_k is the DFT vector.


We first brainstormed various amplifiers that would achieve a high enough gain to amplify the 660Hz signal.  We decided to use an inverting Op Amp cascaded with a low pass filter.  



In the above schematic, the microphone input signal passes through DC signal blocking capacitor C1, which is then fed into the inverting input of the op amp.  The non-inverting input of the op amp is held at a DC bias of Vdd/2 for maximum output voltage swing.  The output signal from the op amp is then passed through a low pass filter with pole at 600kHz.  We did account for the fact that the LPF acts as a load of the Op Amp when we experimented with capacitor and resistor values.  A rough calculation shows that the interstage voltage divider is not too significant.  The final values we used amplified the signal producing a gain of around 100x and a low pass filter with cutoff frequency 4kHz.  The below image shows an oscilloscope reading when a 660Hz tone is played around 10 cm away from the microphone.  

[logo]: https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/portfolio/Optical_Amplifier.jpg  “Osciloscope reading”

The below image shows a frequency sweep of the operational amplifier.  The cutoff frequency is around 4kHz as mentioned above.  Note that this does not affect the gain of the 660Hz signal.


 



On the software side, we initially captured the FFT output from the analog circuit using the example code provided by the FFT library from the Open Music Lab: fft_adc_serial.ino. This script captures analog data on Arduino pin A0 using the embedded ADC at a sampling rate of approximately 38kHz. There are 256 samples (bins) per FFT taken, giving a resolution of 38,000/256 ≈ 150Hz. This indicates that we should see the 660Hz tone in bin ceiling(660/150) = 5, which is what we observed. We wrote the following MATLAB script to capture the data output on to the serial port:

```MATLAB
% Ensure that COM port is open
if ~isempty(instrfind)
 	fclose(instrfind);
  	delete(instrfind);
end

% Clear old data
clear all
close all

% Open serial port on COM3
myserialport = serial('COM3', 'BaudRate', 115200);
fopen(myserialport);

% Get data for approximately 4 FFTs to ensure that we get a complete one
n = 1;
while n <= 512
	% 'start' signal will generate matching failure warning
	% Safe to ignore
	temp = fscanf(myserialport,'%i');
	if (isscalar(temp) && isnumeric(temp))
    		y(n) = temp;
    		n = n+1;
	end
end
```
Using this code, we recorded the following data:


We observed that the 660Hz tone appeared as a strong peak in bin 5 as expected. In addition, tones at 1320Hz and 1960Hz appear in bins 10 and 15 respectively, demonstrating the linearity of the FFT.
Since the peak for 660Hz appears in such a low-numbered bin, we discovered that we could use a significantly slower sampling rate to conserve CPU time. The built-in Arduino function AnalogRead is capable of this. It samples at approximately 8930Hz. This gives us a resolution of 8930/256 ≈ 35Hz. Using this method, we expect to see the 660Hz tone in bin ceiling(660/35) = 19. We modified the code as follows:

```Arduino
#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft

#include <FFT.h> // include the library

void setup() {
  Serial.begin(115200);
}

void loop() {
  while(1) {
	cli();
	for (int i = 0; i < 512 ; i +=2) {
  	fft_input[i] = analogRead(A0);
  	fft_input[i+1] = 0;
	}
	fft_window(); // window the data for better frequency response
	fft_reorder(); // reorder the data before doing the fft
	fft_run(); // process the data in the fft
	fft_mag_log(); // take the output of the fft
	sei();
	for (byte i = 0 ; i < FFT_N/2 ; i++) {
    	Serial.println(fft_log_out[i]); // send out the data
	}
  }
}
```

 Using the same MATLAB script, we captured the following data:



We noticed that the actual bin numbers corresponding to the peaks for the 660Hz, 1320Hz, and 1960Hz test tones were 20, 39, and 57 respectively. Although these bins are not exactly what we expected, they are within a margin of error of a bin, which we found acceptable for an Arduino.

In our next step of testing, we applied the 660 Hz signal with background noise.  To see if we could discern the signal from the noise, we looked at the FFT and checked if the bin with the 660Hz signal is within a certain intensity of the other signal.  Please see the below video for a demonstration:


https://www.youtube.com/watch?v=u_h56zHHiHA 


