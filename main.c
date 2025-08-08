/*************************************************************************
 *  Basic stereo loop code for C6713 DSK and AIC23 codec
 *  D. Richard Brown on 22-Aug-2011
 *  Based on code from "Real-Time Digital Signal Processing Based on TMS320C6000"
 *  by N. Kehtarnavaz and N. Kim.
 *************************************************************************/

#define CHIP_6713 1

#include <stdio.h>
#include <c6x.h>
#include <csl.h>
#include <csl_mcbsp.h>
#include <csl_irq.h>

#include <math.h>
#include <stdlib.h>

#include <dsk6713.h>
#include <dsk6713_aic23.h>
#include <dsk6713_led.h>
#include <dsk6713_dip.h>

#define Ncoef 251
#include "tmwtypes.h"
#include "coefis_Hamming250.h"



Uint32 data;
int a,i;
real64_T *Filter_Bank[8] = {Bank_1,Bank_2,Bank_3,Bank_4,Bank_5,Bank_6,Bank_7,Bank_8};
short x[Ncoef];
real64_T y = 0.0;
int val_in;

DSK6713_AIC23_CodecHandle hCodec;                           // Codec handle
DSK6713_AIC23_Config config = DSK6713_AIC23_DEFAULTCONFIG;  // Codec configuration with default settings
interrupt void serialPortRcvISR(void);

void main()
{
    DSK6713_init();
    DSK6713_LED_init();
    DSK6713_DIP_init();
    hCodec = DSK6713_AIC23_openCodec(0, &config);   // open codec and get handle

    // Configure buffered serial ports for 32 bit operation
    // This allows transfer of both right and left channels in one read/write
    MCBSP_FSETS(SPCR1, RINTM, FRM);
    MCBSP_FSETS(SPCR1, XINTM, FRM);
    MCBSP_FSETS(RCR1, RWDLEN1, 32BIT);
    MCBSP_FSETS(XCR1, XWDLEN1, 32BIT);

    // set codec sampling frequency
    DSK6713_AIC23_setFreq(hCodec, DSK6713_AIC23_FREQ_48KHZ);

    // interrupt setup
    IRQ_globalDisable();            // Globally disables interrupts
    IRQ_nmiEnable();                // Enables the NMI interrupt
    IRQ_map(IRQ_EVT_RINT1,15);      // Maps an event to a physical interrupt
    IRQ_hook(15, serialPortRcvISR);
    IRQ_enable(IRQ_EVT_RINT1);      // Enables the event
    IRQ_globalEnable();             // Globally enables interrupts
    while(1){
    }
}
// Note: Configurar optimización de código y debug.
interrupt void serialPortRcvISR()
{
    //union {Uint32 combo; short channel[2];} temp;

    data = MCBSP_read(DSK6713_AIC23_DATAHANDLE);
    // Note that right channel is in temp.channel[0]
    // Note that left channel is in temp.channel[1]
    //MCBSP_write(DSK6713_AIC23_DATAHANDLE,(sine_table[loop]*gain));
    val_in = 0;
    val_in = DSK6713_DIP_get(0);
    val_in =val_in | DSK6713_DIP_get(1) << 1;
    val_in =val_in | DSK6713_DIP_get(2) << 2;
    //val_in =val_in | DSK6713_DIP_get(3) << 3;
    DSK6713_rset(DSK6713_USER_REG, (val_in|DSK6713_DIP_get(3) << 3));
    if(DSK6713_DIP_get(3) == 1){
        MCBSP_write(DSK6713_AIC23_DATAHANDLE,3*data);
    }
    else{
        for(i = Ncoef-1 ; i > 0; i--){
            x[i] = x[i-1];
        }
        x[0] = 3*((short)data);
        y=0.0;
        for(i = 0; i < Ncoef; i++){
            y = y + ((Filter_Bank[val_in][i]) * (real64_T)x[i]);
        }
        MCBSP_write(DSK6713_AIC23_DATAHANDLE,(short)y);
    }
    return;
}
