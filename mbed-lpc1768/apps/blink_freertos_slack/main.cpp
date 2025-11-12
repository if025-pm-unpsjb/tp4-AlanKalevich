#include "mbed.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "slack.h"

#define TASK_CNT 4
#define TASK_1_WCET 1000
#define TASK_2_WCET 1000
#define TASK_3_WCET 1000
#define TASK_4_WCET 1000
#define TASK_1_PERIOD 3000
#define TASK_2_PERIOD 4000
#define TASK_3_PERIOD 6000
#define TASK_4_PERIOD 12000
#define TASK_1_PRIO configMAX_PRIORITIES - configSS_SLACK_PRIOS - 1
#define TASK_2_PRIO configMAX_PRIORITIES - configSS_SLACK_PRIOS - 2
#define TASK_3_PRIO configMAX_PRIORITIES - configSS_SLACK_PRIOS - 3
#define TASK_4_PRIO configMAX_PRIORITIES - configSS_SLACK_PRIOS - 4

void task(void *p);
void vUtilsBusyWait( TickType_t ticks );

static TaskHandle_t task_handles[ TASK_CNT ];

traceString slack_channel;


PwmOut leds[] = { LED1, LED2, LED3, LED4 };

Serial pc( USBTX, USBRX );

int main()
{
    xTraceEnable( TRC_START );
    slack_channel = xTraceRegisterString("Slack Events");

    // Periodic tasks.
    xTaskCreate( task, "T1", 256, (void*) 1, TASK_1_PRIO, &task_handles[ 0 ] );
    xTaskCreate( task, "T2", 256, (void*) 2, TASK_2_PRIO, &task_handles[ 1 ] );
    xTaskCreate( task, "T3", 256, (void*) 3, TASK_3_PRIO, &task_handles[ 2 ] );
    xTaskCreate( task, "T4", 256, (void*) 4, TASK_4_PRIO, &task_handles[ 3 ] );

    #if( configUSE_SLACK_STEALING == 1 )
        vSlackSetTaskParams( task_handles[ 0 ], PERIODIC_TASK, TASK_1_PERIOD, TASK_1_PERIOD, TASK_1_WCET );
        vSlackSetTaskParams( task_handles[ 1 ], PERIODIC_TASK, TASK_2_PERIOD, TASK_2_PERIOD, TASK_2_WCET );
        vSlackSetTaskParams( task_handles[ 2 ], PERIODIC_TASK, TASK_3_PERIOD, TASK_3_PERIOD, TASK_3_WCET );
        vSlackSetTaskParams( task_handles[ 3 ], PERIODIC_TASK, TASK_4_PERIOD, TASK_4_PERIOD, TASK_4_WCET );
    #endif

    vTaskStartScheduler();
	for(;;);
}

void task(void *params)
{
    int32_t taskId = (int32_t) params;
    SsTCB_t *pxTaskSsTCB;
    pxTaskSsTCB = pvSlackGetTaskSsTCB( NULL );

	for(;;) {
	    //vTracePrintF( slack_channel, "%d - %d", xSlackGetAvailableSlack(), pxTaskSsTCB->xSlack );

	    leds[ taskId - 1] = 1;
	    for (int i = taskId; i <=4  - 1; i++) {
	        if (leds[ i ] > 0) {
	            leds[ i ] = 0.1;
	        }
	    }

        TickType_t elapsedTicks = 0;
        TickType_t currentTick = 0;
        while ( elapsedTicks < pxTaskSsTCB->xWcet - 200) {
            leds[ taskId - 1] = 1;
            currentTick = xTaskGetTickCount();
            while ( currentTick == xTaskGetTickCount() ) {
                asm("nop");
            }
            elapsedTicks++;
        }

        leds[ taskId - 1] = 0;

        //vTracePrintF( slack_channel, "%d - %d", xSlackGetAvailableSlack(), pxTaskSsTCB->xSlack );

        vTaskDelayUntil( &( pxTaskSsTCB->xPreviousWakeTime ), pxTaskSsTCB->xPeriod );
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


#if ( configUSE_SLACK_STEALING == 1 )
void vApplicationDebugAction( void *param )
{
    ( void ) param;

    taskDISABLE_INTERRUPTS();

    for( ;; )
    {
        leds[ 4 ] = 1;
        wait_ms(1000);
        leds[ 4 ] = 0;
        wait_ms(1000);
    }
}

extern void vApplicationTickHook( void ) __attribute__((weak));
void vApplicationTickHook( void )
{
    if ( xTaskGetTickCountFromISR() % 100 == 0 ) {
        vTracePrintF( slack_channel, "%d", xSlackGetAvailableSlack() );
    }
    vSlackDeadlineCheck();
}

void vApplicationNotSchedulable( void )
{
    taskDISABLE_INTERRUPTS();

    pc.printf( "RTS not schedulable.\r\n" );

    for( ;; )
    {
        leds[ 1 ] = 1;
        wait_ms(1000);
        leds[ 1 ] = 0;
        wait_ms(1000);
    }
}

void vApplicationDeadlineMissedHook( char *pcTaskName, const SsTCB_t *xSsTCB,
        TickType_t xTickCount )
{
    ( void ) xSsTCB;

    taskDISABLE_INTERRUPTS();

    pc.printf( "\n\r%s missed its deadline at %d\n\r", pcTaskName, xTickCount);

    for( ;; )
    {
        leds[ 0 ] = 1;
        wait_ms( 1000 );
        leds[ 0 ] = 0;
        wait_ms( 1000 );
    }
}
#endif
