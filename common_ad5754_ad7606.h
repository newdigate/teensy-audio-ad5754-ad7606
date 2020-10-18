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
#ifndef common_ad5754_ad7606_h_
#define common_ad5754_ad7606_h_

#define AD7607_BUSY 3
#define AD7607_START_CONVERSION 5
#define AD7607_CHIP_SELECT 36
#define AD7607_RESET 4
#define DA_SYNC 16

#include "output_shared_ad5754_dual.h"
#include "SPI.h"

class ad5754_ad7606_shared_context {

public:
    ad5754_ad7606_shared_context() {

    }

    static void initialize() {
        if (_initialized_shared_context) return;
        // input pins
        pinMode(AD7607_BUSY, INPUT_PULLUP);

        // output pins
        pinMode(AD7607_START_CONVERSION, OUTPUT);
        pinMode(AD7607_CHIP_SELECT, OUTPUT);
        pinMode(AD7607_RESET, OUTPUT);

        digitalWrite(AD7607_START_CONVERSION, HIGH);
        digitalWrite(AD7607_RESET, LOW);
        digitalWrite(AD7607_CHIP_SELECT, HIGH);

        attachInterrupt(AD7607_BUSY, busyFallingEdgeISR, FALLING);

        pinMode(DA_SYNC, OUTPUT);
        digitalWrite(DA_SYNC, HIGH);

        SPI1.begin();
        config_dma();
        _initialized_shared_context = true;
    }

    static void resetBuffers(){
        read_index = 0;
        commandsTransmitted = 0;
    }

    static void beginTransfer()
    {
        if (commandsTransmitted == 0) {
            if (read_index >= 128){
                return;
            }
            AudioOutputSharedAD5754Dual::setOutputVoltages(txvoltages, read_index);

            // first 3 bytes -> DAC0
            txbuf[0] = 0x00;              // channel == 0
            txbuf[1] = txvoltages[0] >> 8;
            txbuf[2] = txvoltages[0] & 0xff;

            // second 3 bytes -> DAC1
            txbuf[3] = 0x00;              // channel == 0
            txbuf[4] = txvoltages[4] >> 8;
            txbuf[5] = txvoltages[4] & 0xff;
        }

        if (commandsTransmitted < 4) {
            digitalWrite(DA_SYNC, LOW);
            IMXRT_LPSPI3_S.TCR = (IMXRT_LPSPI3_S.TCR & ~(LPSPI_TCR_FRAMESZ(31))) | LPSPI_TCR_FRAMESZ(7); // Transmit Control Register: ?
            IMXRT_LPSPI3_S.FCR = 0; // FIFO control register

            IMXRT_LPSPI3_S.DER = LPSPI_DER_RDDE | LPSPI_DER_TDDE;//DMA Enable register: enable DMA on RX
            IMXRT_LPSPI3_S.SR = 0x3f00; // StatusRegister: clear out all of the other status...
            _timer.begin(timer, (1000000.0/44100.0)-13.055);
            SPI1.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
            dmarx.enable();
            dmatx.enable();
        }
    }

    static void (*fn_consumeIncommingSamples)(int8_t *, unsigned int);
protected:
    static void prepareForDMA_RX() {
        if (read_index < 128) {
            digitalWrite(AD7607_CHIP_SELECT, LOW);
        }
    }

    // ADC signals conversion complete - start sampling process: read 8 16bit samples from adc and write 8 16bit samples to dac...
    static void busyFallingEdgeISR() {
        if (read_index < 128) {

            digitalWrite(AD7607_CHIP_SELECT, LOW);

            IMXRT_LPSPI3_S.TCR = (IMXRT_LPSPI3_S.TCR & ~(LPSPI_TCR_FRAMESZ(31))) | LPSPI_TCR_FRAMESZ(7); // Transmit Control Register: ?
            IMXRT_LPSPI3_S.FCR = 0; // FIFO control register

            IMXRT_LPSPI3_S.DER = LPSPI_DER_RDDE | LPSPI_DER_TDDE;//DMA Enable register: enable DMA on RX
            IMXRT_LPSPI3_S.SR = 0x3f00; // StatusRegister: clear out all of the other status...

            prepareForDMA_RX();
            beginTransfer();
        };
    }

