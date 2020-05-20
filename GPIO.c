/*
 * @file    GPIO.c
 *
 * @brief   Input and Output pin initialization
 *          functions are defined. The setPin,
 *          clearPin, and enterPowerMode functions
 *          called in the CommandTable.c file are
 *          defined
 *
 * @author  Liam JA MacDonald
 * @date    31-Dec-2019 (created)
 * @date    20-May-2020 (modified)
 */
#define GLOBAL_GPIO
#include "GPIO.h"
#include "UART.h"
#include "Utilities.h"
#include "InterruptType.h"
#include "Queue.h"
#include <msp430.h>

static interruptType faultInt = {FAULT,NUL};

/*
 * @brief   Initializes the MSP430 GPIO module control
 *          registers to configure output pins:
 *          8.1 (OBC)
 *          8.2 (PAYLOAD)
 *          3.7 (RF)
 *          4.0 (ACS)
 *
 *          Set pins corresponding to OBC and ACS so the
 *          EPS is initially in Normal power mode
 *
 */
void setOutputPins(void)
{
    P8DIR |= BIT1+BIT2;             // P1.0 set as output
    P3DIR |= BIT7;
    P4DIR |= BIT0;

    P8OUT |= BIT1;      //Initially in NORMAL power mode
    P4OUT |= BIT0;

}

/*
 * @brief   Initializes the MSP430 GPIO module control
 *          registers to configure input pins:
 *          2.2 (RF Fault)
 *
 *          It enables interrupts on these pins, which
 *          signify a fault condition occurred in the
 *          corresponding subsystem
 */
void setInputPins(void)
{
    P2DIR &= ~BIT2;             // P2.2 set as input
    P2REN &= ~BIT2;
    P2IES &= ~BIT2;
    P2IFG &= ~BIT2;
    P2IE &= ~0xFF; // ensure only pin 2 is interupt enabled
    P2IE |= BIT2;
}
/*
 * @brief Sets Output pin corresponding to the
 *        pin argument high, effectively enabling
 *        a subsystem. Should only be called explicitly
 *        to enable a subsystem after a fault condition.
 *
 * @param [in] int pin:
 *        subsystem to be enabled
 */
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
/*
 * @brief Clear Output pin corresponding to the
 *        pin argument, effectively disabling
 *        a subsystem. Shouldn't be called
 *        explicitly, will be made private in
 *        operational code
 *
 * @param [in] int pin:
 *        subsystem to be disabled
 */
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
/*
 * @brief Sets Output pins according to the power mode requested.
 *
 * @param [in] int mode:
 *        Desired Power mode
 */
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
/*
 * @brief   GPIO ISR.
 *          When a fault condition is triggered at an input port,
 *          execution enters the ISR to send a UART packet to the
 *          OBC to inform it of a fault condition.
 *
 */
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



