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

/* Standard includes. */
#include "string.h"
#include "stdio.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Example includes. */
#include "FreeRTOS_CLI.h"
#include "UARTCommandConsole.h"
#include <libopencm3/cm3/scb.h>

/* Dimensions the buffer into which input characters are placed. */
#define cmdMAX_INPUT_SIZE		50

/* Characters are only ever received slowly on the CLI so it is ok to pass
received characters from the UART interrupt to the task on a queue.  This sets
the length of the queue used for that purpose. */
#define cmdRXED_CHARS_QUEUE_LENGTH			( 10 )

/* DEL acts as a backspace. */
#define cmdASCII_DEL		( 0x7F )

/*-----------------------------------------------------------*/

/*
 * The task that implements the command console processing.
 */
static void prvUARTCommandConsoleTask( void *Parameters );

/*
 * Ensure a previous interrupt driven Tx has completed before sending the next
 * data block to the UART.
 */
static ssize_t prvUARTSendBuffer( uint32_t dev, const char * pcBuffer, size_t xBufferLength );

/*
 * Register the 'standard' sample CLI commands with FreeRTOS+CLI.
 */
extern void vRegisterCLICommands( void );

/*
 * Configure the UART used for IO
 * to handle UART Rx events.
 */
static void prvConsoleConfigureUART( uint32_t dev );

/*-----------------------------------------------------------*/

/* Const messages output by the command console. */
static char * const pcWelcomeMessage = "\r\n\r\nFreeRTOS command server.\r\nType Help to view a list of registered commands.\r\n\r\n>";
static const char * const pcEndOfOutputMessage = "\r\n[Press ENTER to execute the previous command again]\r\n>";
static const char * const pcNewLine = "\r\n";

/*-----------------------------------------------------------*/

void vUARTCommandConsoleStart( uint16_t usStackSize, unsigned portBASE_TYPE uxPriority )
{
	vRegisterCLICommands();

	/* Create that task that handles the console itself. */
	xTaskCreate( 	prvUARTCommandConsoleTask,			/* The task that implements the command console. */
					"CLI",								/* Text name assigned to the task.  This is just to assist debugging.  The kernel does not use this name itself. */
					usStackSize,						/* The size of the stack allocated to the task. */
					NULL,								/* The parameter is not used, so NULL is passed. */
					uxPriority,							/* The priority allocated to the task. */
					NULL );								/* A handle is not required, so just pass NULL. */
}
/*-----------------------------------------------------------*/

static void prvUARTCommandConsoleTask( void *Parameters )
{
char cRxedChar, *pcOutputString;
uint8_t ucInputIndex = 0;
static char cInputString[ cmdMAX_INPUT_SIZE ], cLastInputString[ cmdMAX_INPUT_SIZE ];
portBASE_TYPE xReturned;

	( void ) Parameters;

	/* A UART is used for printf() output and CLI input and output.  Note there
	is no mutual exclusion on the UART, but the demo as it stands does not
	require mutual exclusion. */
	prvConsoleConfigureUART( USART1 );

	/* Obtain the address of the output buffer.  Note there is no mutual
	exclusion on this buffer as it is assumed only one command console
	interface will be used at any one time. */
	pcOutputString = FreeRTOS_CLIGetOutputBuffer();

	/* Send the welcome message. */
	prvUARTSendBuffer( USART1, pcWelcomeMessage, strlen( pcWelcomeMessage ) );

	for( ;; )
	{
		/* Wait for the next character to arrive.  A semaphore is used to
		ensure no CPU time is used until data has arrived. */
		cRxedChar = usart_recv_blocking( USART1 );
			/* Echo the character back. */
			prvUARTSendBuffer( USART1, &cRxedChar, sizeof( cRxedChar ) );

			/* Was it the end of the line? */
			if( cRxedChar == '\n' || cRxedChar == '\r' )
			{
				/* Just to space the output from the input. */
				prvUARTSendBuffer( USART1, pcNewLine, strlen( pcNewLine ) );

				/* See if the command is empty, indicating that the last command is
				to be executed again. */
				if( ucInputIndex == 0 )
				{
					/* Copy the last command back into the input string. */
					strcpy( cInputString, cLastInputString );
				}

				/* Pass the received command to the command interpreter.  The
				command interpreter is called repeatedly until it returns pdFALSE
				(indicating there is no more output) as it might generate more than
				one string. */
				do
				{
					/* Get the next output string from the command interpreter. */
					xReturned = FreeRTOS_CLIProcessCommand( cInputString, pcOutputString, configCOMMAND_INT_MAX_OUTPUT_SIZE );

					/* Write the generated string to the UART. */
					prvUARTSendBuffer( USART1, pcOutputString, strlen( pcOutputString ) );
					if(!strcmp(cInputString,"9"))
						scb_reset_system();
				} while( xReturned != pdFALSE );

				/* All the strings generated by the input command have been sent.
				Clear the input	string ready to receive the next command.  Remember
				the command that was just processed first in case it is to be
				processed again. */
				strcpy( cLastInputString, cInputString );
				ucInputIndex = 0;
				memset( cInputString, 0x00, cmdMAX_INPUT_SIZE );

				prvUARTSendBuffer( USART1, pcEndOfOutputMessage, strlen( pcEndOfOutputMessage ) );
			}
			else
			{
				if( cRxedChar == '\r' )
				{
					/* Ignore the character. */
				}
				else if( ( cRxedChar == '\b' ) || ( cRxedChar == cmdASCII_DEL ) )
				{
					/* Backspace was pressed.  Erase the last character in the
					string - if any. */
					if( ucInputIndex > 0 )
					{
						ucInputIndex--;
						cInputString[ ucInputIndex ] = '\0';
					}
				}
				else
				{
					/* A character was entered.  Add it to the string
					entered so far.  When a \n is entered the complete
					string will be passed to the command interpreter. */
					if( ( cRxedChar >= ' ' ) && ( cRxedChar <= '~' ) )
					{
						if( ucInputIndex < cmdMAX_INPUT_SIZE )
						{
							cInputString[ ucInputIndex ] = cRxedChar;
							ucInputIndex++;
						}
					}
				}
			}
	}
}
/*-----------------------------------------------------------*/

static ssize_t prvUARTSendBuffer( uint32_t dev, const char * pcBuffer, size_t xBufferLength )
{
	int written = 0;

	while( xBufferLength-- > 0 )
	{
		usart_send_blocking(dev, *pcBuffer++);
		written++;
	}
	return written;
}
/*-----------------------------------------------------------*/

static void prvConsoleConfigureUART( uint32_t dev )
{
	/* Setup USART2 parameters. */
	usart_set_baudrate(dev, 115200);
	usart_set_databits(dev, 8);
	usart_set_parity(dev, USART_PARITY_NONE);
	usart_set_stopbits(dev, USART_STOPBITS_1);
	usart_set_mode(dev, USART_MODE_TX_RX);
	usart_set_flow_control(dev, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(dev);
}
/*-----------------------------------------------------------*/
