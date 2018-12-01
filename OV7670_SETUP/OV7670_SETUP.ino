#include <Wire.h>


#define OV7670_I2C_ADDRESS 0x21 // Right shift in a 0 for write and 1 for read
#define FPGADataPin 6
#define FPGAClockPin 7
const int pattern[] = {1, 1, 1 , 0, 0};


///////// Main Program //////////////
void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Starting");

  // TODO: READ KEY REGISTERS  
  if(!read_key_registers())
    Serial.println("Register Writing Failed!");
  else
  Serial.println("Register Writing Success!");
    
  // TODO: WRITE KEY REGISTERS
  write_key_registers();
  Serial.println("Reg Written");
  
 if(!read_key_registers())
   Serial.println("Register Writing Failed!");
 else
 Serial.println("Register Writing Success!");

 set_color_matrix(); 
 Serial.println("STARTED");
}

void loop(){


byte incomingData = shiftIn(FPGADataPin, FPGAClockPin, LSBFIRST);
byte incomingData2 = shiftIn(FPGADataPin, FPGAClockPin, LSBFIRST);
unsigned int totalData = incomingData*256 + incomingData2;
long output = decodeData(totalData, pattern);

if(output != -1) {
  return output;
}

}


long decodeData(int incomingData, int pattern[]) {


for (int x = 2; x < incomingData - 5; x++) {

  int count = 0;
  for(int y = 0; y < 5; y++) {
    if(bitRead(incomingData, x+y) == pattern[y]) {
      count++;
    }
    else {
      break;
    }
  }

  if(count == 5) {

    return 4*bitRead(incomingData, x) + 2*bitRead(incomingData, x+1) + bitRead(incomingData, x+2);
  }
  
    /*
    if((x+y <= 7) && (bitRead(incomingData2, x+y) == pattern[y])) {
      count++;
    }
    else if((x+y > 7) && (bitRead(incomingData, x+y-7) == pattern[y])) {
      count++;
    }
    else {
      break;
    }
  }
  if(count == 5) {

      if(x >= 10) {


    
  }
  */
}

return -1;
  
}


///////// Function Definition //////////////
bool read_key_registers(){
  bool correct = true;
  Serial.println("Reading Key Registers");
  // correct = correct & ((read_register_value(0x12) & 0x80) == 0x80); // Reset regs
  Serial.println(read_register_value(0x12));
  Serial.println("Past First");
  correct = correct & ((read_register_value(0x0C) & 0x04) == 0x04); // Enable scaling
  Serial.println(read_register_value(0x0C));
  correct = correct & ((read_register_value(0x11) & 0x40) == 0x40); // External CLK
  Serial.println(read_register_value(0x11));
  correct = correct & ((read_register_value(0x12) & 0x14) == 0x14); // Camera res
  Serial.println(read_register_value(0x12));
  correct = correct & ((read_register_value(0x12) & 0x02) == 0x02); // Color test
  Serial.println(correct);
  correct = correct & ((read_register_value(0x1E) & 0x10) == 0x10); // Vertical flip
  Serial.println(correct);
  correct = correct & ((read_register_value(0x1E) & 0x20) == 0x20); // Mirror flip
  Serial.println(correct);

  return correct;
}

void write_key_registers(){
  Serial.println(OV7670_write_register( 0x12, 0x80 )); // Reset regs
  //delay
  delay(100);
  Serial.println(OV7670_write_register( 0x12, 0x0C ));// Camera res
  Serial.println(OV7670_write_register( 0x11, 0xC0 )); // External CLK
  Serial.println(OV7670_write_register( 0x0C, 0x08 )); // Enable scaling
  Serial.println(OV7670_write_register( 0x40, 0xD0 )); // rgb 565
  Serial.println(OV7670_write_register( 0x42, 0x00 ));
  Serial.println(OV7670_write_register( 0x1E, 0x30 )); // mirror/flip
  Serial.println(OV7670_write_register( 0x8C, 0x02 )); // rgb 444
  
  Serial.println(OV7670_write_register( 0x32, 0xC0 ));
}

byte read_register_value(int register_address){
  byte data = 0;
  Wire.beginTransmission(OV7670_I2C_ADDRESS);
  Wire.write(register_address);
  Wire.endTransmission();
  Wire.requestFrom(OV7670_I2C_ADDRESS,1);
  while(Wire.available()<1) {
      // Serial.println(Wire.available());
  }
  data = Wire.read();
  return data;
}

String OV7670_write(int start, const byte *pData, int size){
    int n,error;
    Wire.beginTransmission(OV7670_I2C_ADDRESS);
    n = Wire.write(start);
    if(n != 1){
      return "I2C ERROR WRITING START ADDRESS";   
    }
    n = Wire.write(pData, size);
    if(n != size){
      return "I2C ERROR WRITING DATA";
    }
    error = Wire.endTransmission(true);
    if(error != 0){
      return String(error);
    }
    return "no errors :)";
 }                    

String OV7670_write_register(int reg_address, byte data){
  return OV7670_write(reg_address, &data, 1);
 }

void set_color_matrix(){
    OV7670_write_register(0x4f, 0x80);
    OV7670_write_register(0x50, 0x80);
    OV7670_write_register(0x51, 0x00);
    OV7670_write_register(0x52, 0x22);
    OV7670_write_register(0x53, 0x5e);
    OV7670_write_register(0x54, 0x80);
    OV7670_write_register(0x56, 0x40);
    OV7670_write_register(0x58, 0x9e);
    OV7670_write_register(0x59, 0x88);
    OV7670_write_register(0x5a, 0x88);
    OV7670_write_register(0x5b, 0x44);
    OV7670_write_register(0x5c, 0x67);
    OV7670_write_register(0x5d, 0x49);
    OV7670_write_register(0x5e, 0x0e);
    OV7670_write_register(0x69, 0x00);
    OV7670_write_register(0x6a, 0x40);
    OV7670_write_register(0x6b, 0x0a);
    OV7670_write_register(0x6c, 0x0a);
    OV7670_write_register(0x6d, 0x55);
    OV7670_write_register(0x6e, 0x11);
    OV7670_write_register(0x6f, 0x9f);
    OV7670_write_register(0xb0, 0x84);
}
