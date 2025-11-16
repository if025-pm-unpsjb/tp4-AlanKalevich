#include "sapi.h"

int main(void) {
   boardConfig();
   uartConfig( UART_USB, 9600 );

   printf("Echo listo\r\n");

   uint8_t input;
   char buffer[64];
   int i = 0;

   while(TRUE) {
      if( uartReadByte(UART_USB, &input) ) {

         // Ver lo que se escribe en tiempo real:
         uartWriteByte( UART_USB, input );

         // Si es fin de línea (\r o \n), cierro string y hago echo "de verdad"
         if( input == '\r' || input == '\n' ) {
            buffer[i] = '\0';  // termino el string

            // Enviar solo lo que estaba en el buffer (sin el \r/\n)
            uartWriteString( UART_USB, "\r\nECO: " );
            uartWriteString( UART_USB, buffer );
            uartWriteString( UART_USB, "\r\n" );

            i = 0;  // reinicio índice para el próximo mensaje
         }
         else if( i < (int)sizeof(buffer) - 1 ) {
            buffer[i++] = (char)input;   // acumulo char en el buffer
         }
      }
   }

   return 0;
}
