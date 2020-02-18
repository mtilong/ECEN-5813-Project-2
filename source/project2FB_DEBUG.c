/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * Written By: Morgan Tilong
 * @file    project2FB.c
 * @brief   Application entry point.
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

// Macros
#define SCAN_OFFSET 544               // Offset for scan range
#define SCAN_DATA TSI0->DATA & 0xFFFF // Accessing the bits held in TSI0_DATA_TSICNT
#define MASK(x) (1UL << (x))

#define RED_LED_SHIFT   (18)	// on port B (PTB18)
#define GREEN_LED_SHIFT (19)	// on port B (PTB19)
#define BLUE_LED_SHIFT  (1)		// on port D (PTB1)

#define RED  (0)	// on port B (PTB18)
#define GREEN (1)	// on port B (PTB19)
#define BLUE  (2)  // on port D (PTB1)




//lookup table containing delay counter
const uint32_t delay_lookup[] = {2400000,4800000,9600000,14000000}; //estimated values
//const uint32_t delay_lookup[] = {1.04e-10,2.08e-10,4.16e-10,8.33e-10};

int keepgoing; //conditional variable


//time delay function
void delay(int num_ms)
{
	//(ms*cpufreq_Mhz*1000)/10.. CPUfreq = 48MHz

	int loopcount = 0;
	keepgoing = 1;

	if (num_ms == 500 )
	{
		loopcount = delay_lookup[0]; //count for 500ms
	}
	else if(num_ms == 1000)
	{
		loopcount = delay_lookup[1]; //count for 1000ms
	}
	else if(num_ms == 2000)
	{
		loopcount = delay_lookup[2]; //count for 2000ms
	}
	else if(num_ms == 3000)
	{
		loopcount = delay_lookup[3]; //count for 3000ms
	}

	while(keepgoing)  //always true
	{
		loopcount--;  //decrement duration
		if(loopcount <= 0) //time reached
			keepgoing = 0;   //exit delay
	}
}


void Touch_Init()
{
    // Enable clock for TSI PortB 16 and 17
    SIM->SCGC5 |= SIM_SCGC5_TSI_MASK;


    TSI0->GENCS = TSI_GENCS_OUTRGF_MASK |  // Out of range flag, set to 1 to clear
                                //TSI_GENCS_ESOR_MASK |  // This is disabled to give an interrupt when out of range.  Enable to give an interrupt when end of scan
                                TSI_GENCS_MODE(0u) |  // Set at 0 for capacitive sensing.  Other settings are 4 and 8 for threshold detection, and 12 for noise detection
                                TSI_GENCS_REFCHRG(0u) | // 0-7 for Reference charge
                                TSI_GENCS_DVOLT(0u) | // 0-3 sets the Voltage range
                                TSI_GENCS_EXTCHRG(0u) | //0-7 for External charge
                                TSI_GENCS_PS(0u) | // 0-7 for electrode prescaler
                                TSI_GENCS_NSCN(31u) | // 0-31 + 1 for number of scans per electrode
                                TSI_GENCS_TSIEN_MASK | // TSI enable bit
                                //TSI_GENCS_TSIIEN_MASK | //TSI interrupt is disables
                                TSI_GENCS_STPE_MASK | // Enables TSI in low power mode
                                //TSI_GENCS_STM_MASK | // 0 for software trigger, 1 for hardware trigger
                                //TSI_GENCS_SCNIP_MASK | // scan in progress flag
                                TSI_GENCS_EOSF_MASK ; // End of scan flag, set to 1 to clear
                                //TSI_GENCS_CURSW_MASK; // Do not swap current sources

    // The TSI threshold isn't used is in this application
//    TSI0->TSHD =     TSI_TSHD_THRESH(0x0000) |
//                                TSI_TSHD_THRESL(0x0000);

}


int Touch_Scan_LH(void)
{
    int scan;
    TSI0->DATA =     TSI_DATA_TSICH(10u); // Using channel 10 of The TSI
    TSI0->DATA |= TSI_DATA_SWTS_MASK; // Software trigger for scan
    scan = SCAN_DATA;
    TSI0->GENCS |= TSI_GENCS_EOSF_MASK ; // Reset end of scan flag

    return scan - SCAN_OFFSET;
}

