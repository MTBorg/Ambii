#pragma once
#include "FastLED.h"

//Setup-specific values
#define PIXEL_AMOUNT 30 //The total amount of pixels to read from
#define BAUD_RATE 57600
#define PIN_OUTPUT 13 //The data pin the LED strip is connected to

//Options
#define INTERPOLATION_RATE 30

CRGB pixelBuffer[PIXEL_AMOUNT];
CRGB interPolBuffer[PIXEL_AMOUNT];

//Function prototypes
void setup();
void loop();

/*
 * Waits for the pixel buffer to be filled.
 */
void readBuffer(){
  for(unsigned int i = 0;i < PIXEL_AMOUNT; i++){ //DEBUG, should be PIXEL_AMOUNT instead of 1
    while(Serial.available() < 4); //Wait for the entire pixel to be read 

    //Read the data (the RGB structs sent from the host is in order b,g,r, reserved)
    pixelBuffer[i].b = Serial.read();
    pixelBuffer[i].g = Serial.read();
    pixelBuffer[i].r = Serial.read();   
    Serial.read(); //Throw away the fourth (reserved) byte
    // Serial.print("R: "); Serial.println(pixelBuffer[i].r, DEC);

    //DEBUG    
    /*Serial.print("I received: ");
    Serial.print(pixelBuffer[i].r, DEC);
    Serial.print(",");
    Serial.print(pixelBuffer[i].g), DEC;
    Serial.print(",");
    Serial.println(pixelBuffer[i].b, DEC);*/
  }
}

void setup(){
  Serial.begin(BAUD_RATE);
  FastLED.addLeds<NEOPIXEL, PIN_OUTPUT>(pixelBuffer, PIXEL_AMOUNT);
}

void loop(){
  readBuffer();
  //DEBUG
  /*for(unsigned char i = 1; i < PIXEL_AMOUNT; i++){
    pixelBuffer[i].r = pixelBuffer[0].r;
    pixelBuffer[i].g = pixelBuffer[0].g;
    pixelBuffer[i].b = pixelBuffer[0].b;
  }*/
  FastLED.show();
}

