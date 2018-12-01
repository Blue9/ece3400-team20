#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include "robot_state.h"

RF24 radio(9,10);

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0x000000003ALL, 0x000000003BLL };

//
// Role management
//
// Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.
//

// The various roles supported by this sketch
typedef enum { role_ping_out = 1, role_pong_back } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch
role_e role = role_ping_out;

enum Treasure {R_C, R_T, R_S, B_C, B_T, B_S, NaN};
Treasure tsure = NaN;

bool iamhere = true;
bool robot = false;


unsigned short get_msg(){
  unsigned short t;
  switch (tsure){
    case R_C : t = 0b110; break;
    case R_T : t = 0b100; break;
    case R_S : t = 0b010; break;
    case B_C : t = 0b111; break;
    case B_T : t = 0b101; break;
    case B_S : t = 0b011; break;
    case NaN : t = 0b000; break; }
    
  unsigned short msg = (x_robot << 13) + (y_robot << 10) +
              (iamhere  << 9) +
              (west     << 8) +
              (north    << 7) +
              (east     << 6) +
              (south    << 5) +
              (robot    << 4) +
              (t        << 1);

//  Serial.println(x_robot << 13);
//  Serial.println(y_robot << 10);
//  Serial.println(iamhere  << 9);
//  Serial.println(west     << 8);
//  Serial.println(north    << 7);
//  Serial.println(east     << 6);
//  Serial.println(south    << 5);
//  Serial.println(robot    << 4);
//  Serial.println(t        << 1);

  return msg;
}


void radio_setup() {
  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  radio.setAutoAck(true);
  // set the channel
  radio.setChannel(0x50);
  // set the power
  // RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBM, and RF24_PA_MAX=0dBm.
  radio.setPALevel(RF24_PA_MIN);
  //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
  radio.setDataRate(RF24_250KBPS);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  //radio.setPayloadSize(8);

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)
  
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  radio.startListening();
  // radio.printDetails();
}

void transmit_msg() {
  // First, stop listening so we can talk.
    radio.stopListening();

    // Take the time, and send it.  This will block until complete
    unsigned long msg = get_msg();
    Serial.print("Now sending ");

    Serial.print(msg);
    Serial.println("...");
    bool ok = radio.write( &msg, sizeof(unsigned short) );

    if (ok)
      Serial.println("ok...");
    else
      Serial.println("failed.\n\r");

    // Now, continue listening
    radio.startListening();

    // Wait here until we get a response, or timeout (250ms)
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while (!radio.available() && !timeout )
      if (millis() - started_waiting_at > 200 )
        timeout = true;

    // Describe the results
    if (timeout)
    {
      Serial.println("Failed, response timed out.\n\r");
    }
    else
    {
      // Grab the response, compare, and send to debugging spew
      unsigned long got_msg;
      radio.read(&got_msg, sizeof(unsigned long));

      // Spew it
      Serial.println("Got response ");
      Serial.println(got_msg);
    }
    radio.stopListening();
}
