/*
Modified by Ewen Paterson
https://anu.biz/
17/10/2018

---------------------------------------------------------------------------
Ardunio will animate a default face and switch emotions when buttons are pressed
Displaying on 32x8 MAX7219 Matrix Module 4-in-1 Display. 

WORK IN PROGRESS - REV 1
---------------------------------------------------------------------------
*/

#include "LedControl.h"
#include "binary.h"
const int pwButtonPin = 7; //Switch all on or off
const int hButtonPin = 6; //Heart Face
const int dButtonPin = 5; //Dead Face
const int aButtonPin = 4; //Angry Face
const int sButtonPin = 3; //Sad face


int pwButtonState; // variable for reading the pushbutton status
int hButtonState;
int dButtonState;
int aButtonState; 
int sButtonState;   
int pwFlag = 0; //flag when button is toggled
int hFlag = 0;
int dFlag = 0;
int aFlag = 0;
int sFlag = 0;

byte max_units = 4; 
//4 as we are only using 4 MAX7219
LedControl lc = LedControl(12,11,10,max_units);
//DOUT connects to pin 12
//CLK connects to pin 11
//CS connects to pin 10 
//GND connects to pin GND
//VCC connects to pin 5V Power

// Default face:
byte ff[32]= {B00000011,B00001111,B00111100,B11110000,B11000000,B11000000,B11111111,B00111111, 
             B10000000,B11100000,B01111000,B00011100,B00000111,B00000011,B11111111,B11111111,
             B00000000,B00000000,B00000000,B00000000,B00000000,B11110000,B11111111,B11111111,
             B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B11111110,B11110000};
//Heart face
byte f1[32]= {B00001110,B00011111,B00011111,B11011111,B11011111,B11000111,B11000011,B00000001, 
             B11100000,B11110000,B11110000,B11110000,B11100000,B11000011,B10001111,B00001111,
             B00000000,B00000000,B00000000,B00000000,B00000000,B11110000,B11111111,B11111111,
             B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B11111110,B11110000};

//Dead face
byte f2[32]= {B00011000,B00011100,B00001100,B11000011,B11000011,B11001100,B11011100,B00011000, 
             B01100000,B11100000,B11000000,B00000000,B00000000,B11000011,B11101111,B01101111,
             B00000000,B00000000,B00000000,B00000000,B00000000,B11110000,B11111111,B11111111,
             B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B11111110,B11110000};

//Angry face
byte f3[32]= {B00000000,B00000000,B00000000,B11111111,B11111111,B11000000,B11111111,B00111111, 
             B00000000,B00000000,B00000000,B11110000,B11111100,B00000011,B11111111,B11111111,
             B00000000,B00000000,B00000000,B00000000,B00000000,B11110000,B11111111,B11111111,
             B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B11111110,B11110000};

//Sad face
byte f4[32]= {B00000011,B00001111,B00111100,B11110000,B11000000,B11001000,B11111111,B00111111, 
             B10000000,B11100000,B01111000,B00011100,B00111111,B00000011,B11111111,B11111111,
             B00000000,B00111100,B00000000,B00111100,B00000000,B11110000,B11100111,B11111111,
             B00000000,B00111000,B00110000,B00000000,B00111000,B00000000,B11111110,B11110000};

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
  pinMode(pwButtonPin, INPUT);
  pinMode(hButtonPin, INPUT);
  pinMode(dButtonPin, INPUT);
  pinMode(aButtonPin, INPUT); 
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
  lc.setRow(1,0,ff[8]);
  lc.setRow(1,1,ff[9]);
  lc.setRow(1,2,ff[10]);
  lc.setRow(1,3,ff[11]);
  lc.setRow(1,4,ff[12]);
  lc.setRow(1,5,ff[13]);
  lc.setRow(1,6,ff[14]);
  lc.setRow(1,7,ff[15]);
  lc.setRow(2,0,ff[16]);
  lc.setRow(2,1,ff[17]);
  lc.setRow(2,2,ff[18]);
  lc.setRow(2,3,ff[19]);
  lc.setRow(2,4,ff[20]);
  lc.setRow(2,5,ff[21]);
  lc.setRow(2,6,ff[22]);
  lc.setRow(2,7,ff[23]);
  lc.setRow(3,0,ff[24]);
  lc.setRow(3,1,ff[25]);
  lc.setRow(3,2,ff[26]);
  lc.setRow(3,3,ff[27]);
  lc.setRow(3,4,ff[28]);
  lc.setRow(3,5,ff[29]);
  lc.setRow(3,6,ff[30]);
  lc.setRow(3,7,ff[31]);
}

void heartFace(){
  for(int i=0; i<=7; i++){
      lc.setRow(0,i,f1[i]);
    }
  for(int i=0; i<=7; i++){
      lc.setRow(1,i,f1[(i+8)]);
    }
  for(int i=0; i<=7; i++){
      lc.setRow(2,i,f1[(i+16)]);
    }
   for(int i=0; i<=7; i++){
      lc.setRow(3,i,f1[(i+24)]);
    }
}

void clearAll(){
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  lc.clearDisplay(2);
  lc.clearDisplay(3);
}

void loop() {
  // read the state of the pushbutton value:
  pwButtonState = digitalRead(pwButtonPin);
  hButtonState = digitalRead(hButtonPin);
  dButtonState = digitalRead(dButtonPin);
  aButtonState = digitalRead(aButtonPin);
  sButtonState = digitalRead(sButtonPin);

  // If Heart button is HIGH:
  if (hButtonState == HIGH && hFlag == 0) {     
    happyface();
    hFlag = 1;
    hButtonState == LOW;
    delay(250); //small delay to account for button bounce.
  } else if (hButtonState == HIGH && hFlag == 1) {
     clearAll();
     hFlag = 0;
     delay(250);
  }

  // If Dead button is HIGH:
  if (dButtonState == HIGH && dFlag == 0) {     
    heartFace();
    dFlag = 1;
    dButtonState == LOW;
    delay(250); //small delay to account for button bounce.
  } else if (dButtonState == HIGH && dFlag == 1) {
     clearAll();
     dFlag = 0;
     delay(250);
  }

//   if (sButtonState == HIGH && sFlag == 0) {     
//    sadface();
//    sFlag = 1;
//    sButtonState == LOW;
//    delay(500); //small delay to account for button bounce.
//  } else if (sButtonState == HIGH && sFlag == 1) {
//     lc.clearDisplay(2);
//     sFlag = 0;
//     delay(500);
//  }
}
