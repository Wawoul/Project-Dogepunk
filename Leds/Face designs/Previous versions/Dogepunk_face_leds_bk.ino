/*
  Created by Ewen Paterson
  https://anu.biz/
  19/10/2018

  ---------------------------------------------------------------------------
  Ardunio will animate a default face and switch emotions when buttons are pressed
  Displaying on 3x 32x8 MAX7219 Matrix Module 4-in-1 Display.

  WORK IN PROGRESS - REV 11 05/01/2019
  ---------------------------------------------------------------------------
*/

#include "LedControl.h"
#include "binary.h"
const int pwButtonPin = 7; //Switch all on or off
const int resetButtonPin = 6; //Reset emotes
const int hButtonPin = 5; //Heart Face
const int dButtonPin = 4; //Dead Face
const int aButtonPin = 2; //Angry Face
const int gButtonPin = 19; //Glitch

int pwButtonState; // variable for reading the pushbutton status
int resetButtonState;
int hButtonState;
int dButtonState;
int aButtonState;
int gButtonState;
int pwFlag = 0; //flag when power button is activated
int emoteFlag = 0; //flag when emotion button is activated

LedControl lc = LedControl(12, 11, 10, 6); //right side of face
LedControl lc1 = LedControl(8, 9, 3, 6); //left side of face
//DOUT connects to pin 12
//CLK connects to pin 11
//CS connects to pin 10
//GND connects to pin GND
//VCC connects to pin 5V Power
//6 is the number of chained matrices. 6 on each side of face

//Power on:
byte pp[32] = {B11110011, B11111011, B11011011, B11011011, B11011011, B11011011, B11111011, B11110011,
               B11101111, B11101111, B01101100, B01101100, B01101101, B01101101, B11101111, B11101111,
               B10111110, B10111110, B00110000, B00111110, B10111110, B10110000, B10111110, B10111110,
               B00000000, B00000000, B00000000, B01110111, B01010100, B01010111, B01010001, B11110111
              };

// Default face R:
byte dd[48] = {B00000011, B00001111, B00111100, B11110000, B11000000, B11000000, B11111111, B00111111,
               B10000000, B11100000, B01111000, B00011100, B00000111, B00000011, B11111111, B11111111,
               B00000000, B00000000, B00000000, B00000000, B00000000, B11110000, B11111111, B11111111,
               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111110, B11110000,
               B00000000, B00000000, B00001111, B00011111, B00000000, B00000000, B00000000, B00000000,
               B00000000, B00000000, B11111000, B11111100, B00000110, B00000010, B00000001, B00000001
              };
byte d1[32] = {B00000000, B00000000, B00001111, B11111111, B11111000, B11000000, B11111111, B00111111,
               B00000000, B00000000, B11100000, B11111100, B00111111, B00000011, B11111111, B11111111,
               B00000000, B00000000, B00000000, B00000000, B00000000, B11110000, B11111111, B11111111,
               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111110, B11110000
              };
byte d2[32] = {B00000000, B00000000, B00000000, B00000000, B01111111, B11111111, B11111111, B00111111,
               B00000000, B00000000, B00000000, B00000000, B11111111, B11111111, B11111111, B11111111,
               B00000000, B00000000, B00000000, B00000000, B00000000, B11110000, B11111111, B11111111,
               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111110, B11110000
              };

// Default face L:
byte d3[48] = {B00111111, B11111111, B11000000, B11000000, B11110000, B00111100, B00001111, B00000011,
               B11111111, B11111111, B00000011, B00000111, B00011100, B01111000, B11100000, B10000000,
               B11111111, B11111111, B11110000, B00000000, B00000000, B00000000, B00000000, B00000000,
               B11110000, B11111110, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B00000000, B00011111, B00001111, B00000000, B00000000,
               B00000001, B00000001, B00000010, B00000110, B11111100, B11111000, B00000000, B00000000
              };
byte d4[32] = {B00111111, B11111111, B11000000, B11111000, B01111111, B00001111, B00000000, B00000000,
               B11111111, B11111111, B00000011, B00111111, B11111100, B11100000, B00000000, B00000000,
               B11111111, B11111111, B11110000, B00000000, B00000000, B00000000, B00000000, B00000000,
               B11110000, B11111110, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000
              };
byte d5[32] = {B00111111, B11111111, B11111111, B11111111, B00000000, B00000000, B00000000, B00000000,
               B11111111, B11111111, B11111111, B11111111, B00000000, B00000000, B00000000, B00000000,
               B11111111, B11111111, B11110000, B00000000, B00000000, B00000000, B00000000, B00000000,
               B11110000, B11111110, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000
              };

//Heart face
byte hh[48] = {B00001110, B00011111, B00011111, B11011111, B11001111, B11000111, B11000011, B00000001,
               B11100000, B11110000, B11110000, B11110000, B11100000, B11000011, B10001111, B00001111,
               B00000000, B00000000, B00000000, B00000000, B00000000, B11110000, B11111111, B11111111,
               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111110, B11110000,
               B00000000, B00000000, B00001110, B00011111, B00000001, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B11100000, B11111110, B00011110, B00000001, B00000001
              };
