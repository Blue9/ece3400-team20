### Optical Team:

We first set up the photo-transistor circuit as depicted in the lab handout:

![Phototransistor](https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/portfolio/phototransistor.png)

We connected it to a 5V Power Source and then connected the IR Hat to 9 Volts. To increase the IR sensor's range of detection, we added a simple inverting op-amp circuit with a gain of about -100 utilizing resistor values of 100,000 and 1,000 Ohms respectively. See the following article for the circuit design we referenced and built (https://en.wikipedia.org/wiki/Operational_amplifier_applications#Inverting_amplifier). This circuit is shown below but we later found that its benefits were minimal for our practical purposes. And so we decided not to use it in our final design.

<img src=https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/portfolio/OpAmpCircuitOptical.PNG width=250>

The next thing that we tested was utilizing the FFT library in order to detect a 6080 Hz signal once the Arduino received the signal. We opened the example sketch: "fft_adc_serial" from the Open Music Labs Library and determined whether it was detecting frequencies from the IR Hat. What we noticed is that that the Arduino was printing values between 0 to 256 onto the Serial Monitor to denote the value of each of the 256 buckets for the Fourier Transform it was computing. 

Once we made that observation, we changed the sampling rate, according to the calculations done by the Team Alpha report, by setting the ADCSRA register to 0xe4 from the standard fft_adc_serial script. This makes the ADC sample rate to be about 76,800 Hz. This was done since we needed to sample above 36,000 Hz to detect decoys at 18,000 Hz and this was smallest sample rate above that constraint.

We then developed a function denoted as "IsSignalThere" to calculate whether the FFT calculation was detecting the signal at the frequency we wanted. This function took in parameters for 
