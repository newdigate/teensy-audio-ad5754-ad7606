#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// https://github.com/newdigate/teensy-audio-ad5754-ad7606/tree/remove-timer
#include "input_shared_ad7606.h"
#include "output_shared_ad5754_dual.h"

// default configuration for teensy-eurorack v2
#define AD7607_BUSY 3
#define AD7607_START_CONVERSION 5
#define AD7607_CHIP_SELECT 36
#define AD7607_RESET 35 // teensy-control-voltage == 4
#define AD7607_RANGE_SELECT 37
#define DA_SYNC 38
#define LRCLK_CPY 40

//https://github.com/newdigate/teensy-audio-libtftscope/tree/st7735_t3
#include "XYScopeView.h"
#include <ST7735_t3.h> // Hardware-specific library

#define TFT_SCLK 13  // SCLK can also use pin 14
#define TFT_MOSI 11  // MOSI can also use pin 7
#define TFT_CS   6  // CS & DC can use pins 2, 6, 9, 10, 15, 20, 21, 22, 23
#define TFT_DC    2  //  but certain pairs must NOT be used: 2+10, 6+9, 20+23, 21+22
#define TFT_RST   -1 // RST can use any pin
//Adafruit_ST7735 TFT = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
ST7735_t3 TFT = ST7735_t3(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
// GUItool: begin automatically generated code

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

// GUItool: end automatically generated code
int16_t colors[8] = {ST7735_GREEN,ST7735_RED,ST7735_BLUE,ST7735_CYAN,ST7735_MAGENTA,ST7735_YELLOW,0xffAA,ST7735_WHITE};
XYScopeView scopeViewCV1 = XYScopeView(TFT, audioRecordQueue1, audioRecordQueue2, colors[0], (int16_t)ST7735_BLACK, 64, 64);
XYScopeView scopeViewCV2 = XYScopeView(TFT, audioRecordQueue3, audioRecordQueue4, colors[1], (int16_t)ST7735_BLACK, 64, 64);
XYScopeView scopeViewCV3 = XYScopeView(TFT, audioRecordQueue5, audioRecordQueue6, colors[2], (int16_t)ST7735_BLACK, 64, 64);
XYScopeView scopeViewCV4 = XYScopeView(TFT, audioRecordQueue7, audioRecordQueue8, colors[3], (int16_t)ST7735_BLACK, 64, 64);

const XYScopeView* cvscopes[] = {&scopeViewCV1, &scopeViewCV2, &scopeViewCV3, &scopeViewCV4};

#include <Bounce.h>
#define BUTTON 30
Bounce bouncer = Bounce( BUTTON,5 ); 


#include <Encoder.h>

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(28, 29);
//   avoid using pins with LEDs attached
long oldPosition  = -999;

#include <TimerOne.h>

float baseFrequency = 1.0; 

void setup() {
    Serial.begin(9600);
    AudioMemory(100);
    pinMode(41, INPUT);
    pinMode(BUTTON, INPUT_PULLUP);

    ad5754.begin(AD7607_BUSY, AD7607_START_CONVERSION, AD7607_CHIP_SELECT, AD7607_RESET, AD7607_RANGE_SELECT, DA_SYNC, LRCLK_CPY);

    setFrequencies();
    sine1.amplitude(0.8);
    sine2.amplitude(0.8);
    sine3.amplitude(0.8);
    sine4.amplitude(0.8);
    sine5.amplitude(0.8);
    sine6.amplitude(0.8); 
    sine7.amplitude(0.8); 
    sine8.amplitude(0.8); 

    

    TFT.initR(INITR_144GREENTAB);
    TFT.setRotation(3);
    TFT.fillScreen(ST7735_BLACK);
    TFT.useFrameBuffer(false);
    audioRecordQueue1.begin();
    audioRecordQueue2.begin();     
    audioRecordQueue3.begin();
    audioRecordQueue4.begin();
    audioRecordQueue5.begin();
    audioRecordQueue6.begin();
    audioRecordQueue7.begin();
    audioRecordQueue8.begin();

    //TFT.updateScreenAsync(false);
      Timer1.initialize(50000);
  Timer1.attachInterrupt(blinkLED); 
  
}
bool updateDisplay = true, toggleUpdateWhenDisplayReachesEnd = false;

void setFrequencies() {
  sine1.frequency(baseFrequency);
  sine2.frequency(baseFrequency*2.0);
  sine3.frequency(baseFrequency*4.0);
  sine4.frequency(baseFrequency*8.0);
  sine5.frequency(baseFrequency*16.0);
  sine6.frequency(baseFrequency*32.0);
  sine7.frequency(baseFrequency*64.0);
  sine8.frequency(baseFrequency*128.0);
}

void blinkLED(void)
{
  if(!TFT.asyncUpdateActive()){
    TFT.updateScreenAsync(false);
  }
}

void loop() {
  //delayMicroseconds(100);
  if ((cvscopes[0]->oscilliscope_x >= 127 && toggleUpdateWhenDisplayReachesEnd) 
      || (toggleUpdateWhenDisplayReachesEnd && !updateDisplay)) {
    updateDisplay = !updateDisplay;
    toggleUpdateWhenDisplayReachesEnd = false;
  }
  for (int i=0;i<4;i++) {
    cvscopes[i]->checkForUpdateBuffer();
  }

  if (updateDisplay) 
  {    
    for (int i=0;i<4;i++) 
      cvscopes[i]->drawScope();
  }
   
  while (Serial.available()) {
    char ch = Serial.read();
    //Serial.print(ch, DEC);
    if (ch == 10) 
      toggleUpdateWhenDisplayReachesEnd = true;
  }
  bouncer.update();

  // Get the update value
  int value = bouncer.read();
  if ( value == HIGH) {
    toggleUpdateWhenDisplayReachesEnd = true;
  }

  long newPosition = myEnc.read();
  if (newPosition/4 != oldPosition/4) {
    oldPosition = newPosition;
    baseFrequency = pow(2, newPosition/16.0);
    Serial.printf("%2d, %2f\n",newPosition/4, baseFrequency);
    setFrequencies();
  }
}
