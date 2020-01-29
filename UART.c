/*
 * @file    UART.c
 * @details Contains initialization routines to set
 *          a UART interrupts for transmission and receive.
 *          Definition of the UART ISR
 * @author  Liam JA MacDonald
 * @date    18-DEC-2019 (created)
 * @date    18-DEC-2019 (modified)
 */
#define GLOBAL_UART
#include "UART.h"
#include <msp430.h>
#include "InterruptType.h"
#include "Queue.h"
#include "Utilities.h"

static interruptType inputUART = {UART,NUL};
static interruptType outputUART = {UART,NUL};
/*
 * @brief initialize UART_A1
 *        with BAUD-RATE:       115200
 *             Data Bits:       8
 *             Parity Bits:     0
 *             Stop Bits:       1
 */
void init_UART_A1(void)
{
  P4SEL |= BIT4+BIT5;                       // P3.3,4 = USCI_A0 TXD/RXD
  UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA1CTL1 |= UCSSEL_2;                     // SMCLK
  UCA1BR0 = 9;                              // 1MHz 115200 (see User's Guide)
  UCA1BR1 = 0;                              // 1MHz 115200
  UCA1MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
  UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCA1IE |= UCRXIE;                         // Enable USCI_A1 RX interrupt
  UCA1IE |= UCTXIE;                         // Enable USCI_A1 TX interrupt
}

/*
 * @brief   Force character into the data register
 * @param   [in] char data: character to be put into
 *          data register
 */
void force_UART_A1_Output(char data)
{
        while(UCA1STAT&UCBUSY);//wait until not busy
        UCA1TXBUF = data;
}

/*
 * @brief   Enqueues a complete string onto the OUTPUT
 *          queue
 * @param   [in] char* string: string to be queued
 * @details enqueues one character at a time until
 *          nul character
 */

void printString(char* string)
{
    interruptType out ={UART, NUL};
    while(*string)
    {
        out.data =*(string++);
        enqueue( OUTPUT,out);
    }
}

// Echo back RXed character, confirm TX buffer is ready first
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A1_VECTOR))) USCI_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
      inputUART.data = UCA1RXBUF;      // USCI_A0 TX buffer ready?
      enqueue(INPUT, inputUART);                // TX -> RXed character
      break;
  case 4:
      if(dequeue( OUTPUT, &outputUART))
      {
          force_UART_A1_Output(outputUART.data);
      }
      break;                             // Vector 4 - TXIFG
  default: break;
  }
}




