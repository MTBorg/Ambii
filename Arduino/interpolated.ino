#include "FastLED.h"

#define BAUD_RATE 57600
#define PIN_OUTPUT 13
#define PIXEL_AMOUNT 30

//Interpolation settings
#define INTERPOL_TICKS 20 //Keep this as low as possible, color differences lower than this will not be interpolated (diff/ticks < 1 will be rounded to 0).
#define INTERPOL_WAITTIME 1 //This is recommended to be about zero for everything but debugging purposes.

CRGB outputBuffer[PIXEL_AMOUNT];
CRGB newBuffer[PIXEL_AMOUNT];

//Function prototypes
void setup();
void loop();

/*
 * Interpolates the output value linearly.
 */
void interpolLin(){
  CRGB diffs[PIXEL_AMOUNT];
  for(unsigned int i = 0; i < PIXEL_AMOUNT; i++){
      diffs[i].b = (newBuffer[i].b - outputBuffer[i].b) / INTERPOL_TICKS;
      diffs[i].g = (newBuffer[i].g - outputBuffer[i].g) / INTERPOL_TICKS;
      diffs[i].r = (newBuffer[i].r - outputBuffer[i].r) / INTERPOL_TICKS;
  }
  
  for(unsigned int tick = 0; tick < INTERPOL_TICKS; tick++){
    for(unsigned int pixel = 0; pixel < PIXEL_AMOUNT; pixel++){
      outputBuffer[pixel].r += diffs[pixel].r;
      outputBuffer[pixel].g += diffs[pixel].g;
      outputBuffer[pixel].b += diffs[pixel].b;
    }
    FastLED.show();
    
    delay(INTERPOL_WAITTIME);
  }
}

void readBuffer(){
  for(unsigned int i = 0;i < PIXEL_AMOUNT; i++){ //DEBUG, should be PIXEL_AMOUNT instead of 1
    while(Serial.available() < 4); //Wait for the entire pixel to be read 

    //Read the data
    newBuffer[i].b = Serial.read();
    newBuffer[i].g = Serial.read();
    newBuffer[i].r = Serial.read();
    Serial.read(); //Throw away the fourth (reserved) byte

    //DEBUG    
    /*Serial.print("I received: ");
    Serial.print(newBuffer[i].r, DEC);
    Serial.print(",");
    Serial.print(newBuffer[i].g), DEC;
    Serial.print(",");
    Serial.println(newBuffer[i].b, DEC);*/
  }
}

void readBufferInt(){
  for(unsigned int i = 0;i < 1; i++){ //DEBUG, should be PIXEL_AMOUNT instead of 1
    while(Serial.available() < 10); //Wait for the entire pixel to be read 

    //Read the data
    newBuffer[i].r = (Serial.read() - 48) * 100;
    newBuffer[i].r += (Serial.read() - 48) * 10;
    newBuffer[i].r += Serial.read() - 48;
    
    newBuffer[i].g = (Serial.read() - 48) * 100;
    newBuffer[i].g += (Serial.read() - 48) * 10;
    newBuffer[i].g += Serial.read() - 48;

    newBuffer[i].b = (Serial.read() - 48) * 100;
    newBuffer[i].b += (Serial.read() - 48) * 10;
    newBuffer[i].b += Serial.read() - 48;
    Serial.read(); //Throw away the fourth (reserved) byte

    //DEBUG    
    /*Serial.print("I received: ");
    Serial.print(newBuffer[i].r, DEC);
    Serial.print(",");
    Serial.print(newBuffer[i].g), DEC;
    Serial.print(",");
    Serial.println(newBuffer[i].b, DEC);*/
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  readBuffer();
  //readBufferInt();
  interpolLin();
  FastLED.show();
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
  for(unsigned int i = 0; i < PIXEL_AMOUNT; i++){
    outputBuffer[i].r = outputBuffer[i].g = outputBuffer[i].b = 0;
  }
  FastLED.addLeds<NEOPIXEL, PIN_OUTPUT>(outputBuffer, PIXEL_AMOUNT);
}
