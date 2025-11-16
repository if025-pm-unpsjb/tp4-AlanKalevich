#include "mbed.h"
#include <cstdlib>

Serial pc(USBTX, USBRX); // tx, rx
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
    float waitTime = 1.0f;   // tiempo inicial en segundos

    while (1) {
        incrementar();
        wait(waitTime);

        if (pc.readable()) {
            char buffer[16];
            int i = 0;

            // Leer hasta ENTER (\n o \r) o hasta llenar el buffer
            while (i < 15) {
                char c = pc.getc();
                if (c == '\n' || c == '\r') {
                    break;
                }
                buffer[i++] = c;
            }
            buffer[i] = '\0';  // terminador de string C

            // Pasar el string a número (puede ser float)
            float nuevoTiempo = atof(buffer);

            if (nuevoTiempo > 0) {
                waitTime = nuevoTiempo;
                pc.printf("Nuevo tiempo: %.2f s\r\n", waitTime);
            } else {
                pc.printf("Entrada invalida: %s\r\n", buffer);
            }
        }
    }
}
