### Optical Team

We first set up the photo-transistor circuit as depicted in the lab handout. We then connected it to a 5V Power Source and connected it to the IR Hat. To increase the IR hat's range of detection, we added a simple inverting op-amp circuit with a gain of about -100 but found that its benefits were minimal for our practical purposes. The circuit is depicted as below:

## Insert Image of Circuit Here

The next thing that we tested was utilizing the FFT library in order to detect a 6080 Hz signal once we received the signal. We changed the sampling rate, according to the calculations done by the Team Alpha report, by setting ADCSRA to 0xe4 from the standard fft_adc_serial script. This makes the ADC sample rate to be about 76,800 Hz. 

Since the fft_adc_serial script utilizes a 256 point fft, we calculated the bucket length to be about 

