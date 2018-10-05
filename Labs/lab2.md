# Lab 2 #

*Objective*: The goal of this lab was to design and implement analog and digital circuitry to detect both optical and audio signals.

## Lab Procedure ##

### Setup ###

The team was split into two groups, one to detect a 660 Hz audio tone that will signify the start of the competition, and one to detect a 6080 Hz infrared signal. Both groups utilized Open Music Labs Arduino FFT library to perform the required signal analysis.  More information on Open Music Labs can be found on their website [here](http://wiki.openmusiclabs.com/wiki/ArduinoFFT).

### Audio ###

### Optical ###
We first set up the photo-transistor circuit as depicted in the lab handout:

<img src=https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/portfolio/phototransistor.png width=250>

We connected it to a 5V Power Source and then connected the IR Hat to 9 Volts. To increase the IR sensor's range of detection, we added a simple inverting op-amp circuit with a gain of about -100 utilizing resistor values of 100,000 and 1,000 Ohms respectively. See the following article for the circuit design we referenced and built: [Inverting Amplifier](https://en.wikipedia.org/wiki/Operational_amplifier_applications#Inverting_amplifier). This circuit is shown below but we later found that its benefits were minimal for our practical purposes. And so we decided not to use it in our final design. 

<img src=https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/portfolio/OpAmpCircuitOptical.PNG width=250>

The next thing that we tested was utilizing the FFT library in order to detect a 6080 Hz signal once the Arduino received the signal. We opened the example sketch: "fft_adc_serial" from the Open Music Labs Library and determined whether it was detecting frequencies from the IR Hat. What we noticed is that that the Arduino was printing values between 0 to 256 onto the Serial Monitor to denote the value of each of the 256 buckets for the Fourier Transform it was computing. 

Once we made that observation, we changed the sampling rate, according to the calculations done by the Team Alpha report, by setting the ADCSRA register to 0xe4 from the standard fft_adc_serial script. This makes the ADC sample rate to be about 76,800 Hz. This was done since we needed to sample above 36,000 Hz to detect decoys at 18,000 Hz according to the Nyquist condition and this was smallest sample rate above that constraint. We often printed these Fourier transforms to the Serial monitor to see if we were detecting the correct bin/frequency.

We then developed a function denoted as `isSignalThere` to calculate whether the FFT contained a peak at the frequency we desired. This function takes in the target frequency, the sampling rate, and the FFT array as parameters. The width of each bin is the sampling frequency divided by 256, and the function calculates which bin the target frequency is and then checks whether this bin or its adjacent bins contain a value above a threshold, which was determined through empirical tests to be 60.  

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

Utilizing this function along with the `fft_adc_serial` example sketch by calling it with the parameters targetFrequency = 6080 and sampling rate = 76,800, we were able to print to the serial monitor whether we were detecting a signal at 6080 Hz. The following video shows how we were able to detect cycling the IR Hat on and off using the Serial plotter from a foot away:

https://www.youtube.com/watch?v=_tRcdDt8q-A

We then tested the detection with an IR Decoy that was initially set to 12,000 Hz, later set to 18,000 Hz, and natural/flourescent light to make sure only the 6080 Hz tone was detected.

The following video shows how we were able to detect the 6080 Hz signal, but not the decoy when it was placed in the way:

https://www.youtube.com/watch?time_continue=17&v=_UgpGABSOR8


Finally, the IR Hat and decoy were displaced slightly so the IR sensor sensed both signals, but only detected the 6080Hz tone. The FFT of the two signals is below:

<img src= https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/portfolio/fftpicopticalteam%20(1).jpg>

### Combined System ###

Once each system had been implemented and tested they were combined into a single Arduino sketch, with the optical signal reading from A0, and the acoustic signal being read from A5. The loop for individual system was put into a function, and were then called from a top level loop, as seen below:

```
void loop() {
  while(1) { // reduces jitter
    bool audio    = audioFFT();
    bool optical  = opticalFFT();
    
    Serial.print("Optical: ");
    Serial.print(optical);
    Serial.print("\tAudio: ");
    Serial.println(audio);

  }
}

```

Simply pasting the two sets of code into functions do not work properly, as they use different sample rates and ADC settings. To combat this issue initialization of the ADC was moved form the `setup()` function to each of the individual FFT functions. After this change was made the entire system worked properly as shown in the video below:

*https://www.youtube.com/watch?v=Ro4x0O4s6_g*
