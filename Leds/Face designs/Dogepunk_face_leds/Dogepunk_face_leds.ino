/*
  Created by Ewen Paterson
  https://anu.biz/
  19/10/2018

  ---------------------------------------------------------------------------
  Ardunio will animate a default face and switch emotions when buttons are pressed
  Displaying on 3x 32x8 MAX7219 Matrix Module 4-in-1 Display.

  WORK IN PROGRESS - REV 2.5.2 17/09/2019
  ---------------------------------------------------------------------------
*/

#include "LedControl.h"
#include "binary.h"
#include <Adafruit_NeoPixel.h>

#define PIN 6
#define N_LEDS 1
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

const int resetButtonPin = 7; //Reset emotes
const int hButtonPin = A1; //Heart Face
const int dButtonPin = A2; //Dead Face
const int aButtonPin = A3; //Angry Face
const int relayPin = 2; //Relay
const int sButtonPin = A4; //Surprised

// variable for reading the pushbutton status
int resetButtonState;
int hButtonState;
int dButtonState;
int aButtonState;
int sButtonState;
int resetFlag = 0; //flag when reset button is activated
int emoteFlag = 0; //flag when emotion button is activated
int heartFlag = 0; //flag when heart button is activated
int deadFlag = 0; //flag when dead button is activated
int angryFlag = 0; //flag when angry button is activated
int surprisedFlag = 0; //flag when surprised button is activated
int pwAni = 0;

LedControl lc = LedControl(12, 11, 10, 6); //right side of face
LedControl lc1 = LedControl(8, 9, 3, 6); //left side of face
//DOUT connects to pin 12
//CLK connects to pin 11
//CS connects to pin 10
//GND connects to pin GND
//VCC connects to pin 5V Power
//6 is the number of chained matrices. 6 on each side of face

//Template
//byte tt[48] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
//               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
//               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
//               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
//               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
//               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000
//              };

//Power on face R:
byte p1[32] = {B00000000, B00000000, B00000000, B00000011, B00000011, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000
              };
byte p2[32] = {B00000000, B00000011, B00000111, B00001111, B00001111, B00000111, B00000011, B00000000,
               B00000000, B00000000, B10000000, B11000000, B11000000, B10000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000
              };
byte p3[32] = {B00000000, B00000111, B00011111, B00011111, B00011111, B00001111, B00000111, B00000000,
               B00000000, B10000000, B11000000, B11100000, B11100000, B11000000, B10000000, B00000000,
               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000
              };
byte p4[32] = {B00000000, B00000111, B00011111, B01011111, B01011111, B00001111, B00000111, B00000000,
               B00000000, B10000000, B11000000, B11101111, B11101111, B11000000, B10000000, B00000000,
               B00000000, B00000000, B00000000, B11111000, B11111000, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000
              };
byte p5[32] = {B00000000, B00000111, B00011111, B11011111, B11011111, B00001111, B00000111, B00000000,
               B00000000, B10000000, B11000000, B11101111, B11101111, B11000000, B10000000, B00000000,
               B00000000, B00000000, B00000000, B11111111, B11111111, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B11111111, B11111111, B00000000, B00000000, B00000000
              };
byte p6[48] = {B00000000, B00000111, B00011111, B11011100, B11011100, B00001111, B00000111, B00000000,
               B00000000, B10000000, B11000000, B11101111, B11101111, B11000000, B10000000, B00000000,
               B00000000, B00000000, B00000000, B11111111, B11111111, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B11111111, B11111111, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B11111111, B11111111, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B11111111, B11111111, B00000000, B00000000, B00000000
              };
byte p7[48] = {B00000000, B00001111, B00011111, B00111100, B00111100, B11011111, B11001111, B00000000,
               B00000000, B11100000, B11110000, B01111000, B01111000, B11110111, B11100111, B00000000,
               B00000000, B00000000, B00000000, B00000000, B00000000, B11111111, B11111111, B00000000,
               B00000000, B00000000, B00000000, B00000000, B00000000, B11111111, B11111111, B00000000,
               B00000000, B00000000, B01111111, B11111111, B00000000, B00000000, B00000000, B00000000,
               B00000000, B00000000, B11111000, B11111000, B00011111, B00011111, B00000000, B00000000
              };
