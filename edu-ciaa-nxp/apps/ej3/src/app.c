#include "sapi.h"

int main(void) {
   boardConfig();
   uartConfig( UART_USB, 9600 );

   gpioMap_t leds[] = { LEDR, LEDG, LEDB };
   const int LEDS_SIZE = sizeof(leds) / sizeof(leds[0]);

   gpioMap_t botones[] = { TEC1, TEC2, TEC3, TEC4 };
   const int BOTONES_SIZE = sizeof(botones) / sizeof(botones[0]);

   bool_t habilitado = FALSE;

   // valor previo de tec1
   bool_t prevTec1 = TRUE;

   while(1){

      bool_t estadoTec1 = gpioRead(botones[0]);

      // si cambio respecto del anterior, toogle
      if(estadoTec1 == FALSE && prevTec1 == TRUE){
         habilitado = !habilitado;
      }

      prevTec1 = estadoTec1;

	   if (habilitado){
		   for(int i = 1; i < BOTONES_SIZE; i++){
			   if (gpioRead(botones[i]) == FALSE){
				   gpioWrite(leds[i-1], ON);
			   } else {
				   gpioWrite(leds[i-1], OFF);
			   }
		   }

	   } else{
		   for(int i = 1; i < BOTONES_SIZE; i++){
			   gpioWrite(leds[i-1], OFF);
		   }
	   }

	   delay(20);   // pequeño debounce por software
   }

   return 0;
}
