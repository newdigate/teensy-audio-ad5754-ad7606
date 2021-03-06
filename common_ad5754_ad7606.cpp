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
#include "common_ad5754_ad7606.h"

bool ad5754_ad7606_shared_context::_initialized_shared_context = false;
volatile bool ad5754_ad7606_shared_context::alreadyReset = false;
volatile bool ad5754_ad7606_shared_context::_isBusy = false;
volatile u_long ad5754_ad7606_shared_context::_txCompletedMicros = 0;
volatile unsigned int ad5754_ad7606_shared_context::read_index = 0;

DMAChannel ad5754_ad7606_shared_context::dmatx(false);

DMAChannel ad5754_ad7606_shared_context::dmarx(false);

DMAMEM volatile uint8_t ad5754_ad7606_shared_context::txbuf[32] = {};

int ad5754_ad7606_shared_context::txvoltages[8] = {0,0,0,0,0,0,0,0};

DMAMEM volatile int8_t ad5754_ad7606_shared_context::rxbuf[32];

//IntervalTimer ad5754_ad7606_shared_context::_timer = IntervalTimer();

void (*ad5754_ad7606_shared_context::fn_consumeIncommingSamples)(volatile int8_t *, unsigned int) = nullptr;
void (*ad5754_ad7606_shared_context::fn_setOutgoingSamples)(volatile int[], unsigned int) = nullptr;