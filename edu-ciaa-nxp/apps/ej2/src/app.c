/*
 * main.c
 *
 *  Created on: 13 oct. 2022
 *      Author: fep
 */

#include "app.h"         // <= Su propia cabecera (opcional)
#include "sapi.h"        // <= Biblioteca sAPI

int main( void )
{
        // Inicializar y configurar la placa de desarrollo
        boardConfig();

        while( TRUE ) {
                gpioWrite( CIAA_BOARD_LED, 1 );
                delay(1000);
                gpioWrite( CIAA_BOARD_LED, 0 );
                delay(1000);
        }
}
