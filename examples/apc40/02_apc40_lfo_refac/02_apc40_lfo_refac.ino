#include <Audio.h>
#include <ST7735_t3.h> // Hardware-specific library
#include <ResponsiveAnalogRead.h>
#include <USBHost_t36.h>

#include "teensy_eurorack.h"
#include "output_spi.h"
#include "ScopeView.h"
#include "akai_apc40_lfo.h"

ST7735_t3 TFT = ST7735_t3(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

USBHost myusb;
USBHub hub1(myusb);
MIDIDevice midi1(myusb);

// GUItool: begin automatically generated code
AudioInputTDM            tdm;            //xy=210.11108779907227,555.4443893432617
AudioOutputSPI           ad7606;         //xy=253.8888282775879,242.22220611572266
AudioRecordQueue         audioRecordQueue9; //xy=551.6665954589844,622.2221488952637
AudioRecordQueue         audioRecordQueue7; //xy=553.8888320922852,354.444393157959
AudioRecordQueue         audioRecordQueue10; //xy=553.888916015625,655.5554962158203
AudioRecordQueue         audioRecordQueue11; //xy=553.8889541625977,688.8888387680054
AudioRecordQueue         audioRecordQueue8; //xy=556.1110382080078,391.11108016967773
AudioRecordQueue         audioRecordQueue12; //xy=555.0000762939453,722.2222347259521
AudioRecordQueue         audioRecordQueue13; //xy=554.9999313354492,756.6666603088379
AudioRecordQueue         audioRecordQueue14; //xy=554.9999313354492,792.2221641540527
AudioRecordQueue         audioRecordQueue6; //xy=557.2220993041992,308.8888530731201
AudioRecordQueue         audioRecordQueue5; //xy=564.9999771118164,256.66662406921387
AudioRecordQueue         audioRecordQueue3; //xy=567.2221984863281,176.6666259765625
AudioRecordQueue         audioRecordQueue4; //xy=569.4443359375,217.77774238586426
AudioRecordQueue         audioRecordQueue1; //xy=570.5554885864258,97.77776336669922
AudioRecordQueue         audioRecordQueue2; //xy=572.7776794433594,134.4444284439087
AudioSynthWaveformSine   sine10;         //xy=751.1111102634006,491.11109627617725
AudioSynthWaveformSine   sine13;         //xy=753.3333324856228,603.3333184983995
AudioSynthWaveformSine   sine15;         //xy=753.3333625793457,683.3333053588867
AudioSynthWaveformSine   sine16;         //xy=753.3333587646484,722.2221755981445
AudioSynthWaveformSine   sine12;         //xy=754.4444435967339,559.9999851650662
AudioSynthWaveformSine   sine9;          //xy=757.7777769300673,453.33331849839954
AudioSynthWaveformSine   sine14;         //xy=757.7777862548828,644.4443759918213
AudioSynthWaveformSine   sine11;         //xy=758.8888880411783,525.5555407206217
AudioSynthWaveformSine   sine1;          //xy=766.9999732971191,97.77776336669922
AudioSynthWaveformSine   sine2;          //xy=766.999927520752,133.3333683013916
AudioSynthWaveformSine   sine8;          //xy=769.2221870422363,356.66664123535156
AudioSynthWaveformSine   sine3;          //xy=770.3332901000977,172.22220039367676
AudioSynthWaveformSine   sine5;          //xy=770.3332672119141,248.88892650604248
AudioSynthWaveformSine   sine6;          //xy=770.3332710266113,284.4444122314453
AudioSynthWaveformSine   sine7;          //xy=770.3332138061523,318.8888273239136
AudioSynthWaveformSine   sine4;          //xy=772.5555877685547,210.00000286102295
AudioSynthWaveformDc     dc1;            //xy=226,901
AudioSynthWaveformDc     dc2;            //xy=226,946
AudioSynthWaveformDc     dc3;            //xy=227,995
AudioSynthWaveformDc     dc4;            //xy=229,1039
AudioRecordQueue         queue4;         //xy=534,1040
AudioRecordQueue         queue1;         //xy=550,902
AudioRecordQueue         queue2;         //xy=551,945
AudioRecordQueue         queue3;         //xy=560,989

AudioOutputTDM           tdm2;           //xy=1037.8889694213867,542.1110763549805
AudioConnection          patchCord1(tdm, 0, audioRecordQueue9, 0);
AudioConnection          patchCord2(tdm, 2, audioRecordQueue10, 0);
AudioConnection          patchCord3(tdm, 4, audioRecordQueue11, 0);
AudioConnection          patchCord4(tdm, 6, audioRecordQueue12, 0);
AudioConnection          patchCord5(tdm, 8, audioRecordQueue13, 0);
AudioConnection          patchCord6(tdm, 10, audioRecordQueue14, 0);
AudioConnection          patchCord7(ad7606, 0, audioRecordQueue1, 0);
AudioConnection          patchCord8(ad7606, 1, audioRecordQueue2, 0);
AudioConnection          patchCord9(ad7606, 2, audioRecordQueue3, 0);
AudioConnection          patchCord10(ad7606, 3, audioRecordQueue4, 0);
AudioConnection          patchCord11(ad7606, 4, audioRecordQueue5, 0);
AudioConnection          patchCord12(ad7606, 5, audioRecordQueue6, 0);
AudioConnection          patchCord13(ad7606, 6, audioRecordQueue7, 0);
AudioConnection          patchCord14(ad7606, 7, audioRecordQueue8, 0);
AudioConnection          patchCord15(sine10, 0, tdm2, 2);
AudioConnection          patchCord16(sine13, 0, tdm2, 8);
AudioConnection          patchCord17(sine15, 0, tdm2, 12);
AudioConnection          patchCord18(sine16, 0, tdm2, 14);
AudioConnection          patchCord19(sine12, 0, tdm2, 6);
AudioConnection          patchCord20(sine9, 0, tdm2, 0);
AudioConnection          patchCord21(sine14, 0, tdm2, 10);
AudioConnection          patchCord22(sine11, 0, tdm2, 4);
AudioConnection          patchCord23(sine1, 0, ad7606, 0);
AudioConnection          patchCord24(sine2, 0, ad7606, 1);
AudioConnection          patchCord25(sine8, 0, ad7606, 7);
AudioConnection          patchCord26(sine3, 0, ad7606, 2);
AudioConnection          patchCord27(sine5, 0, ad7606, 4);
AudioConnection          patchCord28(sine6, 0, ad7606, 5);
AudioConnection          patchCord29(sine7, 0, ad7606, 6);
AudioConnection          patchCord30(sine4, 0, ad7606, 3);
AudioConnection          patchCord31(dc1, queue1);
AudioConnection          patchCord32(dc2, queue2);
AudioConnection          patchCord33(dc3, queue3);
AudioConnection          patchCord34(dc4, queue4);
AudioControlCS42448      cs42448_1;      //xy=553.4443435668945,831.111083984375
// GUItool: end automatically generated code

uint16_t colors[8] = {ST7735_GREEN,ST7735_RED,ST7735_BLUE,ST7735_CYAN,ST7735_MAGENTA,ST7735_YELLOW,0xFFAA,ST7735_WHITE};

ScopeView scopeViewCV1 = ScopeView(TFT, audioRecordQueue1, colors[0], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewCV2 = ScopeView(TFT, audioRecordQueue2, colors[1], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewCV3 = ScopeView(TFT, audioRecordQueue3, colors[2], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewCV4 = ScopeView(TFT, audioRecordQueue4, colors[3], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewCV5 = ScopeView(TFT, audioRecordQueue5, colors[4], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewCV6 = ScopeView(TFT, audioRecordQueue6, colors[5], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewCV7 = ScopeView(TFT, audioRecordQueue7, colors[6], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewCV8 = ScopeView(TFT, audioRecordQueue8, colors[7], (int16_t)ST7735_BLACK, 64); 

ScopeView scopeViewAudio1 = ScopeView(TFT, audioRecordQueue9, colors[0], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewAudio2 = ScopeView(TFT, audioRecordQueue10, colors[1], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewAudio3 = ScopeView(TFT, audioRecordQueue11, colors[2], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewAudio4 = ScopeView(TFT, audioRecordQueue12, colors[3], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewAudio5 = ScopeView(TFT, audioRecordQueue13, colors[4], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewAudio6 = ScopeView(TFT, audioRecordQueue14, colors[5], (int16_t)ST7735_BLACK, 64); 

ScopeView scopeViewPot1 = ScopeView(TFT, queue1, colors[2], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewPot2 = ScopeView(TFT, queue2, colors[3], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewPot3 = ScopeView(TFT, queue3, colors[4], (int16_t)ST7735_BLACK, 64); 
ScopeView scopeViewPot4 = ScopeView(TFT, queue4, colors[5], (int16_t)ST7735_BLACK, 64); 

ResponsiveAnalogRead analogPot1(TEENSY_EURORACK_PIN_POT1, true, 1.0);
ResponsiveAnalogRead analogPot2(TEENSY_EURORACK_PIN_POT2, true, 1.0);
ResponsiveAnalogRead analogPot3(TEENSY_EURORACK_PIN_POT3, true, 1.0);
ResponsiveAnalogRead analogPot4(TEENSY_EURORACK_PIN_POT4, true, 1.0);

const int num_sine_generators = 16;
AudioSynthWaveformSine* sine_generators[num_sine_generators] = {
  &sine1, &sine2, &sine3, &sine4, &sine5, &sine6, &sine7, &sine8, 
  &sine9, &sine10, &sine11, &sine12, &sine13, &sine14, &sine15, &sine16 };

const int num_scopes = 18;
ScopeView* scopeViews[num_scopes] = {
  &scopeViewCV1, &scopeViewCV2, &scopeViewCV3, &scopeViewCV4, &scopeViewCV5, &scopeViewCV6, &scopeViewCV7, &scopeViewCV8,
  &scopeViewAudio1, &scopeViewAudio2, &scopeViewAudio3, &scopeViewAudio4, &scopeViewAudio5, &scopeViewAudio6,
  &scopeViewPot1, &scopeViewPot2, &scopeViewPot3, &scopeViewPot4 };

AudioRecordQueue* recordQueues[num_scopes] = {
  &audioRecordQueue1,  &audioRecordQueue2,
  &audioRecordQueue3,  &audioRecordQueue4,
  &audioRecordQueue5,  &audioRecordQueue6,
  &audioRecordQueue7,  &audioRecordQueue8,
  &audioRecordQueue9,  &audioRecordQueue10,
  &audioRecordQueue11, &audioRecordQueue12,
  &audioRecordQueue13, &audioRecordQueue14,
  &queue1,             &queue2,
  &queue3,             &queue4 };

akai_apc40_lfo apc40_lfo(sine_generators);

unsigned long start_time;

byte freq_coarse[16] = {0};
byte freq_fine[16] = {0};
unsigned count = 0, count2 = 0;
bool shiftPressed = false;

double getFrequency(int ch) {
  return 1 * pow(2, freq_coarse[ch]/12.0) + 0.01 * pow(2, freq_fine[ch]/12.0); 
}

void setup() {
  pinMode(TEENSY_EURORACK_PIN_POT1, INPUT_DISABLE);
  pinMode(TEENSY_EURORACK_PIN_POT2, INPUT_DISABLE);
  pinMode(TEENSY_EURORACK_PIN_POT3, INPUT_DISABLE);
  pinMode(TEENSY_EURORACK_PIN_POT4, INPUT_DISABLE);

  analogReadResolution(12);
  analogPot1.enableEdgeSnap();
  analogPot1.setAnalogResolution(4096);
  analogPot2.enableEdgeSnap();
  analogPot2.setAnalogResolution(4096);
  analogPot3.enableEdgeSnap();
  analogPot3.setAnalogResolution(4096);
  analogPot4.enableEdgeSnap();
  analogPot4.setAnalogResolution(4096);

  Serial.begin(115200);
  //while(!Serial) {
  //  delay(1);
  //}

  AudioNoInterrupts();  
  
  AudioMemory(160);
  cs42448_1.enable();
  cs42448_1.volume(1);
  AudioInterrupts();    // enable, both tones will start together
  
  TFT.initR(INITR_144GREENTAB);
  TFT.setRotation(3);
  TFT.fillScreen(ST7735_BLACK);
  TFT.useFrameBuffer(true);
  TFT.updateScreenAsync(true);
 
  for (int i=0; i<num_sine_generators; i++) {
    sine_generators[i]->frequency(getFrequency(i));
    sine_generators[i]->amplitude(0);
  }

  for (int i=0; i<num_scopes; i++) {
    recordQueues[i]->begin();
  }

  dc1.amplitude(0.5, 1);
  dc1.amplitude(-0.5, 1);
  dc1.amplitude(0.25, 1);
  dc1.amplitude(-0.25, 1);

  myusb.begin();
  midi1.setHandleNoteOn(myNoteOn);
  midi1.setHandleNoteOff(myNoteOff);
  midi1.setHandleControlChange(myControlChange);

  apc40_lfo.sendNoteOn = sendNoteOn;
  
  start_time = millis();
}

char timestring[] = "                      ";

void updateTFTTime(char *oldString, char *newString, bool rerender) {
  int c=0;
  if (!rerender) {
    int len1 = strlen(oldString);
    int len2 = strlen(newString);
    while(oldString[c] == newString[c] && c < len1 && c < len2 ){
      c++;      
    }
  }
  TFT.setCursor(c * 6,0);
  TFT.setTextColor(ST7735_BLACK);
  TFT.print( (char * ) (oldString + c)); 
  
  TFT.setCursor(c * 6,0);
  TFT.setTextColor(ST7735_WHITE);
  TFT.print( (char * ) (newString + c));    
}


void loop() {
  myusb.Task();
  midi1.read();
    
  if (count % 200 == 0) {
    for (int i=0; i<num_scopes; i++){
      scopeViews[i]->checkForUpdateBuffer();
    }

    for (int i=0; i<num_scopes; i++){
      scopeViews[i]->undrawScope();
    }

    for (int i=0; i<num_scopes; i++){
      scopeViews[i]->drawScope();
    }
  }

  for (int i=0; i<num_scopes; i++) {
    if (recordQueues[i]->available() > 2) {
        recordQueues[i]->end();
        while (recordQueues[i]->available() > 2) {
          recordQueues[i]->readBuffer();
          recordQueues[i]->freeBuffer();
        }
        recordQueues[i]->begin();
    }
  }
      
  if (count % 20000 == 0) {
    analogPot1.update();  
    analogPot2.update();  
    analogPot3.update();  
    analogPot4.update();
    
    // if the responsive value has change, print out 'changed'
    if(analogPot1.hasChanged()) {
      float ff = (analogPot1.getValue()/2048.0) - 1.0f;
      dc1.amplitude(ff, 10);
    }

    if(analogPot2.hasChanged()) {
      float ff = (analogPot2.getValue()/2048.0) - 1.0f;
      dc2.amplitude(ff, 10);
    }
    
    if(analogPot3.hasChanged()) {
      float ff = (analogPot3.getValue()/2048.0) - 1.0f;
      dc3.amplitude(ff, 10);
    }
    
    if(analogPot4.hasChanged()) {
      float ff = (analogPot4.getValue()/2048.0) - 1.0f;
      dc4.amplitude(ff, 10);
    }
    
  }


  count++;
  if (count % 10000 == 0) {   
    unsigned long newmillis = millis() - start_time;
     
    count2++;
    unsigned long days = newmillis / (1000 * 60 * 60 * 24);
    unsigned long remainer = newmillis - (days * 1000 * 60 * 60 * 24);
    unsigned long hours = remainer / (1000 * 60 * 60);
    remainer = remainer - (hours * 1000 * 60 * 60);
    unsigned long minutes = remainer / (1000 * 60);
    remainer = remainer - (minutes * 1000 * 60);
    unsigned long seconds = remainer / 1000;
    remainer = remainer - (seconds * 1000);

    //tft.fillScreen(ST7735_BLACK);


    char newtimestring[] = "                      ";
    int n = sprintf(newtimestring, "%2u:%2u:%2u:%2u.%u", days, hours, minutes, seconds, remainer / 100);
    bool fullUpdate = count2 % 100 == 0;
    updateTFTTime(timestring, newtimestring, fullUpdate);
    memcpy(timestring, newtimestring, strlen(newtimestring) );
    
  }
}


void myNoteOn(byte channel, byte note, byte velocity) {
    // When a USB device with multiple virtual cables is used,
    // midi1.getCable() can be used to read which of the virtual
    // MIDI cables received this message.
    Serial.print("Note On, ch=");
    Serial.print(channel, DEC);
    Serial.print(", note=");
    Serial.print(note, DEC);
    Serial.print(", velocity=");
    Serial.println(velocity, DEC);
    apc40_lfo.noteOffReceived(channel, note, velocity);
}


void myNoteOff(byte channel, byte note, byte velocity) {
    // When a USB device with multiple virtual cables is used,
    // midi1.getCable() can be used to read which of the virtual
    // MIDI cables received this message.
    Serial.print("Note Off, ch=");
    Serial.print(channel, DEC);
    Serial.print(", note=");
    Serial.print(note, DEC);
    Serial.print(", velocity=");
    Serial.println(velocity, DEC);
    apc40_lfo.noteOffReceived(channel, note, velocity);
}

void myControlChange(byte channel, byte control, byte value) {

    Serial.print("Control Change, ch=");
    Serial.print(channel, DEC);
    Serial.print(", control=");
    Serial.print(control, DEC);
    Serial.print(", value=");
    Serial.println(value, DEC);
    
    apc40_lfo.controlChangeReceived(channel, control, value);
}


void sendNoteOn(byte note, byte velocity, byte channel) {

    midi1.sendNoteOn(note, velocity, channel);
}
