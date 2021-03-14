/* This file is based on input_tdm from
 *   https://github.com/PaulStoffregen/Audio
 *
 * Audio Library for Teensy 3.X
 * Copyright (c) 2017, Paul Stoffregen, paul@pjrc.com
 *
 * Development of this audio library was funded by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
 * open source software by purchasing Teensy or other PJRC products.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <Arduino.h>
#include "input_shared_ad7606.h"
#include "memcpy_audio.h"
#include "utility/imxrt_hw.h"
#include <SPI.h>
#include <imxrt.h>
#include <cstdint>
#include <DMAChannel.h>
#include "common_ad5754_ad7606.h"

audio_block_t * AudioInputSharedAD7606::block_incoming[8] = {
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
};
bool AudioInputSharedAD7606::update_responsibility = false;

void AudioInputSharedAD7606::begin(void) {
    ad5754_ad7606_shared_context::initialize();
    ad5754_ad7606_shared_context::fn_consumeIncommingSamples = consumeIncommingSamples;
}

void AudioInputSharedAD7606::update(void) {
    unsigned int i, j;
    audio_block_t *new_block[8];
    audio_block_t *out_block[8];

    // allocate 8 new blocks.  If any fails, allocate none
    for (i = 0; i < 8; i++) {
        new_block[i] = allocate();
        if (new_block[i] == nullptr) {
            for (j = 0; j < i; j++) {
                release(new_block[j]);
            }
            memset(new_block, 0, sizeof(new_block));
            break;
        }
    }
    __disable_irq();
    memcpy(out_block, block_incoming, sizeof(out_block));
    memcpy(block_incoming, new_block, sizeof(block_incoming));
    __enable_irq();

    if (out_block[0] != nullptr) {
        // if we got 1 block, all 16 are filled
        for (i = 0; i < 8; i++) {
            transmit(out_block[i], i);
            release(out_block[i]);
        }
    }
    ad5754_ad7606_shared_context::resetBuffers();
}

void AudioInputSharedAD7606::consumeIncommingSamples(const volatile int8_t *rxbuf, volatile unsigned int index) {
    for (int i = 0; i < 8; i++) {
        if (block_incoming[i] != NULL) {
            block_incoming[i]->data[index] = (rxbuf[i * 2] << 8) + rxbuf[(i * 2) + 1];
        }
        //Serial.printf("%x, %x - ", rxbuf[i * 2], rxbuf[(i * 2)+1]);
    }
    //Serial.println("v");
}
