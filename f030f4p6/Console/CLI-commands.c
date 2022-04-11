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

 /******************************************************************************
 *
 * See the following URL for information on the commands defined in this file:
 * https://www.FreeRTOS.org/FreeRTOS-Plus/FreeRTOS_Plus_UDP/Embedded_Ethernet_Examples/Ethernet_Related_CLI_Commands.shtml
 *
 ******************************************************************************/


/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* FreeRTOS+CLI includes. */
#include "FreeRTOS_CLI.h"
#include <libopencm3/stm32/gpio.h>

/*
 * Implements the command.
 */
static portBASE_TYPE prvTaskResetCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static portBASE_TYPE prvTaskRestartEVMCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static portBASE_TYPE prvTaskTurnOffEVMCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

/* Structure that defines the "task-stats" command line command.  This generates
a table that gives information on each task in the system. */
static const CLI_Command_Definition_t xTaskReset =
{
	"9", /* The command string to type. */
	"\r\n9:\r\n Restart myself\r\n",
	prvTaskResetCommand, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xTaskRestartEVM =
{
    "1", /* The command string to type. */
    "\r\n1:\r\n Restart EVM board\r\n",
    prvTaskRestartEVMCommand, /* The function to run. */
    0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xTaskTurnOffEVM =
{
    "2", /* The command string to type. */
    "\r\n2:\r\n Turn off EVM board\r\n",
    prvTaskTurnOffEVMCommand, /* The function to run. */
    0 /* No parameters are expected. */
};

/*-----------------------------------------------------------*/

void vRegisterCLICommands( void );
void vRegisterCLICommands( void )
{
	/* Register all the command line commands defined immediately above. */
    FreeRTOS_CLIRegisterCommand( &xTaskRestartEVM );
    FreeRTOS_CLIRegisterCommand( &xTaskTurnOffEVM );
    FreeRTOS_CLIRegisterCommand( &xTaskReset );
}
/*-----------------------------------------------------------*/

static portBASE_TYPE prvTaskResetCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
const char *const pcHeader = "Command 9#:\tI am restarting ...\r\n************************************************\r\n";

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	/* Generate a table of task stats. */
	strcpy( pcWriteBuffer, pcHeader );

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}

static portBASE_TYPE prvTaskRestartEVMCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
const char *const pcHeader = "Command 1#:\tEVM Board restarting ...\r\n************************************************\r\n";

    /* Remove compile time warnings about unused parameters, and check the
    write buffer is not NULL.  NOTE - for simplicity, this example assumes the
    write buffer length is adequate, so does not check for buffer overflows. */
    ( void ) pcCommandString;
    ( void ) xWriteBufferLen;
    configASSERT( pcWriteBuffer );

    /* Generate a table of task stats. */
    strcpy( pcWriteBuffer, pcHeader );

    gpio_toggle(GPIOA, GPIO0);
    vTaskDelay(800 / portTICK_PERIOD_MS);
    gpio_toggle(GPIOA, GPIO0);

    /* There is no more data to return after this single string, so return
    pdFALSE. */
    return pdFALSE;
}

static portBASE_TYPE prvTaskTurnOffEVMCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
const char *const pcHeader = "Command 2#:\tEVM Board turning off ...\r\n************************************************\r\n";

    /* Remove compile time warnings about unused parameters, and check the
    write buffer is not NULL.  NOTE - for simplicity, this example assumes the
    write buffer length is adequate, so does not check for buffer overflows. */
    ( void ) pcCommandString;
    ( void ) xWriteBufferLen;
    configASSERT( pcWriteBuffer );

    /* Generate a table of task stats. */
    strcpy( pcWriteBuffer, pcHeader );

    // gpio_clear(GPIOA, GPIO1);
    gpio_set(GPIOA, GPIO1);

    /* There is no more data to return after this single string, so return
    pdFALSE. */
    return pdFALSE;
}

/*-----------------------------------------------------------*/

