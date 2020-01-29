/*
 * @file    CommandTable.c
 * @brief   Defines the commandTable.
 *          Contains functionality to identify command
 *          passed from holding buffer and call its respective
 *          function with arguments
 * @author  Liam JA MacDonald
 * @date    24-Sep-2019 (created)
 * @date    9-Oct-2019 (modified)
 */

#include <string.h>
#define GLOBAL_COMMANDTABLE
#include "CommandTable.h"
#include "UART.h"
#include "Utilities.h"
#include "GPIO.h"
#include "ADC.h"

#define TURN_ON '0'
#define TURN_OFF '1'
#define POWER_MODE '2'
#define DATA_REQUEST '3'






/*
 * @brief   Match command string and pass arguments to it's
 *          intended function
 * @param   [in, out] cmd: string passed from holding buffer.
 *          chars following the command string passed to the
 *          respective function as a parameter
 * @return  int return used as a boolean value,
 *          returns 1 the command was matched and executed successfully;
 *          if 0 it wasn't matched or execution failed
 * @details the cmd strings first 4 or 5 characters are compared
 *          to the commandType strings in the command table
 *          if matched the chars following the command string
 *          are passed to the respective function
 *
 */
void process( char* serializedCommand)
{
    int pinNumber;
    int powerMode;
    char voltOBC[6];
    char voltPayload[6];
    deserialize command;

    command.input = serializedCommand;

    switch(command.command->type)
    {
    case TURN_ON:

        printString("\n\rEnable ");
        myAtoi(&pinNumber, command.command->argument);
        setPin(pinNumber);

        break;
    case TURN_OFF:

        printString("\n\rDisable ");
        myAtoi(&pinNumber, command.command->argument);
        clearPin(pinNumber);

        break;
    case POWER_MODE:

        printString("\n\rEntering ");
        myAtoi(&powerMode, command.command->argument);
        enterPowerMode(powerMode);
        printString("Power Mode ");

        break;
    case DATA_REQUEST:

        startADC();

        returnResult(OBC,voltOBC);
        returnResult(PAYLOAD,voltPayload);
        printString(NEW_LINE);
        printString("Voltage OBC: ");
        printString(voltOBC);
        printString(NEW_LINE);
        printString("Voltage PAYLOAD: ");
        printString(voltPayload);
        printString(NEW_LINE);

        break;
    default:
        printString("Process Fail");
    }

}





