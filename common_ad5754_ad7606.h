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
#define DA_SYNC 38

/* AD5754R Register Map */
#define AD5754R_REG_DAC             0x00 // DAC register
#define AD5754R_REG_RANGE_SELECT    0x01 // Output range select register
#define AD5754R_REG_POWER_CONTROL   0x02 // Power control register
#define AD5754R_REG_CONTROL         0x03 // Control register

/* AD5754R Channel Address */
#define AD5754R_DAC_A               0x00 // Address of channel A
#define AD5754R_DAC_B               0x01 // Address of channel B
#define AD5754R_DAC_C               0x02 // Address of channel C
#define AD5754R_DAC_D               0x03 // Address of channel D
#define AD5754R_DAC_ALL             0x04 // All four DACs


/* AD5754R Range Bits */
#define AD5754R_UNIPOLAR_5_RANGE    0x00 // 0..+5(V)
#define AD5754R_UNIPOLAR_10_RANGE   0x01 // 0..+10(V)
#define AD5754R_UNIPOLAR_10_8_RANGE 0x02 // 0..+10.8(V)
#define AD5754R_BIPOLAR_5_RANGE     0x03 // -5..+5(V)
#define AD5754R_BIPOLAR_10_RANGE    0x04 // -10...+10(V)
#define AD5754R_BIPOLAR_10_8_RANGE  0x05 // -10.8...+10.8(V)

#include <SPI.h>
#include "output_shared_ad5754_dual.h"
#include "SPI.h"

