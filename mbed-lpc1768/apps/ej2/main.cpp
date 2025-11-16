#include "mbed.h"

PwmOut leds[] = { PwmOut(LED1), PwmOut(LED2), PwmOut(LED3), PwmOut(LED4) };


int main()
{
    while(1)
    {
    	for (int i = 0; i < 4; i++){
    		for(float j = 0; j < 1; j = j + 0.01){
    			leds[i].write(j);
    	        wait(0.001f);
    		}
            wait(0.1f);
    	}
    	for (int i = 0; i < 4; i++){
			for(float j = 1; j > 0; j = j - 0.01){
				leds[i].write(j);
				wait(0.001f);
			}
		wait(0.1f);
		}
    }
}
