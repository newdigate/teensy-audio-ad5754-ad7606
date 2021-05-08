//
// Created by Moolet on 18/03/2021.
//

#include <Arduino.h>
#include "SPI.h"
#include "input_ad7606.h"
#if !defined(KINETISL)

#include "memcpy_audio.h"
#include "utility/imxrt_hw.h"
#include "output_ad5754.h"

void AudioInputAD7606::begin(void)
{
}


void AudioInputAD7606::update(void)
{
    unsigned int i;
    audio_block_t *out_block[8];

    __disable_irq();
    memcpy(out_block, AudioOutputAD5754::block_incoming, sizeof(out_block));
    __enable_irq();

    if (out_block[0] != nullptr) {
        // if we got 1 block, all 16 are filled
        for (i = 0; i < 8; i++) {
            transmit(out_block[i], i);
            release(out_block[i]);
        }
    }
    digitalWriteFast(0, LOW);
    digitalWriteFast(0, HIGH);
    digitalWriteFast(0, LOW);
    digitalWriteFast(0, HIGH);
    digitalWriteFast(0, LOW);
    digitalWriteFast(0, HIGH);
    //
/*
    for (i=0; i < 8; i++) {
        if (prev[i]) release(prev[i]);
    }
*/

}

#endif