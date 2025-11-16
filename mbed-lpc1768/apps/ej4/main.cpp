#include "mbed.h"

Serial pc(USBTX, USBRX); // tx, rx
DigitalOut leds[] = { DigitalOut(LED1), DigitalOut(LED2), DigitalOut(LED3), DigitalOut(LED4) };
const int NUM_LEDS = sizeof(leds) / sizeof(leds[0]);

int main() {
    pc.baud(9600);

    while(1) {
    	char inputChar = pc.getc();
    	int inputInt = inputChar - '0';   // '1' -> 1, '2' -> 2, etc.

        if (inputInt < 1 || inputInt > NUM_LEDS) {
            pc.printf("Valor invalido: %d\n", inputInt);
            continue; // vuelve al while(1) a leer otro
        }

        bool interrupcion = false;

        while (!interrupcion){

        	leds[inputInt-1] = 1;
        	wait(0.25f);
        	leds[inputInt-1] = 0;
            wait(0.25f);

        	if(pc.readable()) {
        		interrupcion = true;
        	}
        }
    }
}
