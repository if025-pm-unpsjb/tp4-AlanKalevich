/*
 * main.c
 *
 *  Created on: 13 oct. 2022
 *      Author: fep
 */

#include "FreeRTOS.h"
#include "task.h"
#include "app.h"         // <= Su propia cabecera (opcional)
#include "sapi.h"        // <= Biblioteca sAPI
#include "semphr.h"

void task(void *p);

SemaphoreHandle_t s;

int main( void )
{
        // Inicializar y configurar la placa de desarrollo
        boardConfig();

        pwmConfig(0, PWM_ENABLE);

        pwmConfig(PWM7, PWM_ENABLE_OUTPUT);

        pwmWrite(PWM7, 10);

    	s = xSemaphoreCreateMutex();

        xTaskCreate( task, "T1", 256, NULL, configMAX_PRIORITIES - 1, NULL );
        vTaskStartScheduler();
        for(;;);
}

void task(void *p)
{
		TickType_t xPeriod = 10;
		int dutyCicle = 0;
        while( TRUE ) {
                pwmWrite(PWM7, dutyCicle);
                vTaskDelay( xPeriod );
                dutyCicle = ( dutyCicle + 1 ) % 255;
        }
}
