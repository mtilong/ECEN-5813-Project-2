#include <stdio.h>
#include <stdlib.h>


int delay_lookup[] = {1.2e8,2.4e8,4.8e8,7.2e8}; //lookup table, delay counter
//------------------------------------------------------------------------------
int keepgoing;

void delay(int num_ms)
{
	//(ms*cpufreq_Mhz*1000)/10.. CPUfreq = 2.4GHz //accoungitn for MAC CPUR frequency.
	//int i = 0;
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

//Turn Green On
void LED_cyclegreen_On(int delay_ms)
{
	//PTB->PSOR = MASK(GREEN_LED_SHIFT);//on
	printf("Turn on GREEN LED and start timer %d ms delay.\n", delay_ms);
	delay(delay_ms);
}

//Turn Green Off
void LED_cyclegreen_OFF(int delay_ms)
{
	//PTB->PCOR = MASK(GREEN_LED_SHIFT);//
	printf("Turn off GREEN LED and start timer %d ms delay.\n", delay_ms);
	delay(delay_ms);
}

//Turn Red On
void LED_cyclered_On(int delay_ms)
{
	//PTB->PSOR = MASK(RED_LED_SHIFT);//on
	printf("Turn on RED and start timer %d ms delay.\n", delay_ms);
	delay(delay_ms);
}

//Turn Red Off
void LED_cyclered_OFF(int delay_ms)
{
	//PTB->PCOR = MASK(RED_LED_SHIFT);//
	printf("Turn off RED and start timer %d ms delay.\n", delay_ms);
	delay(delay_ms);
}

//Turn Blue On
void LED_cycleblue_On(int delay_ms)
{
	//PTD->PSOR = MASK(BLUE_LED_SHIFT);//on
	printf("Turn on BLUE and start timer %d ms delay.\n", delay_ms);
	delay(delay_ms);
}

//Turn Blue Off
void LED_cycleblue_OFF(int delay_ms)
{
	//PTD->PCOR = MASK(BLUE_LED_SHIFT);//
	printf("Turn off and start timer %d ms delay.\n", delay_ms);
	delay(delay_ms);
}

//Alternating LED GREEN
void LED_alt1(int delay_ms)
{
    int i;
    int flag = 1;
    for(i=0; i<3; i++)
    {
        if (flag == 1)
        {
            LED_cyclegreen_OFF(delay_ms);
            flag = 0;
        }
        else if(flag ==0)
        {
            LED_cyclegreen_On(delay_ms);
            flag =1;
        }


    }

}


int main(void) {


    /* Just incrementing a counter. */

    int k;
    int j;
    j=0;

    int delay_array[] ={500,500,1000,500,2000,500,3000,500};
    printf("Start Blinking\n");

    for(k=0; k<=8; k++)
    {
        LED_alt1(delay_array[k]);
        LED_cyclered_On(delay_array[k]);
        LED_cyclered_OFF(delay_array[k]);

        LED_alt1(delay_array[k]);
        LED_cyclegreen_On(delay_array[k]);
        LED_cyclegreen_OFF(delay_array[k]);

        LED_alt1(delay_array[k]);
        LED_cycleblue_On(delay_array[k]);
        LED_cycleblue_OFF(delay_array[k]);
    }

    printf("END Blinking\n");
    return 0 ;
}
