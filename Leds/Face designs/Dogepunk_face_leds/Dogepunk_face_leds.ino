/*
Created by Ewen Paterson
https://anu.biz/
17/10/2018

---------------------------------------------------------------------------
Ardunio will animate a default face and switch emotions when buttons are pressed
Displaying on 32x8 MAX7219 Matrix Module 4-in-1 Display. 

WORK IN PROGRESS - REV 3 23/10/2018
---------------------------------------------------------------------------
*/

#include "LedControl.h"
#include "binary.h"
const int pwButtonPin = 7; //Switch all on or off
const int resetButtonPin = 6; //Reset emotes
const int hButtonPin = 5; //Heart Face
const int dButtonPin = 4; //Dead Face
const int aButtonPin = 3; //Angry Face
const int sButtonPin = 2; //Sad face

int pwButtonState; // variable for reading the pushbutton status
int resetButtonState;
int hButtonState;
int dButtonState;
int aButtonState; 
int sButtonState;   
int pwFlag = 0; //flag when button is toggled

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
byte f1[32]= {B00001110,B00011111,B00011111,B11011111,B11001111,B11000111,B11000011,B00000001, 
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
  pinMode(resetButtonPin, INPUT);
  pinMode(hButtonPin, INPUT);
  pinMode(dButtonPin, INPUT);
  pinMode(aButtonPin, INPUT); 
  pinMode(sButtonPin, INPUT);      
} 

void defaultAnimation(){
  for(int i=0; i<=7; i++){
      lc.setRow(0,i,ff[i]);
    }
  for(int i=0; i<=7; i++){
      lc.setRow(1,i,ff[(i+8)]);
    }
  for(int i=0; i<=7; i++){
      lc.setRow(2,i,ff[(i+16)]);
    }
   for(int i=0; i<=7; i++){
      lc.setRow(3,i,ff[(i+24)]);
    }
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

void deadFace(){
  for(int i=0; i<=7; i++){
      lc.setRow(0,i,f2[i]);
    }
  for(int i=0; i<=7; i++){
      lc.setRow(1,i,f2[(i+8)]);
    }
  for(int i=0; i<=7; i++){
      lc.setRow(2,i,f2[(i+16)]);
    }
   for(int i=0; i<=7; i++){
      lc.setRow(3,i,f2[(i+24)]);
    }
}

void angryFace(){
  for(int i=0; i<=7; i++){
      lc.setRow(0,i,f3[i]);
    }
  for(int i=0; i<=7; i++){
      lc.setRow(1,i,f3[(i+8)]);
    }
  for(int i=0; i<=7; i++){
      lc.setRow(2,i,f3[(i+16)]);
    }
   for(int i=0; i<=7; i++){
      lc.setRow(3,i,f3[(i+24)]);
    }
}

void sadFace(){
  for(int i=0; i<=7; i++){
      lc.setRow(0,i,f4[i]);
    }
  for(int i=0; i<=7; i++){
      lc.setRow(1,i,f4[(i+8)]);
    }
  for(int i=0; i<=7; i++){
      lc.setRow(2,i,f4[(i+16)]);
    }
   for(int i=0; i<=7; i++){
      lc.setRow(3,i,f4[(i+24)]);
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
  resetButtonState = digitalRead(resetButtonPin);
  hButtonState = digitalRead(hButtonPin);
  dButtonState = digitalRead(dButtonPin);
  aButtonState = digitalRead(aButtonPin);
  sButtonState = digitalRead(sButtonPin);

  // If Power button is HIGH:
  if (pwButtonState == HIGH && pwFlag == 0) {     
    defaultAnimation();
    pwButtonState == LOW;
    pwFlag = 1;
    delay(250);
  } else if (pwButtonState == HIGH && pwFlag == 1) {     
    clearAll();
    pwButtonState == LOW;
    pwFlag = 0;
    delay(250);
  }

  //If pw button is on
  if (pwFlag == 1){

    // If reset button is HIGH:
    if (resetButtonState == HIGH) {     
    defaultAnimation();
    hButtonState == LOW;
    delay(250); //small delay to account for button bounce.
    }
    
    // If heart button is HIGH:
    if (hButtonState == HIGH) {     
    heartFace();
    hButtonState == LOW;
    delay(250); //small delay to account for button bounce.
    }
    
    // If dead button is HIGH:
    if (dButtonState == HIGH) {     
    deadFace();
    dButtonState == LOW;
    delay(250); //small delay to account for button bounce.
    }
  
    // If angry button is HIGH:
    if (aButtonState == HIGH) {     
    angryFace();
    aButtonState == LOW;
    delay(250); //small delay to account for button bounce.
    }

    // If sad button is HIGH:
    if (sButtonState == HIGH) {     
    sadFace();
    sButtonState == LOW;
    delay(250); //small delay to account for button bounce.
    }
    
  }
}
