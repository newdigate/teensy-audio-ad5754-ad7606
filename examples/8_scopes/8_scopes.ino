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
AudioSynthWaveformSine   sine2;          //xy=272,218
AudioSynthWaveformSine   sine3;          //xy=272,218
AudioSynthWaveformSine   sine4;          //xy=272,218
AudioSynthWaveformSine   sine5;          //xy=272,218
AudioSynthWaveformSine   sine6;          //xy=272,218
AudioSynthWaveformSine   sine7;          //xy=272,218
AudioSynthWaveformSine   sine8;          //xy=272,218
AudioOutputTDM                  tdm2;           //xy=514,291
AudioOutputSharedAD5754Dual     ad5754;           //xy=514,291
AudioInputSharedAD7606          ad7606;
AudioRecordQueue         audioRecordQueue1;
AudioRecordQueue         audioRecordQueue2;
AudioRecordQueue         audioRecordQueue3;
AudioRecordQueue         audioRecordQueue4;
AudioRecordQueue         audioRecordQueue5;
AudioRecordQueue         audioRecordQueue6;
AudioRecordQueue         audioRecordQueue7;
AudioRecordQueue         audioRecordQueue8;

AudioConnection          patchCord1(sine1, 0, ad5754, 0);
AudioConnection          patchCord2(sine2, 0, ad5754, 1);
AudioConnection          patchCord3(sine3, 0, ad5754, 2);
AudioConnection          patchCord4(sine4, 0, ad5754, 3);
AudioConnection          patchCord5(sine5, 0, ad5754, 4);
AudioConnection          patchCord6(sine6, 0, ad5754, 5);
AudioConnection          patchCord7(sine7, 0, ad5754, 6);
AudioConnection          patchCord8(sine8, 0, ad5754, 7);
AudioConnection          patchCord9(ad7606, 0, audioRecordQueue1, 0);
AudioConnection          patchCord10(ad7606, 1, audioRecordQueue2, 0);
AudioConnection          patchCord11(ad7606, 2, audioRecordQueue3, 0);
AudioConnection          patchCord12(ad7606, 3, audioRecordQueue4, 0);
AudioConnection          patchCord13(ad7606, 4, audioRecordQueue5, 0);
AudioConnection          patchCord14(ad7606, 5, audioRecordQueue6, 0);
AudioConnection          patchCord15(ad7606, 6, audioRecordQueue7, 0);
AudioConnection          patchCord16(ad7606, 7, audioRecordQueue8, 0);
int16_t colors[8] = {ST7735_GREEN,ST7735_RED,ST7735_BLUE,ST7735_CYAN,ST7735_MAGENTA,ST7735_YELLOW,ST7735_GREEN,ST7735_WHITE};

ScopeView scopeViewCV1 = ScopeView(TFT, audioRecordQueue1, colors[0], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewCV2 = ScopeView(TFT, audioRecordQueue2, colors[1], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewCV3 = ScopeView(TFT, audioRecordQueue3, colors[2], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewCV4 = ScopeView(TFT, audioRecordQueue4, colors[3], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewCV5 = ScopeView(TFT, audioRecordQueue5, colors[4], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewCV6 = ScopeView(TFT, audioRecordQueue6, colors[5], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewCV7 = ScopeView(TFT, audioRecordQueue7, colors[6], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewCV8 = ScopeView(TFT, audioRecordQueue8, colors[7], (int16_t)ST7735_BLACK, 64); 
const ScopeView* cvscopes[] = {&scopeViewCV1, &scopeViewCV2, &scopeViewCV3, &scopeViewCV4, 
&scopeViewCV5, &scopeViewCV6, &scopeViewCV7, &scopeViewCV8};

#include <Bounce.h>
#define BUTTON 30
Bounce bouncer = Bounce( BUTTON,5 ); 

void setup() {
  Serial.begin(9600);
  //while(!Serial) {
  //  delay(1);
  //}
  AudioMemory(80);
  //pinMode(41, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);
      
  TFT.initR(INITR_144GREENTAB);
  TFT.setRotation(3);
  TFT.fillScreen(ST7735_BLACK);

    sine1.frequency(62.5);
    sine1.amplitude(1.0);
    
    sine2.frequency(125);
    sine2.amplitude(1.0);
    
    sine3.frequency(250);
    sine3.amplitude(1.0);
    
    sine4.frequency(500);
    sine4.amplitude(1.0);
    
    sine5.frequency(750);
    sine5.amplitude(1.0);
    
    sine6.frequency(1000);
    sine6.amplitude(1.0);
    
    sine7.frequency(1500);
    sine7.amplitude(1.0);
    
    sine8.frequency(2000);
    sine8.amplitude(1.0);
    
    audioRecordQueue1.begin();
    audioRecordQueue2.begin();     
    audioRecordQueue3.begin();
    audioRecordQueue4.begin();
    audioRecordQueue5.begin();
    audioRecordQueue6.begin();
    audioRecordQueue7.begin();
    audioRecordQueue8.begin();
}
unsigned count = 0;
bool updateDisplay = true, toggleUpdateWhenDisplayReachesEnd = false;

void loop() {

  if ((cvscopes[0]->oscilliscope_x >= 127 && toggleUpdateWhenDisplayReachesEnd) 
      || (toggleUpdateWhenDisplayReachesEnd && !updateDisplay)) {
    updateDisplay = !updateDisplay;
    toggleUpdateWhenDisplayReachesEnd = false;
  }
  
  for (int i=0;i<8;i++) {
    cvscopes[i]->checkForUpdateBuffer();
  }
  if (updateDisplay) 
  {
    for (int i=0;i<8;i++) 
      cvscopes[i]->undrawScope();
      
    for (int i=0;i<8;i++) 
      cvscopes[i]->drawScope();
  }

  bouncer.update();

  // Get the update value
  int value = bouncer.read();
  if ( value == HIGH) {
    toggleUpdateWhenDisplayReachesEnd = true;
  }
  
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
