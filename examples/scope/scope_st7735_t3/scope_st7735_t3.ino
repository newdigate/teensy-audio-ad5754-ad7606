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
AudioSynthWaveformSine   sine1;          //xy=272,218
AudioOutputTDM                  tdm2;           //xy=514,291
AudioOutputSharedAD5754Dual     ad5754;           //xy=514,291
AudioInputSharedAD7606          ad7606;
AudioRecordQueue         audioRecordQueue1; 
AudioConnection          patchCord1(sine1, 0, ad5754, 0);
AudioConnection          patchCord2(sine1, 0, ad5754, 1);
AudioConnection          patchCord3(sine1, 0, ad5754, 2);
AudioConnection          patchCord4(sine1, 0, ad5754, 3);
AudioConnection          patchCord5(sine1, 0, ad5754, 4);
AudioConnection          patchCord6(sine1, 0, ad5754, 5);
AudioConnection          patchCord7(sine1, 0, ad5754, 6);
AudioConnection          patchCord8(sine1, 0, ad5754, 7);

AudioConnection          patchCord9(ad7606, 0, audioRecordQueue1, 0);
// GUItool: end automatically generated code

int16_t colors[8] = {ST7735_GREEN,ST7735_RED,ST7735_BLUE,ST7735_CYAN,ST7735_MAGENTA,ST7735_YELLOW,0xFFAA,ST7735_WHITE};

ScopeView scopeViewCV1 = ScopeView(TFT, audioRecordQueue1, colors[1], (int16_t)ST7735_BLACK, 64); 

void setup() {
  Serial.begin(9600);
  //while(!Serial) {
  //  delay(1);
  //}
  AudioMemory(80);
  pinMode(41, INPUT);
  
  TFT.initR(INITR_144GREENTAB);
  TFT.setRotation(3);
  TFT.fillScreen(ST7735_BLACK);
  
  sine1.frequency(0.1);
  sine1.amplitude(1.0);
  audioRecordQueue1.begin();
}
unsigned count = 0;
void loop() {
  scopeViewCV1.checkForUpdateBuffer();
  scopeViewCV1.undrawScope();
  scopeViewCV1.drawScope();

          while (audioRecordQueue1.available() > 2) {
            audioRecordQueue1.readBuffer();
            audioRecordQueue1.freeBuffer();
        }
  delayMicroseconds(100);
  count++;
  if (count % 1000 == 0) {
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
