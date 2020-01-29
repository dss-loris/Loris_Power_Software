/*
 * GPIO.c
 *
 *  Created on: Dec 31, 2019
 *      Author: LiamMacDonald
 */
#define GLOBAL_GPIO
#include "GPIO.h"
#include "UART.h"
#include <msp430.h>

void setOutputPins(void)
{
    P8DIR |= BIT1+BIT2;             // P1.0 set as output
    P3DIR |= BIT7;
    P4DIR |= BIT0;
}

void setPin(int pin)
{
    switch (pin)
    {
    case OBC:
        printString("OBC ");
        P8OUT |= BIT1;
        break;
    case PAYLOAD:
        printString("PAYLOAD ");
        P8OUT |= BIT2;
        break;
    case RF:
        printString("RF ");
        P3OUT |= BIT7;
        break;
    case ACS:
        printString("ACS ");
        P4OUT |= BIT0;
        break;
    }
}

void clearPin(int pin)
{
    switch (pin)
    {
        case OBC:
            printString("OBC ");
            P8OUT &= ~BIT1;
            break;
        case PAYLOAD:
            printString("PAYLOAD ");
            P8OUT &= ~BIT2;
            break;
        case RF:
            printString("RF ");
            P3OUT &= ~BIT7;
            break;
        case ACS:
            printString("ACS ");
            P4OUT &= ~BIT0;
            break;
      }
}


void enterPowerMode(int mode)
{
    switch (mode)
    {
    case RADIO:
        printString("RADIO ");
        P8OUT &= ~BIT2;
        P8OUT |= BIT1;
        P4OUT |= BIT0;
        P3OUT |= BIT7;
        break;
    case PICTURE:
        printString("PICTURE ");
        P3OUT &= ~BIT7;
        P8OUT |= BIT1;
        P4OUT |= BIT0;
        P8OUT |= BIT2;
        break;
    case NORMAL:
        printString("NORMAL ");
        P8OUT &= ~BIT2;
        P8OUT |= BIT1;
        P4OUT |= BIT0;
        P3OUT &= ~BIT7;
        break;
    }
}