byte p8[48] = {B00000000, B00000000, B00111111, B11111111, B11100000, B11100000, B11111111, B11111111,
               B00000000, B00000000, B11111111, B11111111, B00000011, B00000011, B11111111, B11111111,
               B00000000, B00000000, B00000000, B10000000, B11000000, B11000000, B11111111, B11111111,
               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111110, B11111110,
               B00000000, B00000000, B00001111, B00011111, B00000000, B00000000, B00000000, B00000000,
               B00000000, B00000000, B11111000, B11111000, B00001110, B00001110, B00000011, B00000000
              };
              
//Power on face L:
byte p11[32] = {B00000000, B00000000, B00000000, B00000011, B00000011, B00000000, B00000000, B00000000,
                B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
                B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
                B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000
               };
byte p22[32] = {B00000000, B00000011, B00000111, B00001111, B00001111, B00000111, B00000011, B00000000,
                B00000000, B00000000, B10000000, B11000000, B11000000, B10000000, B00000000, B00000000,
                B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
                B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000
               };
byte p33[32] = {B00000000, B00000111, B00011111, B00011111, B00011111, B00001111, B00000111, B00000000,
                B00000000, B10000000, B11000000, B11100000, B11100000, B11000000, B10000000, B00000000,
                B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
                B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000
               };
byte p44[32] = {B00000000, B00000111, B00011111, B01011111, B01011111, B00001111, B00000111, B00000000,
                B00000000, B10000000, B11000000, B11101111, B11101111, B11000000, B10000000, B00000000,
                B00000000, B00000000, B00000000, B11111000, B11111000, B00000000, B00000000, B00000000,
                B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000
               };
byte p55[32] = {B00000000, B00000111, B00011111, B11011111, B11011111, B00001111, B00000111, B00000000,
                B00000000, B10000000, B11000000, B11101111, B11101111, B11000000, B10000000, B00000000,
                B00000000, B00000000, B00000000, B11111111, B11111111, B00000000, B00000000, B00000000,
                B00000000, B00000000, B00000000, B11111111, B11111111, B00000000, B00000000, B00000000
               };
byte p66[48] = {B00000000, B00000111, B00011111, B11011100, B11011100, B00001111, B00000111, B00000000,
                B00000000, B10000000, B11000000, B11101111, B11101111, B11000000, B10000000, B00000000,
                B00000000, B00000000, B00000000, B11111111, B11111111, B00000000, B00000000, B00000000,
                B00000000, B00000000, B00000000, B11111111, B11111111, B00000000, B00000000, B00000000,
                B00000000, B00000000, B00000000, B11111111, B11111111, B00000000, B00000000, B00000000,
                B00000000, B00000000, B00000000, B11111111, B11111111, B00000000, B00000000, B00000000
               };
byte p77[48] = {B0000000, B11001111, B11011111, B00111100, B00111100, B00011111, B00001111, B00000000,
                B00000000, B11100111, B11110111, B01111000, B01111000, B11110000, B11100000, B00000000,
                B00000000, B11111111, B11111111, B00000000, B00000000, B00000000, B00000000, B00000000,
                B00000000, B11111111, B11111111, B00000000, B00000000, B00000000, B00000000, B00000000,
                B00000000, B00000000, B00000000, B00000000, B11111111, B11111111, B00000000, B00000000,
                B00000000, B00000000, B00011111, B00011111, B11111000, B11111000, B00000000, B00000000
               };
byte p88[48] = {B11111111, B11111111, B11100000, B11100000, B11111111, B00111111, B00000000, B00000000,
                B11111111, B11111111, B00000011, B00000011, B11111111, B11111111, B00000000, B00000000,
                B11111111, B11111111, B11000000, B11000000, B10000000, B00000000, B00000000, B00000000,
                B11111111, B11111111, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
                B00000000, B00000000, B00000000, B00000000, B00001111, B00011111, B00000000, B00000000,
                B00000000, B00000011, B00001110, B00001110, B11111000, B11111000, B00000000, B00000000
               };

// Default face R:
byte d1[48] = {B00000011, B00001111, B00111100, B11110000, B11000000, B11000000, B11111111, B00111111,
               B10000000, B11100000, B01111000, B00011100, B00000111, B00000011, B11111111, B11111111,
               B00000000, B00000000, B00000000, B00000000, B00000000, B11110000, B11111111, B11111111,
               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111110, B11110000,
               B00000000, B00000000, B00001111, B00011111, B00000000, B00000000, B00000000, B00000000,
               B00000000, B00000000, B11111000, B11111100, B00000110, B00000010, B00000001, B00000001
              };