//Turn Green On
void LED_cyclegreen_On(int delay_ms)
{
	PTB->PSOR = MASK(GREEN_LED_SHIFT);//on
	PRINTF("Turn on GREEN LED and start timer %d ms delay.\n", delay_ms);
	delay(delay_ms);
}

//Turn Green Off
void LED_cyclegreen_OFF(int delay_ms)
{
	PTB->PCOR = MASK(GREEN_LED_SHIFT);//
	PRINTF("Turn off GREEN LED and start timer %d ms delay.\n", delay_ms);
	delay(delay_ms);
}

//Turn Red On
void LED_cyclered_On(int delay_ms)
{
	PTB->PSOR = MASK(RED_LED_SHIFT);//on
	PRINTF("Turn on RED and start timer %d ms delay.\n", delay_ms);
	delay(delay_ms);
}

//Turn Red Off
void LED_cyclered_OFF(int delay_ms)
{
	PTB->PCOR = MASK(RED_LED_SHIFT);//
	PRINTF("Turn off RED and start timer %d ms delay.\n", delay_ms);
	delay(delay_ms);
}

//Turn Blue On
void LED_cycleblue_On(int delay_ms)
{
	PTD->PSOR = MASK(BLUE_LED_SHIFT);//on
	PRINTF("Turn on BLUE and start timer %d ms delay.\n", delay_ms);
	delay(delay_ms);
}

//Turn Blue Off
void LED_cycleblue_OFF(int delay_ms)
{
	PTD->PCOR = MASK(BLUE_LED_SHIFT);//
	PRINTF("Turn off and start timer %d ms delay.\n", delay_ms);
	delay(delay_ms);
}

void KL25Z_RGB_Flasher(void)
{
	// Enable clock to Port B and Port D
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

	// Make 3 pins GPIO
	//First pin to GPIO
	PORTB->PCR[RED_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_SHIFT] |= PORT_PCR_MUX(1);

	//Second pin to GPIO
	PORTB->PCR[GREEN_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_SHIFT] |= PORT_PCR_MUX(1);

	//Third pin to GPIO
	PORTD->PCR[BLUE_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_SHIFT] |= PORT_PCR_MUX(1);

	// Set ports to outputs
	PTB->PDDR |= (MASK(RED_LED_SHIFT) | MASK(GREEN_LED_SHIFT));
	PTD->PDDR |= MASK(BLUE_LED_SHIFT);

	/*//turn every light on
	PTB->PCOR |= MASK(RED_LED_SHIFT) | MASK(GREEN_LED_SHIFT);
	PTD->PCOR |= MASK(BLUE_LED_SHIFT);*/

	//turn every led off
	PTB->PSOR |= MASK(RED_LED_SHIFT) | MASK(GREEN_LED_SHIFT);
	PTD->PSOR |= MASK(BLUE_LED_SHIFT);
}


int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();


    /* Just incrementing (counters). */
    uint32_t x;
    uint32_t i;
    uint32_t j;
    uint32_t k;

    int LED = 0;
    const uint32_t delay_array[] ={500,500,1000,500,2000,500,3000,500};


    j=0;
    printf("Start Blinking\n");

    while(j<10)
    {
    	j++;
    	for (k=0; k<8; k++)
            {

                KL25Z_RGB_Flasher();


                Touch_Init();           //initialize touch capacitor
                x = Touch_Scan_LH();    // Get the touch sensor input
                printf("Touch Value: %d\n", x);
                for ( i = 0; i < 25000; i++); // Delay for capacitor.

                if((x >= 60) && (x <=120) )
                {
                	LED = RED;
                }
                else if((x >= 121) && (x<= 400))
                {
                	LED = GREEN;
                }
                else if(x >= 410)
                {
                    LED = BLUE;
                }
                else
                {
                 LED = 1;
                }

                if (LED == RED)
                {
                	LED_cyclered_On(delay_array[k]);
                	LED_cyclered_OFF(delay_array[k]);
                	LED = RED;
                }

                else if (LED == GREEN)
                {
                	LED_cyclegreen_On(delay_array[k]);
                	LED_cyclegreen_OFF(delay_array[k]);
                	LED = GREEN;
                }

                else if (LED == BLUE)
                {
                	LED_cycleblue_On(delay_array[k]);
                	LED_cycleblue_OFF(delay_array[k]);
                	LED = BLUE;
                }
            }
    }
    PRINTF("END Blinking\n");
    return 0 ;
}
