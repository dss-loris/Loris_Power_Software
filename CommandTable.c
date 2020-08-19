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
#include "Queue.h"
#include "GPIO.h"
#include "InterruptType.h"
#include "ADC.h"
#include "I2C.h"

#define TURN_ON '0'
#define TURN_OFF '1'
#define POWER_MODE '2'
#define DATA_REQUEST '3'
#define COULOMB_COUNT_INIT '4'//testing will eventually part of Data request
#define COULOMB_COUNT '5'
#define DEBUG_MENU '6'


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

#define CC_CONTROL_R 0x01
#define CC_CONTROL_VALUE 0x3c

static interruptType cc_register = {I2C,NUL};
static interruptType cc_value = {I2C,NUL};

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
    case COULOMB_COUNT_INIT:

        printString("\n\rConfigure LTC2942\n\r ");
        printString("Control_R: DEFAULT\n\r ");
        cc_register.data = CC_CONTROL_R;
        cc_value.data = CC_CONTROL_VALUE;
        enqueue(OUTPUT_I2C, cc_register);
        enqueue(OUTPUT_I2C, cc_value);
        Transmit();

        break;
    case COULOMB_COUNT:

        printString("\n\rReceive from LTC2942\n\r ");
        cc_register.data = CC_CONTROL_R;
        enqueue(OUTPUT_I2C, cc_register);
        Receive();

        break;
    case DEBUG_MENU:

        printString("\n\rCOMMANDS:\n\r\n");

        printString("ENABLE = 0\n\r");
        printString("ARGUMENTS = 0 (OBC) | 1(PAYLOAD) | 2(RF) | 3(ACS)\n\r\n");

        printString("DISABLE = 1\n\r");
        printString("ARGUMENTS = 0 (OBC) | 1(PAYLOAD) | 2(RF) | 3(ACS)\n\r\n");

        printString("POWER MODE = 2\n\r");
        printString("ARGUMENTS = 0 (NORMAL) | 1(RADIO) | 2(PICTURE)\n\r\n");

        printString("MEASURE LINE LEVEL = 3\n\r");
        printString("ARGUMENTS = 0 (OBC & PAYLOAD VOLATAGE)\n\r");
        printString("**FOR TESTING**\n\r");

        printString("CONFIGURE LTC2942 CC = 4\n\r");
        printString("ARGUMENTS = 0 (CONTROL REGISTER; NORMAL MODE)\n\r");
        printString("**FOR TESTING**\n\r\n");

        printString("Print Commands for debugging = 5\n\r\n");

        printString("ENTER AS: (command)(argument)\n\r\n");

        break;
    default:
        printString("Process Fail");
    }

}





