#include "Muca.h"
#define MICRO_S 100000

long timer = micros();

Muca muca;

//////// LED Matrix
#define ROW_1 2
#define ROW_2 3
#define ROW_3 4
#define ROW_4 5
#define ROW_5 6
#define ROW_6 7
#define ROW_7 8
#define ROW_8 9

#define COL_1 10
#define COL_2 11
#define COL_3 12
#define COL_4 13
#define COL_5 A0
#define COL_6 A1
#define COL_7 A2
#define COL_8 A3

const byte rows[] = {
    ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8
};

// The display buffer
// It shows a walking steps (1 = ON, 0 = OFF)
byte mode_hi[] = {B00000000,B01100110,B01100110,B00000000,B10000001,B01000010,B00111100,B00000000};
byte mode_chat[] = {B00000000,B00000001,B00000010,B00000100,B10001000,B01010000,B00100000,B00000000};
byte mode_heart[] = {B00000000,B01100110,B11111111,B11111111,B01111110,B00111100,B00011000,B00000000};


void setup() {
  Serial.begin(115200);
  //pinMode(LED_BUILTIN,OUTPUT);
  //  muca.skipLine(TX, (const short[]) {
  //    1,2,3,4,5,6,7,8,9,10
  //  }, 10);
  //  muca.skipLine(RX, (const short[]) {
  //    7,8,9,10,11,12
  //  }, 6);
  for (byte i = 2; i <= 13; i++)
      pinMode(i, OUTPUT);
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);

  muca.init();
  muca.useRawData(true); // If you use the raw data, the interrupt is not working
}

void loop() {
  if (micros() - timer > MICRO_S) { //Timer: send sensor data in every 2ms
    timer = micros();
    getDataFromProcessing();
    Serial.flush(); //Flush the serial buffer
    GetRaw();
  }
  
}

void GetRaw() {
  if (muca.updated()) {
    for (int i = 0; i < NUM_ROWS * NUM_COLUMNS; i++) {
      if (muca.grid[i] > 0) Serial.print(muca.grid[i]);
      if (i != NUM_ROWS * NUM_COLUMNS - 1)
        Serial.print(",");
    }
    Serial.println();
  }
}

void  drawScreen(byte buffer2[]){

   // Turn on each row in series
  for (byte i = 0; i < 8; i++) {
    setColumns(buffer2[i]); // Set columns for this specific row
        
    digitalWrite(rows[i], HIGH);
    delay(2); // Set this to 50 or 100 if you want to see the multiplexing effect!
    digitalWrite(rows[i], LOW);
        
  }
}

void setColumns(byte b) {
  digitalWrite(COL_1, (~b >> 0) & 0x01); // Get the 1st bit: 10000000
  digitalWrite(COL_2, (~b >> 1) & 0x01); // Get the 2nd bit: 01000000
  digitalWrite(COL_3, (~b >> 2) & 0x01); // Get the 3rd bit: 00100000
  digitalWrite(COL_4, (~b >> 3) & 0x01); // Get the 4th bit: 00010000
  digitalWrite(COL_5, (~b >> 4) & 0x01); // Get the 5th bit: 00001000
  digitalWrite(COL_6, (~b >> 5) & 0x01); // Get the 6th bit: 00000100
  digitalWrite(COL_7, (~b >> 6) & 0x01); // Get the 7th bit: 00000010
  digitalWrite(COL_8, (~b >> 7) & 0x01); // Get the 8th bit: 00000001
    
    // If the polarity of your matrix is the opposite of mine
    // remove all the '~' above.
}


void getDataFromProcessing() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == 'a') { //when an 'a' charactor is received.
      drawScreen(mode_chat);
      //digitalWrite(LED_BUILTIN, ledOn); //turn on the built in LED on Arduino Uno
    }
    if (inChar == 'b') { //when an 'b' charactor is received.
      drawScreen(mode_hi);
      //digitalWrite(LED_BUILTIN, 0); //turn on the built in LED on Arduino Uno
    }
    if (inChar == 'c') { //when an 'b' charactor is received.
      drawScreen(mode_heart);
      //digitalWrite(LED_BUILTIN, 0); //turn on the built in LED on Arduino Uno
    }
  }
}
