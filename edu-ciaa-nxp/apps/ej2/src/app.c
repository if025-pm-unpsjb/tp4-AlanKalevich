#include "sapi.h"

int main(void) {
   boardConfig();
   uartConfig( UART_USB, 9600 );

   gpioMap_t leds[] = { LEDB, LED1, LED2, LED3 };
   const int LEDS_SIZE = sizeof(leds) / sizeof(leds[0]);

   gpioMap_t botones[] = { TEC1, TEC2, TEC3, TEC4 };
   const int BOTONES_SIZE = sizeof(botones) / sizeof(botones[0]);

   // Inicializacion de estado previo de cada botón
   bool_t estadoAnterior[ BOTONES_SIZE ];
   for(int i = 0; i < BOTONES_SIZE; i++){
      estadoAnterior[i] = TRUE;
   }

   while(1){

      for(int i = 0; i < BOTONES_SIZE; i++){

         bool_t estadoActual = gpioRead(botones[i]);

         // si ahora esta presionado, y antes no lo estaba
         if(estadoActual == FALSE && estadoAnterior[i] == TRUE){

            if(i < LEDS_SIZE){
               gpioWrite(leds[i], ON);
            }

            printf("Boton %d presionado\r\n", i+1);
         }

         // si ahora NO esta presionado, y antes si lo estaba
         if(estadoActual == TRUE && estadoAnterior[i] == FALSE){

            if(i < LEDS_SIZE){
               gpioWrite(leds[i], OFF);
            }
         }

         estadoAnterior[i] = estadoActual;
      }

      delay(20);   // pequeño debounce por software
   }

   return 0;
}