#define MOSI_PIN 26
#define MISO_PIN 39
#define SCK_PIN 27

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
        pinMode(DA_SYNC, OUTPUT);

        digitalWrite(AD7607_START_CONVERSION, HIGH);
        digitalWrite(AD7607_RESET, LOW);
        digitalWrite(AD7607_CHIP_SELECT, HIGH);
        //attachInterrupt(AD7607_BUSY, busyFallingEdgeISR, FALLING);
        digitalWrite(DA_SYNC, HIGH);

        SPI1.setSCK(SCK_PIN);
        SPI1.setCS(DA_SYNC);
        SPI1.setMOSI(MOSI_PIN);
        SPI1.setMISO(MISO_PIN);
        SPI1.begin();

        SPI1.beginTransaction(SPISettings());
        digitalWrite(DA_SYNC, LOW);
        uint8_t configureDac[] = {
                0x10,
                0x00,
                0x0f,
                0x10,
                0x00,
                0x0f
        };
        SPI1.transfer(configureDac, 6);
        SPI1.endTransaction();
        digitalWrite(DA_SYNC, HIGH);
        delayMicroseconds(10);


        // Set voltage range for DAC0, DAC1
        digitalWrite(DA_SYNC, LOW);
        uint8_t configureDacVoltageRange[] = {
                (AD5754R_REG_RANGE_SELECT << 3) + AD5754R_DAC_ALL,
                0x00,
                AD5754R_BIPOLAR_10_RANGE,
                (AD5754R_REG_RANGE_SELECT << 3) + AD5754R_DAC_ALL,
                0x00,
                AD5754R_BIPOLAR_10_RANGE
        };
        SPI1.beginTransaction(SPISettings());
        SPI1.transfer(configureDacVoltageRange, 6);
        SPI1.endTransaction();
        digitalWrite(DA_SYNC, HIGH);
        delayMicroseconds(10);

        setClockDivider_noInline(30000000);         // 30 MHz
        config_dma();
        _initialized_shared_context = true;
    }

    static void resetBuffers(){
        if(!alreadyReset) {
            alreadyReset = true;
            _timer.end();
            _timer.begin(timer, (1000000.0 / 44100.0) - 0.4);
            read_index = 0;
            toggleStartConversion();
            beginTransfer();
        }
    }

    static void beginTransfer()
    {
        if (read_index < 128 && fn_setOutgoingSamples != nullptr) {
            fn_setOutgoingSamples(txvoltages, read_index);
        };


        for (uint8_t count=0; count<4; count++) {

            txbuf[(count*8)+3] = count;                   //DAC0, channel=count
            txbuf[(count*8)+5] = txvoltages[count+4] >> 8;
            txbuf[(count*8)+4] = txvoltages[count+4] & 0xff;
            txbuf[(count*8)+0] = count;                //DAC1, channel=count
            txbuf[(count*8)+2] = txvoltages[count] >> 8;
            txbuf[(count*8)+1] = txvoltages[count] & 0xff;
        }
        while (IMXRT_LPSPI3_S.FSR & 0x1f);
        while (IMXRT_LPSPI3_S.SR & LPSPI_SR_MBF) ;  //Status Register? Module Busy flag
        SPI1.setCS(DA_SYNC);

        IMXRT_LPSPI3_S.TCR = (IMXRT_LPSPI3_S.TCR & ~(LPSPI_TCR_FRAMESZ(15))) | LPSPI_TCR_FRAMESZ(47) ;
        //IMXRT_LPSPI3_S.TCR = (IMXRT_LPSPI3_S.TCR & ~(LPSPI_TCR_TXMSK) | LPSPI_TCR_RXMSK);
        IMXRT_LPSPI3_S.FCR = 0;
        IMXRT_LPSPI3_S.DER = LPSPI_DER_TDDE;// | LPSPI_DER_RDDE; //enable DMA on both TX and RX
        IMXRT_LPSPI3_S.SR = 0x3f00; // clear out all of the other status...
        dmatx.enable();
    }

    static void beginReceive() {
        while (IMXRT_LPSPI3_S.FSR & 0x1f);
        while (IMXRT_LPSPI3_S.SR & LPSPI_SR_MBF) ;  //Status Register? Module Busy flag

        IMXRT_LPSPI3_S.TCR = (IMXRT_LPSPI3_S.TCR & ~(LPSPI_TCR_FRAMESZ(47))) | LPSPI_TCR_FRAMESZ(15);
        //IMXRT_LPSPI3_S.TCR = (IMXRT_LPSPI3_S.TCR & ~(LPSPI_TCR_RXMSK) | LPSPI_TCR_TXMSK);
        IMXRT_LPSPI3_S.FCR = 0;
        IMXRT_LPSPI3_S.DER = LPSPI_DER_RDDE; //enable DMA on RX
        IMXRT_LPSPI3_S.SR = 0x3f00; // clear out all of the other status...

        //uint32_t **x = (uint32_t**)SPI1.spiclass_lpspi3_hardware.pcs_select_input_register;
        //x[0][0] = 0;
        *(portConfigRegister(DA_SYNC)) = 0;
        //SPI1.spiclass_lpspi3_hardware.pcs_select_input_register[0][0] = SPI1.spiclass_lpspi3_hardware.cs_mux[0];
        dmarx.enable();
        digitalWrite(AD7607_CHIP_SELECT, LOW);
        for (int i=0; i < 8; i++) {
            IMXRT_LPSPI3_S.TDR = 0xFFFF;
        }
    }

    static void (*fn_consumeIncommingSamples)(int8_t *, unsigned int);
    static void (*fn_setOutgoingSamples)(int[], unsigned int);