byte h1[48] = {B00000001, B11000011, B11000111, B11001111, B11011111, B00011111, B00011111, B00001110,
               B00001111, B10001111, B11000011, B11100000, B11110000, B11110000, B11110000, B11100000,
               B11111111, B11111111, B11110000, B00000000, B00000000, B00000000, B00000000, B00000000,
               B11110000, B11111110, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B00000001, B00011111, B00001110, B00000000, B00000000,
               B00000001, B00000001, B00011110, B11111110, B11100000, B00000000, B00000000, B00000000
              };
//byte h1[32] = {B00000000, B00000000, B00000000, B11000000, B11001111, B11000111, B11000011, B00000001,
//               B00000000, B00000000, B00000000, B00000000, B11100000, B11000011, B10001111, B00001111,
//               B00000000, B00000000, B00000000, B00000000, B00000000, B11110000, B11111111, B11111111,
//               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111110, B11110000
//              }; 
//byte h2[32] = {B00000000, B00000000, B00000000, B11000000, B11000000, B11000000, B11000000, B00000000,
//               B00000000, B00000000, B00000000, B00000000, B00000000, B00000011, B00001111, B00001111,
//               B00000000, B00000000, B00000000, B00000000, B00000000, B11110000, B11111111, B11111111,
//               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111110, B11110000
//              };                       

//Dead face
byte xx[48] = {B00011000, B00011100, B00001100, B11000011, B11000011, B11001100, B11011100, B00011000,
               B01100000, B11100000, B11000000, B00000000, B00000000, B11000011, B11101111, B01101111,
               B00000000, B00000000, B00000000, B00000000, B00000000, B11110000, B11111111, B11111111,
               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111110, B11110000,
               B00000000, B00000000, B00001011, B00000100, B00001011, B00000000, B00000000, B00000000,
               B00000000, B00000000, B01101000, B10010000, B01101000, B00000101, B00000010, B00000101
              };
byte x1[48] = {B00011000, B11011100, B11001100, B11000011, B11000011, B00001100, B00011100, B00011000,
               B01101111, B11101111, B11000011, B00000000, B00000000, B11000000, B11100000, B01100000,
               B11111111, B11111111, B11110000, B00000000, B00000000, B00000000, B00000000, B00000000,
               B11110000, B11111110, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B00001011, B00000100, B00001011, B00000000, B00000000,
               B00000101, B00000010, B00000101, B01101000, B10010000, B01101000, B00000000, B00000000
              };

//Angry face
byte aa[48] = {B00000000, B00000000, B00000000, B11111111, B11111111, B11000000, B11111111, B00111111,
               B00000000, B00000000, B00000000, B11110000, B11111100, B00000011, B11111111, B11111111,
               B00000000, B00000000, B00000000, B00000000, B00000000, B11110000, B11111111, B11111111,
               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111110, B11110000,
               B00000000, B00000000, B00000000, B00000000, B00001111, B00011111, B00000000, B00000000,
               B00000000, B00000000, B00000000, B00000000, B11111100, B11111110, B00000011, B00000001
              };
byte a1[48] = {B00111111, B11111111, B11000000, B11111111, B11111111, B00000000, B00000000, B00000000,
               B11111111, B11111111, B00000011, B11111100, B11110000, B00000000, B00000000, B00000000,
               B11111111, B11111111, B11110000, B00000000, B00000000, B00000000, B00000000, B00000000,
               B11110000, B11111110, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00011111, B00001111, B00000000, B00000000, B00000000, B00000000,
               B00000001, B00000011, B11111110, B11111100, B00000000, B00000000, B00000000, B00000000
              };

//Glitch
byte gg[32] = {B00000011, B00001111, B00111100, B11110000, B11000000, B11001000, B11111111, B00111111,
               B10000000, B11100000, B01111000, B00011100, B00111111, B00000011, B11111111, B11111111,
               B00000000, B00111100, B00000000, B00111100, B00000000, B11110000, B11100111, B11111111,
               B00000000, B00111000, B00110000, B00000000, B00111000, B00000000, B11111110, B11110000
             };

void setup() {
  for(int i = 0; i < 6; i++) {
    lc.shutdown(i,false);
    lc1.shutdown(i,false);
    lc.setIntensity(i,8);
    lc1.setIntensity(i,8);
    lc.clearDisplay(i);
    lc1.clearDisplay(i);
  }
  
  // initialize the pushbutton pin as an input:
  pinMode(pwButtonPin, INPUT);
  pinMode(resetButtonPin, INPUT);
  pinMode(hButtonPin, INPUT);
  pinMode(dButtonPin, INPUT);
  pinMode(aButtonPin, INPUT);
  pinMode(gButtonPin, INPUT);
}

