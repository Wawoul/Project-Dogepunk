/*
Modified by Ewen Paterson
https://anu.biz/
13/10/2018

---------------------------------------------------------------------------
Ardunio will display faces on a 8x8 module. 
---------------------------------------------------------------------------
*/

#include "LedControl.h"
#include "binary.h"

byte max_units = 4; //4 as we are only using 4 MAX7219
LedControl lc = LedControl(12,11,10,max_units);
//DOUT connects to pin 12
//CLK connects to pin 11
//CS connects to pin 10 


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
//  // turn off power saving, enables display
//  lc.shutdown(0,false);
//
//  // Set the intensity of the LEDs (Max 15)
//  // setIntensity(int addr, int intensity):
//  lc.setIntensity(0,8);
//
//  // Clear all LEDs:
//  lc.clearDisplay(0);
} 

void displayFaces(){
  //setRow(addr,row,value)
  //setRow(addr,col,value)
  //setLed(addr, row, col, state)
  
  // Happy face:
  lc.setRow(0,0,ff[0]);
  lc.setRow(0,1,ff[1]);
  lc.setRow(0,2,ff[2]);
  lc.setRow(0,3,ff[3]);
  lc.setRow(0,4,ff[4]);
  lc.setRow(0,5,ff[5]);
  lc.setRow(0,6,ff[6]);
  lc.setRow(0,7,ff[7]);
  delay(delaytime);

  // Netural face:
  lc.setRow(1,0,f1[0]);
  lc.setRow(1,1,f1[1]);
  lc.setRow(1,2,f1[2]);
  lc.setRow(1,3,f1[3]);
  lc.setRow(1,4,f1[4]);
  lc.setRow(1,5,f1[5]);
  lc.setRow(1,6,f1[6]);
  lc.setRow(1,7,f1[7]);
  delay(delaytime);
 
  // Sad face:
  lc.setRow(2,0,f2[0]);
  lc.setRow(2,1,f2[1]);
  lc.setRow(2,2,f2[2]);
  lc.setRow(2,3,f2[3]);
  lc.setRow(2,4,f2[4]);
  lc.setRow(2,5,f2[5]);
  lc.setRow(2,6,f2[6]);
  lc.setRow(2,7,f2[7]);
  delay(delaytime);

  // Misc face:
  lc.setRow(3,0,f3[0]);
  lc.setRow(3,1,f3[1]);
  lc.setRow(3,2,f3[2]);
  lc.setRow(3,3,f3[3]);
  lc.setRow(3,4,f3[4]);
  lc.setRow(3,5,f3[5]);
  lc.setRow(3,6,f3[6]);
  lc.setRow(3,7,f3[7]);
  delay(delaytime);
}

void set_unit(byte number_of_unit){

  // turn off power saving, enables display
  // shutdown(addr, state)
  lc.shutdown(number_of_unit-1,false);
  // Set the intensity of the LEDs (Max 15)
  // setIntensity(int addr, int intensity):
  lc.setIntensity(number_of_unit-1,2);
  // Clear all LEDs:
  lc.clearDisplay(number_of_unit-1);
}

void loop(){
  displayFaces();
  for(byte i=1;i<6;i++)
{
  set_unit(i);
  }
}
