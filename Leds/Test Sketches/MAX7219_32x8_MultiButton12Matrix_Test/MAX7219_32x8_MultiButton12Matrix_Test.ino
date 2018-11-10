/*
Modified by Ewen Paterson
https://anu.biz/
10/11/2018

---------------------------------------------------------------------------
Ardunio will display faces on different switch buttons that are toggled 

The purpose of this test is to check 12 MAX7219 can be daisy chained together.
Result: They can not! Only 8 matrixes can be in one instance of daisy LedControl
We will need to create two instances of LedControl;
lc1 with 8 MAX7219 (Displays happyface & neutralface)
lc2 with 4 MAX7219 (Displays sadface)
This works but required more pins

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

//byte max_units = 8; //8 as we are only using 8 MAX7219
LedControl lc1 = LedControl(12,11,10,8);
LedControl lc2 = LedControl(8,9,3,4);
//DOUT connects to pin 12, 8
//CLK connects to pin 11, 9
//CS connects to pin 10, 3 
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
  int devices=lc1.getDeviceCount();
  for(int address=0;address<devices;address++) {
    lc1.shutdown(address,false);
    lc1.setIntensity(address,8);
    lc1.clearDisplay(address);
  }
  int devices2=lc2.getDeviceCount();
  for(int address=0; address<devices2 ;address++) {
    lc2.shutdown(address,false);
    lc2.setIntensity(address,8);
    lc2.clearDisplay(address);
  }

  // initialize the pushbutton pin as an input:
  pinMode(hButtonPin, INPUT);
  pinMode(nButtonPin, INPUT);
  pinMode(sButtonPin, INPUT);     
} 

void happyface() {
  lc1.setRow(0,0,ff[0]);
  lc1.setRow(0,1,ff[1]);
  lc1.setRow(0,2,ff[2]);
  lc1.setRow(0,3,ff[3]);
  lc1.setRow(0,4,ff[4]);
  lc1.setRow(0,5,ff[5]);
  lc1.setRow(0,6,ff[6]);
  lc1.setRow(0,7,ff[7]);
}

void neutralface()  {
  lc1.setRow(5,0,f1[0]);
  lc1.setRow(5,1,f1[1]);
  lc1.setRow(5,2,f1[2]);
  lc1.setRow(5,3,f1[3]);
  lc1.setRow(5,4,f1[4]);
  lc1.setRow(5,5,f1[5]);
  lc1.setRow(5,6,f1[6]);
  lc1.setRow(5,7,f1[7]);
}
 
void sadface()  {
  lc2.setRow(2,0,f2[0]);
  lc2.setRow(2,1,f2[1]);
  lc2.setRow(2,2,f2[2]);
  lc2.setRow(2,3,f2[3]);
  lc2.setRow(2,4,f2[4]);
  lc2.setRow(2,5,f2[5]);
  lc2.setRow(2,6,f2[6]);
  lc2.setRow(2,7,f2[7]);
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
     lc1.clearDisplay(0);
     hFlag = 0;
     delay(500);
  }
  
  if (nButtonState == HIGH && nFlag == 0) {     
    neutralface();
    nFlag = 1;
    nButtonState == LOW;
    delay(500); //small delay to account for button bounce.
  } else if (nButtonState == HIGH && nFlag == 1) {
     lc1.clearDisplay(5);
     nFlag = 0;
     delay(500);
  }

   if (sButtonState == HIGH && sFlag == 0) {     
    sadface();
    sFlag = 1;
    sButtonState == LOW;
    delay(500); //small delay to account for button bounce.
  } else if (sButtonState == HIGH && sFlag == 1) {
     lc2.clearDisplay(2);
     sFlag = 0;
     delay(500);
  }
}