protected:
    static volatile bool alreadyReset;

    // ADC signals conversion complete - start sampling process: read 8 16bit samples from adc and write 8 16bit samples to dac...
    static void busyFallingEdgeISR() {
       // if (read_index < 128) {
            //beginReceive();
       // };
    }

    static void toggleStartConversion(){
        digitalWrite(AD7607_START_CONVERSION, LOW);
        digitalWrite(AD7607_START_CONVERSION, HIGH);
    }

    static void config_dma(void)
    {
        dmatx.begin(true); // allocate the DMA channel first
        dmatx.disable();
        dmatx.sourceBuffer(txbuf, 32);
        dmatx.transferSize(4);
        dmatx.transferCount(8);
        dmatx.destination((volatile uint32_t &)IMXRT_LPSPI3_S.TDR);
        dmatx.triggerAtHardwareEvent(DMAMUX_SOURCE_LPSPI3_TX);
        dmatx.disableOnCompletion();
        dmatx.attachInterrupt(txisr);
        dmatx.interruptAtCompletion();

        dmarx.begin(true); // allocate the DMA channel first
        dmarx.disable();
        dmarx.destinationBuffer(rxbuf, 16);
        dmarx.transferSize(2);
        dmarx.transferCount(8);
        dmarx.source((volatile uint16_t &)IMXRT_LPSPI3_S.RDR);
        dmarx.triggerAtHardwareEvent(DMAMUX_SOURCE_LPSPI3_RX);
        dmarx.disableOnCompletion();
        dmarx.attachInterrupt(rxisr);
        dmarx.interruptAtCompletion();
    }

    static void txisr(void) {
        dmatx.clearInterrupt();

        SPI1.endTransaction();

        IMXRT_LPSPI3_S.FCR = LPSPI_FCR_TXWATER(15); // _spi_fcr_save; // restore the FSR status...
        IMXRT_LPSPI3_S.DER = 0;
        IMXRT_LPSPI3_S.CR = LPSPI_CR_MEN | LPSPI_CR_RRF | LPSPI_CR_RTF; // actually clear both...
        IMXRT_LPSPI3_S.SR = 0x3f00;    // clear out all of the other status...
        while (IMXRT_LPSPI3_S.FSR & 0x1f);
        while (IMXRT_LPSPI3_S.SR & LPSPI_SR_MBF) ;
        beginReceive();
    }

    static void rxisr(void) {
        dmarx.clearInterrupt();
        SPI1.endTransaction();

        IMXRT_LPSPI3_S.FCR = LPSPI_FCR_TXWATER(15); // _spi_fcr_save; // restore the FSR status...
        IMXRT_LPSPI3_S.DER = 0;
        IMXRT_LPSPI3_S.CR = LPSPI_CR_MEN | LPSPI_CR_RRF | LPSPI_CR_RTF; // actually clear both...
        IMXRT_LPSPI3_S.SR = 0x3f00;    // clear out all of the other status...

        while (IMXRT_LPSPI3_S.FSR & 0x1f);
        while (IMXRT_LPSPI3_S.SR & LPSPI_SR_MBF) ;  //Status Register? Module Busy flag
        digitalWrite(AD7607_CHIP_SELECT, HIGH);
        /*
        for (int count=0; count<16; count++) {
            Serial.printf("%x, ", rxbuf[count]);
        }
        Serial.println();
        */
        if (read_index < 128 && fn_consumeIncommingSamples != nullptr) {
            fn_consumeIncommingSamples(rxbuf, read_index);
        };
    }

    static void timer(void){
        if (read_index > 2 && alreadyReset)
            alreadyReset = false;
        if (read_index < 127) {
            read_index++;
            toggleStartConversion();
            beginTransfer();
        } else {
            _timer.end();
        }
    }

    static void setClockDivider_noInline(uint32_t clk)
    {
        static const uint32_t clk_sel[4] = {
                664615384,  // PLL3 PFD1
                720000000,  // PLL3 PFD0
                528000000,  // PLL2
                396000000}; // PLL2 PFD2

        uint32_t cbcmr = CCM_CBCMR;
        uint32_t clkhz = clk_sel[(cbcmr >> 4) & 0x03] / (((cbcmr >> 26 ) & 0x07 ) + 1);  // LPSPI peripheral clock

        uint32_t d, div;
        d = clk ? clkhz/clk : clkhz;

        if (d && clkhz/d > clk) d++;
        if (d > 257) d= 257;  // max div
        if (d > 2) {
            div = d-2;
        } else {
            div =0;
        }

        uint32_t ccr = LPSPI_CCR_SCKDIV(div) | LPSPI_CCR_DBT(div/2) | LPSPI_CCR_PCSSCK(div/2);

        //Serial.printf("SPI.setClockDivider_noInline CCR:%x TCR:%x\n", _ccr, port().TCR);
        IMXRT_LPSPI3_S.CR = 0;
        IMXRT_LPSPI3_S.CFGR1 = LPSPI_CFGR1_MASTER | LPSPI_CFGR1_SAMPLE;
        IMXRT_LPSPI3_S.CCR = ccr;
        IMXRT_LPSPI3_S.CR = LPSPI_CR_MEN;
    }

    static bool _initialized_shared_context;
    static unsigned int read_index;

    static DMAChannel dmarx;
    static DMAChannel dmatx;

    static volatile uint8_t txbuf[32];
    static int txvoltages[8];

    static int8_t rxbuf[32];

    static IntervalTimer _timer;

};

#endif // common_ad5754_ad7606_h_