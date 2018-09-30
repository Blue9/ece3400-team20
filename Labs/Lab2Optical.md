### Optical Team:

We first set up the photo-transistor circuit as depicted in the lab handout:

![Phototransistor](https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/portfolio/phototransistor.png)

We connected it to a 5V Power Source and then connected the IR Hat to 9 Volts. To increase the IR sensor's range of detection, we added a simple inverting op-amp circuit with a gain of about -100 utilizing resistor values of 100,000 and 1,000 Ohms respectively. See the following article for the circuit design we referenced and built (https://en.wikipedia.org/wiki/Operational_amplifier_applications#Inverting_amplifier). This circuit is shown below but we later found that its benefits were minimal for our practical purposes. And so we decided not to use it in our final design. 

<img src=https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/portfolio/OpAmpCircuitOptical.PNG width=250>

We tested our output using the oscilloscope and received the following:

<img src= https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/portfolio/Optical_Amplifier.jpg>

The next thing that we tested was utilizing the FFT library in order to detect a 6080 Hz signal once the Arduino received the signal. We opened the example sketch: "fft_adc_serial" from the Open Music Labs Library and determined whether it was detecting frequencies from the IR Hat. What we noticed is that that the Arduino was printing values between 0 to 256 onto the Serial Monitor to denote the value of each of the 256 buckets for the Fourier Transform it was computing. 

Once we made that observation, we changed the sampling rate, according to the calculations done by the Team Alpha report, by setting the ADCSRA register to 0xe4 from the standard fft_adc_serial script. This makes the ADC sample rate to be about 76,800 Hz. This was done since we needed to sample above 36,000 Hz to detect decoys at 18,000 Hz according to the Nyquist condition and this was smallest sample rate above that constraint. We often printed these Fourier transforms to the Serial monitor to see if we were detecting the correct bin/frequency.

We then developed a function denoted as "IsSignalThere" to calculate whether the FFT contained a peak at the frequency we desired. This function takes in the target frequency, the sampling rate, and the FFT array as parameters. Each bin length is simply the sampling frequency divided by 256 and the function determines which bin the target frequency is and then determines whether this bin or its adjacent bins contain a value above a threshold, which was determined through empirical tests to be 60.  

```
boolean isSignalThere(uint8_t fft[], int targetFrequency, long samplingRate) {

  long bucketLength = samplingRate / 256;
  uint8_t maxValue = 0;
  
  uint8_t threshold = 60; 
  int targetBucket = targetFrequency / bucketLength;
  byte width = 2;
  for(byte i = 0; i < width; i++) {
    if(fft[targetBucket + i] >= threshold || fft[targetBucket - i] >= threshold) {
      return true;
    }
  }
  return false;
}
```

Utilizing this function along with the fft_adc_serial example sketch by calling it with the parameters targetFrequency = 6080 and sampling rate = 76,800, we were able to print to the serial monitor whether we were detecting a signal at 6080 Hz. The following video shows how we were able to detect cycling the IR Hat on and off using the Serial plotter from a foot away:

*Embedded Link for IR Sensor 6 rotated*

We then tested the detection with an IR Decoy that was initially set to 12,000 Hz, later set to 18,000 Hz, and natural/flourescent light to make sure only the 6080 Hz tone was detected.

The following video shows how we did not detect the decoy signal but when the decoy was removed we were able to detect again the 6080 Hz signal:

*Embedded Link for IR Sensor 7 rotated*






