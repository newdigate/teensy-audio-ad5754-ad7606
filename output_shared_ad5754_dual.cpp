/* This file is based on output_tdm from
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
#include "output_shared_ad5754_dual.h"
#include "memcpy_audio.h"
#include "utility/imxrt_hw.h"
#include <SPI.h>
#include <imxrt.h>
#include <cstdint>
#include <DMAChannel.h>

#include "common_ad5754_ad7606.h"
#include "input_shared_ad7606.h"

audio_block_t * AudioOutputSharedAD5754Dual::block_input[8] = {
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};
bool AudioOutputSharedAD5754Dual::update_responsibility = false;

void AudioOutputSharedAD5754Dual::begin(void)
{
    ad5754_ad7606_shared_context::initialize();

    pinMode(0, OUTPUT);
	digitalWrite(0, HIGH);

	for (int i=0; i < 8; i++) {
		block_input[i] = NULL;
	}
}

void AudioOutputSharedAD5754Dual::update(void)
{
    digitalWrite(0, LOW);

	audio_block_t *prev[8];
	unsigned int i;

	__disable_irq();
	for (i=0; i < 8; i++) {
		prev[i] = block_input[i];
		block_input[i] = receiveReadOnly(i);
	}
    ad5754_ad7606_shared_context::resetBuffers();

	__enable_irq();

	for (i=0; i < 8; i++) {
		if (prev[i]) release(prev[i]);
	}
    ad5754_ad7606_shared_context::beginTransfer();
    digitalWrite(0, HIGH);
}

void AudioOutputSharedAD5754Dual::setOutputVoltages(int *voltages, unsigned int read_index) {
    const uint32_t zero_level = 0xFFFF / 2;
    voltages[0] = (block_input[0] != NULL) ? block_input[0]->data[read_index] + zero_level : zero_level;
    voltages[1] = (block_input[1] != NULL) ? block_input[1]->data[read_index] + zero_level : zero_level;
    voltages[2] = (block_input[2] != NULL) ? block_input[2]->data[read_index] + zero_level : zero_level;
    voltages[3] = (block_input[3] != NULL) ? block_input[3]->data[read_index] + zero_level : zero_level;
    voltages[4] = (block_input[4] != NULL) ? block_input[4]->data[read_index] + zero_level : zero_level;
    voltages[5] = (block_input[5] != NULL) ? block_input[5]->data[read_index] + zero_level : zero_level;
    voltages[6] = (block_input[6] != NULL) ? block_input[6]->data[read_index] + zero_level : zero_level;
    voltages[7] = (block_input[7] != NULL) ? block_input[7]->data[read_index] + zero_level : zero_level;
}
