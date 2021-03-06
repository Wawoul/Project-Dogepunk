/*
Modified by Ewen Paterson
https://anu.biz/
16/10/2018

---------------------------------------------------------------------------
Ardunio will display faces on different switch buttons that are toggled 
Displaying on 32x8 MAX7219 Matrix Module 4-in-1 Display. 

Note: only using 3 buttons for module 1,2,3. 4 is unused
---------------------------------------------------------------------------
*/

#include "LedControl.h"
#include "binary.h"
const int hButtonPin = 7; //First button (Happy)
const int nButtonPin = 6; //Sceond button (Neutral)
const int sButtonPin = 5; //Third button (Sad)

int hButtonState; // variable for reading the pushbutton status
int nButtonState;
int sButtonState;  
int hFlag = 0; //flag when button is toggled
int nFlag = 0;
int sFlag = 0;

byte max_units = 4; 
//4 as we are only using 4 MAX7219
LedControl lc = LedControl(12,11,10,max_units);
//DOUT connects to pin 12
//CLK connects to pin 11
//CS connects to pin 10 
//GND connects to pin GND
//VCC connects to pin 5V Power

// Delay time between faces:
//-------------------------------
//unsigned long delaytime=1000;

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

  // initialize the pushbutton pin as an input:
  pinMode(hButtonPin, INPUT);
  pinMode(nButtonPin, INPUT);
  pinMode(sButtonPin, INPUT);     
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

//void miscface() {
//  lc.setRow(3,0,f3[0]);
//  lc.setRow(3,1,f3[1]);
//  lc.setRow(3,2,f3[2]);
//  lc.setRow(3,3,f3[3]);
//  lc.setRow(3,4,f3[4]);
//  lc.setRow(3,5,f3[5]);
//  lc.setRow(3,6,f3[6]);
//  lc.setRow(3,7,f3[7]);
//}

void loop() {
  // read the state of the pushbutton value:
  hButtonState = digitalRead(hButtonPin);
  nButtonState = digitalRead(nButtonPin);
  sButtonState = digitalRead(sButtonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (hButtonState == HIGH && hFlag == 0) {     
    happyface();
    hFlag = 1;
    hButtonState == LOW;
    delay(500); //small delay to account for button bounce.
  } else if (hButtonState == HIGH && hFlag == 1) {
     lc.clearDisplay(0);
     hFlag = 0;
     delay(500);
  }
  
  if (nButtonState == HIGH && nFlag == 0) {     
    neturalface();
    nFlag = 1;
    nButtonState == LOW;
    delay(500); //small delay to account for button bounce.
  } else if (nButtonState == HIGH && nFlag == 1) {
     lc.clearDisplay(1);
     nFlag = 0;
     delay(500);
  }

   if (sButtonState == HIGH && sFlag == 0) {     
    sadface();
    sFlag = 1;
    sButtonState == LOW;
    delay(500); //small delay to account for button bounce.
  } else if (sButtonState == HIGH && sFlag == 1) {
     lc.clearDisplay(2);
     sFlag = 0;
     delay(500);
  }
}