    static void toggleStartConversion(){
        if (read_index < 127) {
            digitalWrite(AD7607_START_CONVERSION, LOW);
            digitalWrite(AD7607_START_CONVERSION, HIGH);
        }
    }

    static void config_dma(void)
    {
        dmatx.begin(true); // allocate the DMA channel first
        dmatx.TCD->SADDR = txbuf;
        dmatx.TCD->SOFF = 1;
        dmatx.TCD->ATTR = DMA_TCD_ATTR_SSIZE(0) | DMA_TCD_ATTR_DSIZE(0);
        dmatx.TCD->NBYTES_MLNO = 1;
        dmatx.TCD->SLAST = -sizeof(txbuf);
        dmatx.TCD->DOFF = 0;
        dmatx.TCD->CITER_ELINKNO = sizeof(txbuf);
        dmatx.TCD->DLASTSGA = 0;
        dmatx.TCD->BITER_ELINKNO = sizeof(txbuf);
        dmatx.TCD->CSR = DMA_TCD_CSR_INTMAJOR;
        dmatx.TCD->DADDR = (void *)((uint32_t)&(IMXRT_LPSPI3_S.TDR));
        dmatx.triggerAtHardwareEvent(DMAMUX_SOURCE_LPSPI3_TX);
        dmatx.disableOnCompletion();
        dmatx.attachInterrupt(txisr);
        dmatx.interruptAtCompletion();

        dmarx.begin(true); // allocate the DMA channel first
        dmarx.destinationBuffer(rxbuf, 16);
        dmarx.transferCount(16);
        dmarx.transferSize(1);
        dmarx.source((volatile uint8_t &)IMXRT_LPSPI3_S.RDR);
        dmarx.interruptAtCompletion();
        dmarx.triggerAtHardwareEvent(DMAMUX_SOURCE_LPSPI3_RX);
        dmarx.disableOnCompletion();
        dmarx.attachInterrupt(rxisr);
    }

    static void txisr(void) {
        commandsTransmitted++;
        unsigned int tx = commandsTransmitted;
        dmatx.clearInterrupt();
        SPI1.endTransaction();

        IMXRT_LPSPI3_S.FCR = LPSPI_FCR_TXWATER(15); // FIFO control register: set transmit watermark
        IMXRT_LPSPI3_S.DER = 0;                     // DMA enable register: disable DMA TX
        IMXRT_LPSPI3_S.CR = LPSPI_CR_MEN | LPSPI_CR_RRF | LPSPI_CR_RTF; // Control register... ?
        IMXRT_LPSPI3_S.SR = 0x3f00;                 // Status register: clear out all of the other status...

        while (IMXRT_LPSPI3_S.FSR & 0x1f);          //FIFO Status Register? wait until FIFO is empty before continuing...
        while (IMXRT_LPSPI3_S.SR & LPSPI_SR_MBF) ;  //Status Register? Module Busy flag, wait until SPI is not busy...

        digitalWrite(DA_SYNC, HIGH);

        if (tx < 4) {
            txbuf[0] = commandsTransmitted;                   //DAC0, channel=count
            txbuf[1] = txvoltages[commandsTransmitted] >> 8;
            txbuf[2] = txvoltages[commandsTransmitted] & 0xff;
            txbuf[3] = commandsTransmitted;                   //DAC1, channel=count
            txbuf[4] = txvoltages[commandsTransmitted+4] >> 8;
            txbuf[5] = txvoltages[commandsTransmitted+4] & 0xff;

            beginTransfer();
        } else {
            commandsTransmitted = 0;
            read_index++;
        }
    }

    static void rxisr(void) {
        dmarx.clearInterrupt();
        dmarx.disable();

        digitalWrite(AD7607_CHIP_SELECT, HIGH);

        if (read_index < 128 && fn_consumeIncommingSamples != nullptr) {
            fn_consumeIncommingSamples(rxbuf, read_index);
        };
    }

    static void timer(void){
        _timer.end();
        toggleStartConversion();
    }

    static bool _initialized_shared_context;
    static unsigned int commandsTransmitted;
    static unsigned int read_index;

    static DMAChannel dmarx;
    static DMAChannel dmatx;

    static volatile uint8_t txbuf[6];
    static int txvoltages[8];

    static int8_t rxbuf[32];

    static IntervalTimer _timer;

};

#endif // common_ad5754_ad7606_h_