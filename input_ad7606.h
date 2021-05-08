//
// Created by Moolet on 18/03/2021.
//

#ifndef TEENSYAUDIOSPISHAREDLIBRARY_INPUT_SPI_H
#define TEENSYAUDIOSPISHAREDLIBRARY_INPUT_SPI_H


#include "Arduino.h"
#include "AudioStream.h"
#include "DMAChannel.h"

class AudioInputAD7606 : public AudioStream
{
public:
    AudioInputAD7606(void) : AudioStream(8, inputQueueArray) { begin(); }
    virtual void update(void);
    void begin(void);
protected:

private:
    audio_block_t *inputQueueArray[8];
};


#endif //TEENSYAUDIOSPISHAREDLIBRARY_OUTPUT_SPI_H
