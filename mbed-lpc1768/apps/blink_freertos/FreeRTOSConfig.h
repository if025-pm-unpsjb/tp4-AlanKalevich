#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

extern uint32_t SystemCoreClock;

#define configUSE_PREEMPTION			1
#define configUSE_IDLE_HOOK				0
#define configUSE_TICK_HOOK				0
#define configCPU_CLOCK_HZ				( SystemCoreClock )
#define configTICK_RATE_HZ				( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES			( 5 )
#define configMINIMAL_STACK_SIZE		( ( unsigned short ) 130 )
#define configTOTAL_HEAP_SIZE			( ( size_t ) ( 16 * 1024 ) )
#define configMAX_TASK_NAME_LEN			( 10 )
#define configUSE_TRACE_FACILITY		1
#define configUSE_16_BIT_TICKS			0
#define configIDLE_SHOULD_YIELD			1
#define configUSE_MUTEXES				1
#define configQUEUE_REGISTRY_SIZE		0
#define configCHECK_FOR_STACK_OVERFLOW	0
#define configUSE_RECURSIVE_MUTEXES		0
#define configUSE_MALLOC_FAILED_HOOK	0
#define configUSE_APPLICATION_TASK_TAG	0
#define configUSE_COUNTING_SEMAPHORES	0
#define configGENERATE_RUN_TIME_STATS	1

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 			0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/* Software timer definitions. */
#define configUSE_TIMERS				0

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet			0
#define INCLUDE_uxTaskPriorityGet			0
#define INCLUDE_vTaskDelete					0
#define INCLUDE_vTaskSuspend				0
#define INCLUDE_vTaskDelayUntil				0
#define INCLUDE_vTaskDelay					1
#define INCLUDE_pcTaskGetTaskName       	0
#define INCLUDE_xTaskGetCurrentTaskHandle 	1

/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
	/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		5        /* 32 priority levels */   // MODIFIED 2020-07-18
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			0x1f  // changed from 0x0f on 2020-07-18

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	32

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

/* For accurate execution time in FreeRTOS TAD */
#if configGENERATE_RUN_TIME_STATS == 1
    extern void vConfigureTimerForRunTimeStats( void );
    #define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() vConfigureTimerForRunTimeStats()
    #define portGET_RUN_TIME_COUNTER_VALUE() LPC_TIM1->TC
    #define configRECORD_STACK_HIGH_ADDRESS 1
#endif

#if ( configUSE_TRACE_FACILITY == 1 )
    #include "trcRecorder.h"
#endif

#endif /* FREERTOS_CONFIG_H */
