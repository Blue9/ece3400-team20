/*
 * Takes in data on ADC0 (Analog0) and processes them
 * with the fft. the data is sent out over the serial
 * port at 115.2kb.
*/

#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft

#include <avr/interrupt.h>
#include <pins_arduino.h>
#include <Arduino.h> // needed for analogRead
#include <FFT.h> // include the library

typedef uint8_t byte;

const int audio_threshold = 10;
short audio_cnt;
uint8_t empty_arr[FFT_N/2];

void resetFFT(){
  for(int i = 0; i < 256; i++){
    fft_log_out[i] = 0;
  }  
}

bool isSignalThere(uint8_t fft[], int targetFrequency, long samplingRate) {
  int bucketLength = samplingRate / 256;
  // Serial.print("Bucket Length:\t");
  // Serial.println(bucketLength);
  byte maxValue = 0;
  
  for (byte i = 0; i < FFT_N / 2; i++) {
    if (fft[i] > maxValue) {
      maxValue = fft[i];
    }
  }

  byte threshold = 60; //maxValue * (3/4);
  // Serial.print("Threshold:\t");
  // Serial.println(threshold);

  byte targetBucket = targetFrequency / bucketLength;
  // Serial.print("Target Bucket:\t");
  // Serial.println(targetBucket);

  byte width = 2;
  for (int i = 0; i < width; i++) {
  // Serial.print("Bucket Val:\t");
  // Serial.println(fft[targetBucket + i]);
    if (fft[targetBucket + i] >= threshold || fft[targetBucket - i] >= threshold) {
      return true;
    }
  }
  return false;
}

bool opticalFFT(){
    TIMSK0 = 0; // turn off timer0 for lower jitter
    byte init_adcsra = ADCSRA;
    ADCSRA = 0xe4; // set the adc to free running mode
    ADMUX = 0x40; // use adc0
    DIDR0 = 0x01; // turn off the digital input for adc0
    cli();
    for (int i = 0 ; i < 512 ; i += 2) { // save 256 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf4; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fft_input[i] = k; // put real data into even bins
     // Serial.println(m);
      fft_input[i+1] = 0; // set odd bins to 0
    }
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
    ADCSRA = init_adcsra; // This is needed so the function actually returns
    sei();
    //for (byte i = 0 ; i < FFT_N/2 ; i++) { 
    //  Serial.println(fft_log_out[i]); // send out the data
    //}
    return isSignalThere(fft_log_out, 6080, 76800);
}

bool audioFFT(){
    TIMSK0 = 1; // turn off timer0 for lower jitter
    byte init_adcsra = ADCSRA;
    ADCSRA = 0x87; // set the adc to free running mode
    ADMUX = 0x50; // use adc0
    DIDR0 = 0x00; // turn off the digital input for adc0
    cli();
    for (int i = 0; i < 512 ; i +=2) {
      fft_input[i] = analogRead(A5);
      fft_input[i+1] = 0;
    }
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
    sei();
    ADCSRA = init_adcsra;
    //Serial.println("start");
    /*for (byte i = 0 ; i < FFT_N/2 ; i++) {
        Serial.println(fft_log_out[i]); // send out the data
    } */
    bool found = isSignalThere(fft_log_out, 660, 8930);
    if(found){
      audio_cnt++;
    }
    else {
      audio_cnt = 0;
    }
    /*
    Serial.print(found);
    Serial.print("\t");
    Serial.println(audio_cnt);    */
//    resetFFT();
    if(audio_cnt >= audio_threshold){
      // Serial.println("660Hz Found"); 
      return true;
    }
    return false; 
}
