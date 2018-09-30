### Optical Team

We first set up the photo-transistor circuit as depicted in the lab handout. We then connected it to a 5V Power Source and connected it to the IR Hat. To increase the IR hat's range of detection, we added a simple inverting op-amp circuit with a gain of about -100 utilizing resistor values of 100,000 and 1,000 Ohms respectively. See the following article for the circuit design we referenced (https://en.wikipedia.org/wiki/Operational_amplifier_applications#Inverting_amplifier). This circuit is shown below but we later found that its benefits were minimal for our practical purposes.

![ArduinoToolsImage](.https://github.com/Blue9/ece3400-team20/blob/gh-pages/img/portfolio/OpAmpCircuitOptical.PNG =20x20)

The next thing that we tested was utilizing the FFT library in order to detect a 6080 Hz signal once the Arduino received the signal. We opened the example sketch: "fft_adc_serial" 

We changed the sampling rate, according to the calculations done by the Team Alpha report, by setting the ADCSRA register to 0xe4 from the standard fft_adc_serial script. This makes the ADC sample rate to be about 76,800 Hz. 

Since the fft_adc_serial script utilizes a 256 point fft, we calculated the bucket length to be about 