void powerAnimation() {
  for (int i = 0; i <= 7; i++) {
    lc.setRow(0, i, pp[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(1, i, pp[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(2, i, pp[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(3, i, pp[(i + 24)]);
  }
  delay(500);
}

void defaultAnimation() {
  //Default right right side
  for (int i = 0; i <= 7; i++) {
    lc.setRow(0, i, dd[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(1, i, dd[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(2, i, dd[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(3, i, dd[(i + 24)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(4, i, dd[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(5, i, dd[(i + 40)]);
  }
  //Default face left side
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(0, i, d3[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(1, i, d3[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(2, i, d3[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(3, i, d3[(i + 24)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(4, i, d3[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(5, i, d3[(i + 40)]);
  }
}

void defaultAnimation1() {
  //Default face right side
  for (int i = 0; i <= 7; i++) {
    lc.setRow(0, i, d1[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(1, i, d1[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(2, i, d1[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(3, i, d1[(i + 24)]);
  }
  //Default face left side
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(0, i, d4[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(1, i, d4[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(2, i, d4[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(3, i, d4[(i + 24)]);
  }
  delay(30);
}

void defaultAnimation2() {
  //Default face right side
  for (int i = 0; i <= 7; i++) {
    lc.setRow(0, i, d2[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(1, i, d2[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(2, i, d2[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(3, i, d2[(i + 24)]);
  }
  //Default face left side
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(0, i, d5[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(1, i, d5[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(2, i, d5[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(3, i, d5[(i + 24)]);
  }
  delay(50);
}

void heartFace() {
  for (int i = 0; i <= 7; i++) {
    lc.setRow(0, i, hh[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(1, i, hh[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(2, i, hh[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(3, i, hh[(i + 24)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(4, i, hh[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(5, i, hh[(i + 40)]);
  }
  //Left side
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(0, i, h1[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(1, i, h1[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(2, i, h1[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(3, i, h1[(i + 24)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(4, i, h1[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(5, i, h1[(i + 40)]);
  }
}

void deadFace() {
  for (int i = 0; i <= 7; i++) {
    lc.setRow(0, i, xx[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(1, i, xx[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(2, i, xx[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(3, i, xx[(i + 24)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(4, i, xx[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(5, i, xx[(i + 40)]);
  }
  //Left side
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(0, i, x1[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(1, i, x1[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(2, i, x1[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(3, i, x1[(i + 24)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(4, i, x1[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(5, i, x1[(i + 40)]);
  }
}

void angryFace() {
  for (int i = 0; i <= 7; i++) {
    lc.setRow(0, i, aa[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(1, i, aa[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(2, i, aa[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(3, i, aa[(i + 24)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(4, i, aa[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(5, i, aa[(i + 40)]);
  }
  //Left side
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(0, i, a1[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(1, i, a1[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(2, i, a1[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(3, i, a1[(i + 24)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(4, i, a1[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(5, i, a1[(i + 40)]);
  }
}

void glitch() {
  for (int i = 0; i <= 7; i++) {
    lc.setRow(0, i, gg[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(1, i, gg[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(2, i, gg[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(3, i, gg[(i + 24)]);
  }
}

void clearAll() {
  for (int i = 0; i < 6; i++) {
    lc.clearDisplay(i);
  }
  for (int i = 0; i < 6; i++) {
    lc1.clearDisplay(i);
  }
}

void readButtons() {
  // read the state of the pushbutton value:
  pwButtonState = digitalRead(pwButtonPin);
  resetButtonState = digitalRead(resetButtonPin);
  hButtonState = digitalRead(hButtonPin);
  dButtonState = digitalRead(dButtonPin);
  aButtonState = digitalRead(aButtonPin);
  gButtonState = digitalRead(gButtonPin);
  if(pwButtonState == HIGH || hButtonState == HIGH || dButtonState == HIGH || aButtonState == HIGH || gButtonState == HIGH){
    emoteFlag = 1;
  }
}

void loop() {
  readButtons();

  // If Power button is HIGH:
  if (pwButtonState == HIGH && pwFlag == 0) {
    powerAnimation();
    emoteFlag = 0;
    pwFlag = 1;
    pwButtonState == LOW;
    delay(250);
  } else if (pwButtonState == HIGH && pwFlag == 1) {
    clearAll();
    pwFlag = 0;
    pwButtonState == LOW;
    delay(250);
  }

  //If pw button is on
  if (pwFlag == 1) {

    if (emoteFlag == 0) {
      for (int l = 0; l <= 30 && emoteFlag == 0; l++) {
        defaultAnimation();
        delay(50);
        readButtons();
      }
     defaultAnimation1();
     defaultAnimation2();
    }

    // If reset button is HIGH:
    if (resetButtonState == HIGH) {
      emoteFlag = 0;
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

    // If Glitch button is HIGH:
    if (gButtonState == HIGH) {
      glitch();
      gButtonState == LOW;
      delay(250); //small delay to account for button bounce.
    }
  }
}
