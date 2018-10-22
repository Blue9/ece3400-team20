/*
 * Takes in data on ADC0 (Analog0) and processes them
 * with the fft. the data is sent out over the serial
 * port at 115.2kb.
*/

#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft

#include <FFT.h> // include the library

const int audio_threshold = 10;
short audio_cnt;
uint8_t empty_arr[FFT_N/2];

void setup() {
  Serial.begin(115200); // use the serial port
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe4; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
  audio_cnt = 0;
}

void loop() {
  while(1) { // reduces jitter
    // cli();  // UDRE interrupt slows this way down on arduino1.0
    
    bool audio    = audioFFT();
    bool optical  = opticalFFT();

    Serial.print("Optical: ");
    Serial.print(optical);
    Serial.print("\tAudio: ");
    Serial.println(audio);

  }
}

void resetFFT(){
  for(int i = 0; i < 256; i++){
    fft_log_out[i] = 0;
  }  
}

boolean opticalFFT(){
    TIMSK0 = 0; // turn off timer0 for lower jitter
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
    sei();
    // Serial.println("start");
    /* for (byte i = 0 ; i < FFT_N/2 ; i++) { 
       Serial.println(fft_log_out[i]); // send out the data
      } */
//    resetFFT();
    return isSignalThere(fft_log_out, 6080, 76800);      
}

boolean audioFFT(){
    TIMSK0 = 1; // turn off timer0 for lower jitter
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
    //Serial.println("start");
    /*for (byte i = 0 ; i < FFT_N/2 ; i++) {
        Serial.println(fft_log_out[i]); // send out the data
    } */
    boolean found = isSignalThere(fft_log_out, 660, 8930);
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

boolean isSignalThere(uint8_t fft[], int targetFrequency, long samplingRate) {

  long bucketLength = samplingRate / 256;
  // Serial.print("Bucket Length:\t");
  // Serial.println(bucketLength);
  uint8_t maxValue = 0;
  
  for(byte i = 0; i < FFT_N/2; i++) {
    if(fft[i] > maxValue) {
      maxValue = fft[i];
    }
  }

  uint8_t threshold = 60; //maxValue * (3/4);
  // Serial.print("Threshold:\t");
  // Serial.println(threshold);

  int targetBucket = targetFrequency / bucketLength;
  // Serial.print("Target Bucket:\t");
  // Serial.println(targetBucket);

  byte width = 2;

  for(byte i = 0; i < width; i++) {
  // Serial.print("Bucket Val:\t");
  // Serial.println(fft[targetBucket + i]);

    if(fft[targetBucket + i] >= threshold || fft[targetBucket - i] >= threshold) {
      return true;
    }

  }

  return false;

}
