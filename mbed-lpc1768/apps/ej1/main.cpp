#include "mbed.h"

// Array de 4 LEDs
DigitalOut leds[] = { DigitalOut(LED1), DigitalOut(LED2), DigitalOut(LED3), DigitalOut(LED4) };
const int NUM_LEDS = sizeof(leds) / sizeof(leds[0]);


void incrementar() {
    for (int k = 0; k < NUM_LEDS; k++) {
        if (leds[k] == 0) {
            leds[k] = 1;
            break;
        } else {
            leds[k] = 0;
        }
    }
}

int main() {
    while (1) {
        for (int i = 0; i < 15; i++) {
            incrementar();
            wait(0.1f);
        }
    }
}
