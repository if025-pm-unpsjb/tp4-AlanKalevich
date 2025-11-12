#include "mbed.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

void task(void *p);
void vUtilsBusyWait( TickType_t ticks );

DigitalOut led1(LED1);

SemaphoreHandle_t s;

int main()
{
    xTraceEnable( TRC_START );
    s = xSemaphoreCreateMutex();
    led1 = 0;
    xTaskCreate( task, "T1", 256, NULL, configMAX_PRIORITIES - 1, NULL );
    vTaskStartScheduler();
	for(;;);
}

void task(void *p) {
	TickType_t xPeriod = 3000;
	for(;;) {
		led1 = 1;
		vUtilsBusyWait(900);
		led1 = 0;
		vTaskDelay( xPeriod );
	}
}

void vUtilsBusyWait( TickType_t ticks )
{
    TickType_t elapsedTicks = 0;
    TickType_t currentTick = 0;
    while ( elapsedTicks < ticks ) {
        currentTick = xTaskGetTickCount();
        while ( currentTick == xTaskGetTickCount() ) {
                asm("nop");
        }
        elapsedTicks++;
    }
}

void vConfigureTimerForRunTimeStats( void )
{
const unsigned long TCR_COUNT_RESET = 2,
                    CTCR_CTM_TIMER = 0x00,
                    TCR_COUNT_ENABLE = 0x01;

    /* Power up and feed the timer with a clock. */
    LPC_SC->PCONP |= 0x02UL;
    LPC_SC->PCLKSEL0 = (LPC_SC->PCLKSEL0 & (~(0x3<<2))) | (0x01 << 2);

    /* Reset Timer 0 */
    LPC_TIM1->TCR = TCR_COUNT_RESET;

    /* Just count up. */
    LPC_TIM1->CTCR = CTCR_CTM_TIMER;

    /* Prescale to a frequency that is good enough to get a decent resolution,
    but not too fast so as to overflow all the time. */
    LPC_TIM1->PR =  ( SystemCoreClock / 10000UL ) - 1UL;

    /* Start the counter. */
    LPC_TIM1->TCR = TCR_COUNT_ENABLE;
}
