/*
 * GPIO.c
 *
 *  Created on: Dec 31, 2019
 *      Author: LiamMacDonald
 */
#define GLOBAL_GPIO
#include "GPIO.h"
#include "UART.h"
#include "Utilities.h"
#include "InterruptType.h"
#include "Queue.h"
#include <msp430.h>

static interruptType faultInt = {FAULT,NUL};

void setOutputPins(void)
{
    P8DIR |= BIT1+BIT2;             // P1.0 set as output
    P3DIR |= BIT7;
    P4DIR |= BIT0;

    P8OUT |= BIT1;      //Initially in NORMAL power mode
    P4OUT |= BIT0;

}

void setInputPins(void)
{
    P2DIR &= ~BIT2;             // P2.2 set as input
    P2REN &= ~BIT2;
    P2IES &= ~BIT2;
    P2IFG &= ~BIT2;
    P2IE &= ~0xFF; // ensure only pin 2 is interupt enabled
    P2IE |= BIT2;
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
    case NORMAL:
        printString("NORMAL ");
        P8OUT &= ~BIT2;
        P8OUT |= BIT1;
        P4OUT |= BIT0;
        P3OUT &= ~BIT7;
        break;
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
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void Port_2 (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12ISR (void)
#else
#error Compiler not supported!
#endif
{
    enqueue(INPUT, faultInt);                // TX -> RXed character
    P2IFG &= ~BIT2;
}