byte d2[48] = {B00000000, B00000000, B00001111, B11111111, B11111000, B11000000, B11111111, B00111111,
               B00000000, B00000000, B11100000, B11111100, B00111111, B00000011, B11111111, B11111111,
               B00000000, B00000000, B00000000, B00000000, B00000000, B11110000, B11111111, B11111111,
               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111110, B11110000,
               B00000000, B00000000, B00000000, B00001111, B00011111, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B11111000, B11111100, B00000010, B00000001, B00000001
              };
byte d3[48] = {B00000000, B00000000, B00000000, B00000000, B01111111, B11111111, B11111111, B00111111,
               B00000000, B00000000, B00000000, B00000000, B11111111, B11111111, B11111111, B11111111,
               B00000000, B00000000, B00000000, B00000000, B00000000, B11110000, B11111111, B11111111,
               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111110, B11110000,
               B00000000, B00000000, B00000000, B00001111, B00011111, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B11111000, B11111100, B00000010, B00000001, B00000001
              };

// Default face L:
byte d11[48] = {B00111111, B11111111, B11000000, B11000000, B11110000, B00111100, B00001111, B00000011,
                B11111111, B11111111, B00000011, B00000111, B00011100, B01111000, B11100000, B10000000,
                B11111111, B11111111, B11110000, B00000000, B00000000, B00000000, B00000000, B00000000,
                B11110000, B11111110, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
                B00000000, B00000000, B00000000, B00000000, B00011111, B00001111, B00000000, B00000000,
                B00000001, B00000001, B00000010, B00000110, B11111100, B11111000, B00000000, B00000000
               };
byte d22[48] = {B00111111, B11111111, B11000000, B11111000, B01111111, B00001111, B00000000, B00000000,
                B11111111, B11111111, B00000011, B00111111, B11111100, B11100000, B00000000, B00000000,
                B11111111, B11111111, B11110000, B00000000, B00000000, B00000000, B00000000, B00000000,
                B11110000, B11111110, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
                B00000000, B00000000, B00000000, B00011111, B00001111, B00000000, B00000000, B00000000,
                B00000001, B00000001, B00000010, B11111100, B11111000, B00000000, B00000000, B00000000
               };
