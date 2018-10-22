#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define x_mask 0xe000
#define x_shift 13
#define y_mask 0x1c00
#define y_shift 10
#define iamhere_mask 0x0200
#define iamhere_shift 9
#define w_mask 0x0100
#define w_shift 8
#define n_mask 0x0080
#define n_shift 7
#define e_mask 0x0040
#define e_shift 6
#define s_mask 0x0020
#define s_shift 5
#define robot_mask 0x0010
#define robot_shift 4
#define shape_mask 0x000c
#define shape_shift 2
#define color_mask 0x0002
#define color_shift 1

#define get_x(a) (a&x_mask) >> x_shift
#define get_y(a) (a&y_mask) >> y_shift
#define get_iamhere(a) (a&iamhere_mask) >> iamhere_shift
#define get_w(a) (a&w_mask) >> w_shift
#define get_n(a) (a&n_mask) >> n_shift
#define get_e(a) (a&e_mask) >> e_shift
#define get_s(a) (a&s_mask) >> s_shift
#define get_robot(a) (a&robot_mask) >> robot_shift
#define get_shape(a) (a&shape_mask) >> shape_shift
#define get_color(a) (a&color_mask) >> color_shift

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(9,10);

//
// Topology
//

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
role_e role = role_pong_back;

void setup() {

  Serial.begin(9600);
  while (!Serial);
  
  //
  // Setup and configure rf radio
  //
  
  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  radio.setAutoAck(true);
  // set the channel
  radio.setChannel(0x50);
  // set the power
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

  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);

  //
  // Start listening
  //

  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();
}

void loop() {
  // if there is data ready
  if ( radio.available() )
  {
    // Dump the payloads until we've gotten everything
    unsigned long received_val;
    bool done = false;
    while (!done)
    {
      // Fetch the payload, and see if this was the last one.
      done = radio.read( &received_val, sizeof(unsigned long) );

      // Variables from msg
      short x_msg = get_x(received_val);
      short y_msg = get_y(received_val);
      short iamhere_msg = get_iamhere(received_val);
      short w_msg = get_w(received_val);
      short n_msg = get_n(received_val);
      short e_msg = get_e(received_val);
      short s_msg = get_s(received_val);
      short robot_msg = get_robot(received_val);
      short shape_msg = get_shape(received_val);
      short color_msg = get_color(received_val);

      // Spew it
      //Debugging output to serial monitor
//      Serial.print("Got payload ");
//      Serial.println(received_val);
//      Serial.print("x: ");
//      Serial.println(x_msg);
//      Serial.print("y: ");
//      Serial.println(y_msg);
//      Serial.print("iamhere: ");
//      Serial.println(iamhere_msg);
//      Serial.print("W: ");
//      Serial.println(w_msg);
//      Serial.print("N: ");
//      Serial.println(n_msg);
//      Serial.print("E: ");
//      Serial.println(e_msg);
//      Serial.print("S: ");
//      Serial.println(s_msg);
//      Serial.print("Robot: ");
//      Serial.println(robot_msg);
//      Serial.print("Shape: ");
//      Serial.println(shape_msg);
//      Serial.print("Color: ");
//      Serial.println(color_msg);

      //Formatted GUI output
      String gui_out;
      gui_out = String(x_msg) + "," + String(y_msg);
      gui_out = gui_out + ",iamhere=" + (iamhere_msg ? "true" : "false") + ",west=" + (w_msg ? "true" : "false") + ",north=" + (n_msg ? "true" : "false")
        + ",east=" + (e_msg ? "true" : "false") + ",south=" + (s_msg ? "true" : "false") + ",robot=" + (robot_msg ? "true" : "false");
//        + ",tshape=";

//      switch (shape_msg) {
//        case 1:
//          gui_out = gui_out + "square";
//          break;
//        case 2:
//          gui_out = gui_out + "triangle";
//          break;
//        case 3:
//          gui_out = gui_out + "circle";
//          break;
//        default:
//          gui_out = gui_out + "none";
//       }
//       gui_out = gui_out + ",tcolor=";
//       if (shape_msg) {
//         switch (color_msg) {
//            case 0:
//              //red
//              gui_out = gui_out + "red";
//              break;
//            case 1:
//              gui_out = gui_out + "blue";
//              break;
//            default:
//              gui_out = gui_out + "none";
//         }
//       } else {
//          gui_out = gui_out + "none";
//       }
//      Serial.println("reset");
      delay(1000);
//      gui_out = "0,0,iamhere=false,west=false,north=true";
      Serial.println(gui_out);

      // Delay just a little bit to let the other unit
      // make the transition to receiver
      delay(20);

    }

    // First, stop listening so we can talk
    radio.stopListening();

    // Send the final one back.
    radio.write( &received_val, sizeof(unsigned long) );
    //Serial.println("Sent response.\n\r");

    // Now, resume listening so we catch the next packets.
    radio.startListening();
  } 
}
