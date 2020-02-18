#include <stdio.h>
#include <stdlib.h>


int delay_lookup[] = {1.2e14,2.4e14,4.8e14,7.2e14}; //lookup table, delay counter
//------------------------------------------------------------------------------
int keepgoing;

void delay(int num_ms)
{
	//(ms*cpufreq_Ghz*1000)/10.. CPUfreq = 2.4GHz //accoungitn for MAC CPUR frequency.
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

void KL25Z_RGB_Flasher(void) {

	printf("LED GREEN ON\n");
	printf("Start timer 500ms delay.\n");
	delay(500);

	printf("LED GREEN OFF\n");
	printf("Start second timer 500ms delay.\n");
	delay(500);

	printf("LED GREEN ON\n");
	printf("Start timer 1000ms delay.\n");
	delay(1000);

	printf("LED GREEN OFF\n");
	printf("Start third timer 500ms delay.\n");
	delay(500);

	printf("LED GREEN ON\n");
	printf("Start timer 2000ms delay.\n");
	delay(2000);

	printf("LED GREEN OFF\n");
	printf("Start fourth timer 500ms delay.\n");
	delay(500);


	printf("LED GREEN ON\n");
	printf("Start timer 3000ms delay.\n");
	delay(3000);

	printf("LED GREEN OFF\n");
	printf("Start fifth timer 500ms delay.\n");
	delay(500);
}


int main(void) {


    /* Just incrementing a counter. */
    //int x;
    //int i;
    int j;
    j=0;
    printf("Start Blinking\n");

    while(j<3)
            {
    			j++;
                KL25Z_RGB_Flasher();

                if(j == 1)
                {
                    printf("LED BLUE ON.\n");
                    printf("Start 1000 ms delay.\n");
                    delay(1000);
                    printf("LED BLUE OFF.\n");
                }

                 else if(j == 2)
                {
                    printf("LED GREEN ON.\n");
                    printf("Start 1000 ms delay.\n");
                    delay(1000);
                    printf("LED GREEN OFF.\n");
                }

                else if(j == 3)
                {
                    printf("LED RED ON.\n");
                    printf("Start 1000 ms delay.\n");
                    delay(1000);
                    printf("LED RED OFF.\n");
                }

            }
    printf("END Blinking\n");

    return 0 ;
}