byte d33[48] = {B00111111, B11111111, B11111111, B11111111, B00000000, B00000000, B00000000, B00000000,
                B11111111, B11111111, B11111111, B11111111, B00000000, B00000000, B00000000, B00000000,
                B11111111, B11111111, B11110000, B00000000, B00000000, B00000000, B00000000, B00000000,
                B11110000, B11111110, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
                B00000000, B00000000, B00000000, B00011111, B00001111, B00000000, B00000000, B00000000,
                B00000001, B00000001, B00000010, B11111100, B11111000, B00000000, B00000000, B00000000
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

//Surprised
byte ss[48] = {B00001111, B00011111, B00011111, B11011000, B11011000, B11011000, B11011111, B00001111,
               B11000000, B11100000, B01100000, B01100000, B01100000, B01100011, B11101111, B11011111,
               B00000000, B00000000, B00000000, B00000000, B00000000, B11110000, B11111111, B11111111,
               B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111110, B11111100,
               B00000000, B00000000, B00000000, B00000000, B00011111, B00011111, B00000000, B00000000,
               B00000000, B00000000, B00000000, B00000000, B11111110, B11111110, B00000001, B00000001
              };
byte s1[48] = {B00001111, B11011111, B11011000, B11011000, B11011000, B00011111, B00011111, B00001111,
               B11011111, B11101111, B01100011, B01100000, B01100000, B01100000, B11100000, B11000000,
               B11111111, B11111111, B11110000, B00000000, B00000000, B00000000, B00000000, B00000000,
               B11111100, B11111110, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00011111, B00011111, B00000000, B00000000, B00000000, B00000000,
               B00000001, B00000001, B11111110, B11111110, B00000000, B00000000, B00000000, B00000000
              };

void setup() {
  strip.begin();
  strip.setBrightness(15);

  for (int i = 0; i < 6; i++) {
    lc.shutdown(i, false);
    lc1.shutdown(i, false);
    lc.setIntensity(i, 8);
    lc1.setIntensity(i, 8);
    lc.clearDisplay(i);
    lc1.clearDisplay(i);
  }

  // initialize the pushbutton pin as an input:
  pinMode(resetButtonPin, INPUT);
  pinMode(hButtonPin, INPUT);
  pinMode(dButtonPin, INPUT);
  pinMode(aButtonPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(sButtonPin, INPUT);
}

void powerAnimationP1() {
  //face right side
  for (int i = 0; i <= 7; i++) {
    lc.setRow(0, i, p1[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(1, i, p1[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(2, i, p1[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(3, i, p1[(i + 24)]);
  }
  //face left side
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(0, i, p11[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(1, i, p11[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(2, i, p11[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(3, i, p11[(i + 24)]);
  }
}

void powerAnimationP2() {
  for (int i = 0; i <= 7; i++) {
    lc.setRow(0, i, p2[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(1, i, p2[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(2, i, p2[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(3, i, p2[(i + 24)]);
  }
  //face left side
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(0, i, p22[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(1, i, p22[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(2, i, p22[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(3, i, p22[(i + 24)]);
  }
}
void powerAnimationP3() {
  for (int i = 0; i <= 7; i++) {
    lc.setRow(0, i, p3[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(1, i, p3[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(2, i, p3[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(3, i, p3[(i + 24)]);
  }
  //face left side
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(0, i, p33[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(1, i, p33[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(2, i, p33[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(3, i, p33[(i + 24)]);
  }
}

void powerAnimationP4() {
  for (int i = 0; i <= 7; i++) {
    lc.setRow(0, i, p4[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(1, i, p4[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(2, i, p4[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(3, i, p4[(i + 24)]);
  }
  //face left side
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(0, i, p4[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(1, i, p44[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(2, i, p44[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(3, i, p44[(i + 24)]);
  }
}

void powerAnimationP5() {
  for (int i = 0; i <= 7; i++) {
    lc.setRow(0, i, p5[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(1, i, p5[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(2, i, p5[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(3, i, p5[(i + 24)]);
  }
  //face left side
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(0, i, p55[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(1, i, p55[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(2, i, p55[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(3, i, p55[(i + 24)]);
  }
}

void powerAnimationP6() {
  for (int i = 0; i <= 7; i++) {
    lc.setRow(0, i, p6[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(1, i, p6[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(2, i, p6[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(3, i, p6[(i + 24)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(4, i, p6[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(5, i, p6[(i + 40)]);
  }
  //face left side
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(0, i, p66[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(1, i, p66[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(2, i, p66[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(3, i, p66[(i + 24)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(4, i, p66[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(5, i, p66[(i + 40)]);
  }
}

void powerAnimationP7() {
  for (int i = 0; i <= 7; i++) {
    lc.setRow(0, i, p7[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(1, i, p7[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(2, i, p7[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(3, i, p7[(i + 24)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(4, i, p7[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(5, i, p7[(i + 40)]);
  }
  //face left side
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(0, i, p77[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(1, i, p77[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(2, i, p77[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(3, i, p77[(i + 24)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(4, i, p77[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(5, i, p77[(i + 40)]);
  }
}

void powerAnimationP8() {
  for (int i = 0; i <= 7; i++) {
    lc.setRow(0, i, p8[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(1, i, p8[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(2, i, p8[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(3, i, p8[(i + 24)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(4, i, p8[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(5, i, p8[(i + 40)]);
  }
  //face left side
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(0, i, p88[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(1, i, p88[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(2, i, p88[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(3, i, p88[(i + 24)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(4, i, p88[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(5, i, p88[(i + 40)]);
  }
}

void defaultAnimation() {
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
  for (int i = 0; i <= 7; i++) {
    lc.setRow(4, i, d1[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(5, i, d1[(i + 40)]);
  }
  //Default face left side
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(0, i, d11[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(1, i, d11[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(2, i, d11[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(3, i, d11[(i + 24)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(4, i, d11[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(5, i, d11[(i + 40)]);
  }
}

void defaultAnimation1() {
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
  for (int i = 0; i <= 7; i++) {
    lc.setRow(4, i, d2[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(5, i, d2[(i + 40)]);
  }
  //Default face left side
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(0, i, d22[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(1, i, d22[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(2, i, d22[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(3, i, d22[(i + 24)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(4, i, d22[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(5, i, d22[(i + 40)]);
  }
}

void defaultAnimation2() {
  //Default face right side
  for (int i = 0; i <= 7; i++) {
    lc.setRow(0, i, d3[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(1, i, d3[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(2, i, d3[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(3, i, d3[(i + 24)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(4, i, d3[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(5, i, d3[(i + 40)]);
  }
  //Default face left side
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(0, i, d33[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(1, i, d33[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(2, i, d33[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(3, i, d33[(i + 24)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(4, i, d33[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(5, i, d33[(i + 40)]);
  }
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

void surprisedFace() {
  for (int i = 0; i <= 7; i++) {
    lc.setRow(0, i, ss[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(1, i, ss[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(2, i, ss[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(3, i, ss[(i + 24)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(4, i, ss[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc.setRow(5, i, ss[(i + 40)]);
  }
  //Left Side
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(0, i, s1[i]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(1, i, s1[(i + 8)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(2, i, s1[(i + 16)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(3, i, s1[(i + 24)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(4, i, s1[(i + 32)]);
  }
  for (int i = 0; i <= 7; i++) {
    lc1.setRow(5, i, s1[(i + 40)]);
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

void check() {
  // read the state of the pushbutton value:
  resetButtonState = digitalRead(resetButtonPin);
  hButtonState = digitalRead(hButtonPin);
  dButtonState = digitalRead(dButtonPin);
  aButtonState = digitalRead(aButtonPin);
  sButtonState = digitalRead(sButtonPin);
  if (resetButtonState == HIGH) {
    resetFlag = 1;
  }
  if (hButtonState == HIGH || dButtonState == HIGH || aButtonState == HIGH || sButtonState == HIGH) {
    emoteFlag = 1;
  }
  if (hButtonState == HIGH) {
    heartFlag = 1;
  }
  if (dButtonState == HIGH) {
    deadFlag = 1;
  }
  if (aButtonState == HIGH) {
    angryFlag = 1;
  }
  if (sButtonState == HIGH) {
    surprisedFlag = 1;
  }
}

void loop() {
  if (pwAni == 0) {
    powerAnimationP1();
    delay(500);
    clearAll();
    delay(500);
    powerAnimationP1();
    delay(500);
    clearAll();
    delay(500);
    powerAnimationP1();
    delay(500);
    clearAll();
    delay(500);
    powerAnimationP1();
    delay(50);
    powerAnimationP2();
    delay(50);
    powerAnimationP3();
    delay(50);
    powerAnimationP4();
    delay(50);
    powerAnimationP5();
    delay(50);
    powerAnimationP6();
    delay(50);
    powerAnimationP7();
    delay(50);
    powerAnimationP8();
    delay(50);
    defaultAnimation1();
    delay(100);
    defaultAnimation2();
    delay(100);
    defaultAnimation1();
    delay(100);
    digitalWrite(relayPin, HIGH);
    pwAni = 1;
  }

  if (emoteFlag == 0) {
    for (int l = 0; l <= 20 && emoteFlag == 0 && resetFlag == 0; l++) {
      defaultAnimation();
      //strip.setPixelColor(n, red, green, blue, white);
      strip.setPixelColor(0, 0, 0, 0, 255);
      strip.show();
      delay(25);
      check();
    }
    defaultAnimation1();
    delay(25);
    defaultAnimation2();
    delay(25);
    check();
  }
  check();
  if (emoteFlag == 1) {
    check();
    // If heart button is HIGH:
    if (heartFlag == 1) {
      heartFace();
      //strip.setPixelColor(n, red, green, blue, white);
      strip.setPixelColor(0, 255, 0, 0, 000);
      strip.show();
      heartFlag = 0;
      delay(250); //small delay to account for button bounce.
    }
    // If dead button is HIGH:
    if (deadFlag == 1) {
      deadFace();
      //strip.setPixelColor(n, red, green, blue, white);
      strip.setPixelColor(0, 0, 255, 0, 000);
      strip.show();
      deadFlag = 0;
      delay(250); //small delay to account for button bounce.
    }
    // If angry button is HIGH:
    if (angryFlag == 1) {
      angryFace();
      //strip.setPixelColor(n, red, green, blue, white);
      strip.setPixelColor(0, 0, 0, 255, 000);
      strip.show();
      angryFlag = 0;
      delay(250); //small delay to account for button bounce.
    }
    // If surprised button is HIGH:
    if (surprisedFlag == 1) {
      surprisedFace();
      //strip.setPixelColor(n, red, green, blue, white);
      strip.setPixelColor(0, 255, 255, 255, 000);
      strip.show();
      surprisedFlag = 0;
      delay(250); //small delay to account for button bounce.
    }
    if (resetFlag == 1) {
      //clearAll();
      emoteFlag = 0;
      resetFlag = 0;
      strip.setPixelColor(0, 000, 000, 000, 000);
      strip.show();
      delay(250); //small delay to account for button bounce.
    }
  }
  // If reset button is HIGH:
  if (resetFlag == 1) {
    //clearAll();
    emoteFlag = 0;
    resetFlag = 0;
    strip.setPixelColor(0, 000, 000, 000, 000);
    strip.show();
    delay(250); //small delay to account for button bounce.
  }
}
