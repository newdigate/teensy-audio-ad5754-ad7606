#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "input_shared_ad7606.h"
#include "output_shared_ad5754_dual.h"

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

    sine1.frequency(160);
    sine1.amplitude(1.0);
}

void loop() {

    Serial.print(".");
    delay(1000);
}
