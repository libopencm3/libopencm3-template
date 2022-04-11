/*
* Copyright (C) 2019 ChinhPC <chinhphancong@outlook.com>
* 
* Author: ChinhPC
*
* This file is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This file is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

static void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_48mhz();

	/* Enable GPIOA clock for LED & USARTs. */
	rcc_periph_clock_enable(RCC_GPIOA);

	/* Enable clocks for USART2. */
	rcc_periph_clock_enable(RCC_USART1);
}

static void usart_setup(void)
{
	/* Setup USART2 parameters. */
	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 8);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_stopbits(USART1, USART_CR2_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX_RX);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART1);
}

static void gpio_setup(void)
{
	/* Setup GPIO pin GPIO4 on GPIO port A for LEDs. */
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO4);

	/* Setup GPIO pins for USART2 transmit. */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO3);

	/* Setup USART1 TX pin as alternate function. */
	gpio_set_af(GPIOA, GPIO_AF1, GPIO2 | GPIO3);
}

/*
 * Called by main() to create the simply main_func application
 */
static void main_func( void );

int main(void)
{
	// int i, j = 0, c = 0;

	clock_setup();
	gpio_setup();
	usart_setup();

	// /* Blink the LED (PA4) on the board with every transmitted byte. */
	// while (1) {
	// 	gpio_toggle(GPIOA, GPIO4);	/* LED on/off */
	// 	usart_send_blocking(USART1, c + '0'); /* USART1: Send byte. */
	// 	c = (c == 9) ? 0 : c + 1;	 Increment c. 
	// 	if ((j++ % 80) == 0) {		/* Newline after line full. */
	// 		usart_send_blocking(USART1, '\r');
	// 		usart_send_blocking(USART1, '\n');
	// 	}
	// 	for (i = 0; i < 200000; i++) {	/* Wait a bit. */
	// 		__asm__("NOP");
	// 	}
	// }

	main_func();

	return 0;
}

/* The number of items the queue can hold.  This is 1 as the receive task
will remove items as they are added, meaning the send task should always find
the queue empty. */
#define mainQUEUE_LENGTH					( 1 )

/* Priorities at which the tasks are created. */
#define blinkQUEUE_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )
#define uartQUEUE_TASK_PRIORITY			( tskIDLE_PRIORITY + 2 )

/*-----------------------------------------------------------*/

/* The queue used by both tasks. */
static QueueHandle_t xQueue = NULL;

/*-----------------------------------------------------------*/

static void blink( void *pvParameters )
{
	(void)pvParameters;
	/* Block for 500ms. */
	const TickType_t xDelay = ( TickType_t )50 / portTICK_PERIOD_MS;

	/* Blink the LED (PA4) on the board with every transmitted byte. */
	while (1) {
		gpio_toggle(GPIOA, GPIO4);	/* LED on/off */
		vTaskDelay(xDelay);
	}
}

static void uart( void *pvParameters )
{
	(void)pvParameters;
	/* Block for 500ms. */
	const TickType_t xDelay = ( TickType_t ) 100 / portTICK_PERIOD_MS;
	int j = 0, c = 0;

	while (1) {
		usart_send_blocking(USART1, c + '0'); /* USART1: Send byte. */
		c = (c == 9) ? 0 : c + 1;
		if ((j++ % 80) == 0) {		/* Newline after line full. */
			usart_send_blocking(USART1, '\r');
			usart_send_blocking(USART1, '\n');
		}
		vTaskDelay(xDelay);
	}
}

/*-----------------------------------------------------------*/

static void main_func( void )
{
	/* Create the queue. */
	xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( unsigned long ) );

	if( xQueue != NULL )
	{
		/* Start the two tasks as described in the comments at the top of this
		file. */
		xTaskCreate( blink,                   /* The function that implements the task. */
					"Blink",                  /* The text name assigned to the task - for debug only as it is not used by the kernel. */
					configMINIMAL_STACK_SIZE, /* The size of the stack to allocate to the task. */
					NULL,                     /* The parameter passed to the task - just to check the functionality. */
					blinkQUEUE_TASK_PRIORITY, /* The priority assigned to the task. */
					NULL );                   /* The task handle is not required, so NULL is passed. */

		xTaskCreate( uart,                    /* The function that implements the task. */
					"Uart",                   /* The text name assigned to the task - for debug only as it is not used by the kernel. */
					configMINIMAL_STACK_SIZE, /* The size of the stack to allocate to the task. */
					NULL,                     /* The parameter passed to the task - just to check the functionality. */
					uartQUEUE_TASK_PRIORITY,  /* The priority assigned to the task. */
					NULL );                   /* The task handle is not required, so NULL is passed. */

		/* Start the tasks and timer running. */
		vTaskStartScheduler();
	}

	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then
	there was insufficient FreeRTOS heap memory available for the idle and/or
	timer tasks	to be created.  See the memory management section on the
	FreeRTOS web site for more details. */
	for( ;; );
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

void vApplicationTickHook( void );
void vApplicationTickHook( void )
{
	/* This function will be called by each tick interrupt if
	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
	added here, but the tick hook is called from an interrupt context, so
	code must not attempt to block, and only the interrupt safe FreeRTOS API
	functions can be used (those that end in FromISR()). */
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void );
void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/
