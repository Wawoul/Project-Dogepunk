/*
Modified by Ewen Paterson
https://anu.biz/
15/10/2018

---------------------------------------------------------------------------
Ardunio will display faces on each button press 
Displaying on 32x8 MAX7219 Matrix Module 4-in-1 Display. 
---------------------------------------------------------------------------
*/

#include "LedControl.h"
#include "binary.h"
const int buttonPin = 3; 
int counter = 0;

byte max_units = 4; //4 as we are only using 4 MAX7219
LedControl lc = LedControl(12,11,10,max_units);
//DOUT connects to pin 12
//CLK connects to pin 11
//CS connects to pin 10 
//GND connects to pin GND
//VCC connects to pin 5V Power

// Delay time between faces:
//-------------------------------
unsigned long delaytime=1000;

// Happy face:
byte ff[8]= {B00111100,B01000010,B10100101,B10000001,B10100101,B10011001,B01000010,B00111100};

//Neutral face
byte f1[8]= {B00111100,B01000010,B10100101,B10000001,B10111101,B10000001,B01000010,B00111100};

//Sad face
byte f2[8]= {B00111100,B01000010,B10100101,B10000001,B10011001,B10100101,B01000010,B00111100};

//Misc face
byte f3[8]= {B00111100,B01000010,B10100101,B10000001,B10011001,B10100101,B01011010,B00111100};

void setup() {
  lc.shutdown(0,false);
  lc.shutdown(1,false);
  lc.shutdown(2,false);
  lc.shutdown(3,false);
  lc.setIntensity(0,8);
  lc.setIntensity(1,8);
  lc.setIntensity(2,8);
  lc.setIntensity(3,8);
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  lc.clearDisplay(2);
  lc.clearDisplay(3);
} 

void happyface() {
  lc.setRow(0,0,ff[0]);
  lc.setRow(0,1,ff[1]);
  lc.setRow(0,2,ff[2]);
  lc.setRow(0,3,ff[3]);
  lc.setRow(0,4,ff[4]);
  lc.setRow(0,5,ff[5]);
  lc.setRow(0,6,ff[6]);
  lc.setRow(0,7,ff[7]);
}

void neturalface()  {
  lc.setRow(1,0,f1[0]);
  lc.setRow(1,1,f1[1]);
  lc.setRow(1,2,f1[2]);
  lc.setRow(1,3,f1[3]);
  lc.setRow(1,4,f1[4]);
  lc.setRow(1,5,f1[5]);
  lc.setRow(1,6,f1[6]);
  lc.setRow(1,7,f1[7]);
}
 
void sadface()  {
  lc.setRow(2,0,f2[0]);
  lc.setRow(2,1,f2[1]);
  lc.setRow(2,2,f2[2]);
  lc.setRow(2,3,f2[3]);
  lc.setRow(2,4,f2[4]);
  lc.setRow(2,5,f2[5]);
  lc.setRow(2,6,f2[6]);
  lc.setRow(2,7,f2[7]);
}

void miscface() {
  lc.setRow(3,0,f3[0]);
  lc.setRow(3,1,f3[1]);
  lc.setRow(3,2,f3[2]);
  lc.setRow(3,3,f3[3]);
  lc.setRow(3,4,f3[4]);
  lc.setRow(3,5,f3[5]);
  lc.setRow(3,6,f3[6]);
  lc.setRow(3,7,f3[7]);
}

void loop() {
  int buttonState;
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == LOW) {
    counter++;
    delay(150);
  }
  
  if (counter == 1) {
    happyface();
  }
  
  if (counter == 2) {
    neturalface();
  }
  
  if (counter == 3) {
    sadface();
  }
  
  if (counter == 4) {
    miscface();
  }
  if (counter == 5) {
    lc.clearDisplay(0);
    lc.clearDisplay(1);
    lc.clearDisplay(2);
    lc.clearDisplay(3);
    counter = 0;
  }
}
