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

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=272,218
AudioOutputTDM                  tdm2;           //xy=514,291
AudioOutputSharedAD5754Dual     ad5754;           //xy=514,291
AudioInputSharedAD7606          ad7606;
AudioConnection          patchCord1(sine1, 0, ad5754, 0);
AudioConnection          patchCord2(sine1, 0, ad5754, 1);
AudioConnection          patchCord3(sine1, 0, ad5754, 2);
AudioConnection          patchCord4(sine1, 0, ad5754, 3);
AudioConnection          patchCord5(sine1, 0, ad5754, 4);
AudioConnection          patchCord6(sine1, 0, ad5754, 5);
AudioConnection          patchCord7(sine1, 0, ad5754, 6);
AudioConnection          patchCord8(sine1, 0, ad5754, 7);
// GUItool: end automatically generated code

void setup() {
    Serial.begin(9600);
    AudioMemory(60);
    pinMode(16, INPUT);
    pinMode(34, INPUT);
    pinMode(37, INPUT);

    ad5754.begin(AD7607_BUSY, AD7607_START_CONVERSION, AD7607_CHIP_SELECT, AD7607_RESET, AD7607_RANGE_SELECT, DA_SYNC, LRCLK_CPY);

    sine1.frequency(160);
    sine1.amplitude(1.0);
}

void loop() {

    Serial.print(".");
    delay(1000);
}
