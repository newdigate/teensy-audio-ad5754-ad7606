#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "input_shared_ad7606.h"
#include "output_shared_ad5754_dual.h"

#include "ScopeView.h"
#include <ST7735_t3.h> // Hardware-specific library
#include <SPI.h>
  
#define TFT_SCLK 13  // SCLK can also use pin 14
#define TFT_MOSI 11  // MOSI can also use pin 7
#define TFT_CS   6  // CS & DC can use pins 2, 6, 9, 10, 15, 20, 21, 22, 23
#define TFT_DC    2  //  but certain pairs must NOT be used: 2+10, 6+9, 20+23, 21+22
#define TFT_RST   -1 // RST can use any pin
//Adafruit_ST7735 TFT = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
ST7735_t3 TFT = ST7735_t3(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
// GUItool: begin automatically generated code
AudioOutputTDM                  tdm2;           //xy=514,291
AudioOutputSharedAD5754Dual     ad5754;           //xy=514,291
AudioInputSharedAD7606          ad7606;
AudioSynthWaveformSine   sine1;          //xy=272,218
AudioRecordQueue         audioRecordQueue1; 
AudioRecordQueue         audioRecordQueue2; 
AudioRecordQueue         audioRecordQueue3; 
AudioRecordQueue         audioRecordQueue4; 
AudioRecordQueue         audioRecordQueue5; 
AudioRecordQueue         audioRecordQueue6; 
AudioRecordQueue         audioRecordQueue7; 
AudioRecordQueue         audioRecordQueue8;
 
AudioConnection          patchCord1(sine1, 0, ad5754, 0);
AudioConnection          patchCord2(sine1, 0, ad5754, 1);
AudioConnection          patchCord3(sine1, 0, ad5754, 2);
AudioConnection          patchCord4(sine1, 0, ad5754, 3);
AudioConnection          patchCord5(sine1, 0, ad5754, 4);
AudioConnection          patchCord6(sine1, 0, ad5754, 5);
AudioConnection          patchCord7(sine1, 0, ad5754, 6);
AudioConnection          patchCord8(sine1, 0, ad5754, 7);

AudioConnection          patchCord9(ad7606, 0, audioRecordQueue1, 0);
AudioConnection          patchCord10(ad7606, 1, audioRecordQueue2, 0);
AudioConnection          patchCord11(ad7606, 2, audioRecordQueue3, 0);
AudioConnection          patchCord12(ad7606, 3, audioRecordQueue4, 0);
AudioConnection          patchCord13(ad7606, 4, audioRecordQueue5, 0);
AudioConnection          patchCord14(ad7606, 5, audioRecordQueue6, 0);
AudioConnection          patchCord15(ad7606, 6, audioRecordQueue7, 0);
AudioConnection          patchCord16(ad7606, 7, audioRecordQueue8, 0);

// GUItool: end automatically generated code

int16_t colors[8] = {ST7735_GREEN,ST7735_RED,ST7735_BLUE,ST7735_CYAN,ST7735_MAGENTA,ST7735_YELLOW,0xFFAA,ST7735_WHITE};

ScopeView scopeViewCV1 = ScopeView(TFT, audioRecordQueue1, colors[0], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewCV2 = ScopeView(TFT, audioRecordQueue2, colors[1], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewCV3 = ScopeView(TFT, audioRecordQueue3, colors[2], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewCV4 = ScopeView(TFT, audioRecordQueue4, colors[3], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewCV5 = ScopeView(TFT, audioRecordQueue5, colors[4], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewCV6 = ScopeView(TFT, audioRecordQueue6, colors[5], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewCV7 = ScopeView(TFT, audioRecordQueue7, colors[6], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewCV8 = ScopeView(TFT, audioRecordQueue8, colors[7], (int16_t)ST7735_BLACK, 64); 

void setup() {
  Serial.begin(9600);
  //while(!Serial) {
  //  delay(1);
  //}
  AudioMemory(580);
  //pinMode(41, INPUT);
  
  TFT.initR(INITR_144GREENTAB);
  TFT.setRotation(3);
  TFT.fillScreen(ST7735_BLACK);
  noInterrupts();
  sine1.frequency(1000);
  sine1.amplitude(0.8);
  audioRecordQueue1.begin();
  audioRecordQueue2.begin(); 
  audioRecordQueue3.begin();
  audioRecordQueue4.begin();
  audioRecordQueue5.begin();
  audioRecordQueue6.begin();
  audioRecordQueue7.begin();
  audioRecordQueue8.begin(); 
  interrupts();
}
unsigned count = 0;
void loop() {
  if (count % 1000 == 0) {
    scopeViewCV1.checkForUpdateBuffer();
    scopeViewCV2.checkForUpdateBuffer();
    scopeViewCV3.checkForUpdateBuffer();
    scopeViewCV4.checkForUpdateBuffer();
    scopeViewCV5.checkForUpdateBuffer();
    scopeViewCV6.checkForUpdateBuffer();
    scopeViewCV7.checkForUpdateBuffer();
    scopeViewCV8.checkForUpdateBuffer();
    
    scopeViewCV1.undrawScope();
    scopeViewCV2.undrawScope();
    scopeViewCV3.undrawScope();
    scopeViewCV4.undrawScope();
    scopeViewCV5.undrawScope();
    scopeViewCV6.undrawScope();
    scopeViewCV7.undrawScope();
    scopeViewCV8.undrawScope();
    
    scopeViewCV1.drawScope();
    scopeViewCV2.drawScope();
    scopeViewCV3.drawScope();
    scopeViewCV4.drawScope();
    scopeViewCV5.drawScope();
    scopeViewCV6.drawScope();
    scopeViewCV7.drawScope();    
    scopeViewCV8.drawScope();  
  }
    
  while (audioRecordQueue1.available() > 2) {
      audioRecordQueue1.readBuffer();
      audioRecordQueue1.freeBuffer();
  }

    
  while (audioRecordQueue2.available() > 2) {
      audioRecordQueue2.readBuffer();
      audioRecordQueue2.freeBuffer();
  }

      
  while (audioRecordQueue3.available() > 2) {
      audioRecordQueue3.readBuffer();
      audioRecordQueue3.freeBuffer();
  }

      
  while (audioRecordQueue4.available() > 2) {
      audioRecordQueue4.readBuffer();
      audioRecordQueue4.freeBuffer();
  }

      
  while (audioRecordQueue5.available() > 2) {
      audioRecordQueue5.readBuffer();
      audioRecordQueue5.freeBuffer();
  }

      
  while (audioRecordQueue6.available() > 2) {
      audioRecordQueue6.readBuffer();
      audioRecordQueue6.freeBuffer();
  }

      
  while (audioRecordQueue7.available() > 2) {
      audioRecordQueue7.readBuffer();
      audioRecordQueue7.freeBuffer();
  }

  while (audioRecordQueue8.available() > 2) {
      audioRecordQueue8.readBuffer();
      audioRecordQueue8.freeBuffer();
  }
    
  //delayMicroseconds(100);
  count++;
  if (count % 100000 == 0) {
    Serial.print("all=");
    Serial.print(AudioProcessorUsage());
    Serial.print(",");
    Serial.print(AudioProcessorUsageMax());
    Serial.print("    ");
    Serial.print("Memory: ");
    Serial.print(AudioMemoryUsage());
    Serial.print(",");
    Serial.print(AudioMemoryUsageMax());
    Serial.println();
  }
}
