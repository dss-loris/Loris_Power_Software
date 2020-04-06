/*
 * I2C.c
 *
 *  Created on: Feb 14, 2020
 *      Author: LiamMacDonald
 */

#define GLOBAL_I2C
#include "I2C.h"
#include <msp430.h>
#include "msp430f5529.h"
#include "InterruptType.h"
#include "Queue.h"
#include "Utilities.h"

#define CC_ADDRESS 0x64


void init_I2C_B0(void){

    P3SEL |= BIT0+BIT1;                       // P3.0,1 = USCI_B0 SDA/SCL
    UCB0CTL1 |= UCSWRST;                      // Enable SW reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
    UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
    UCB0IE |= UCRXIE | UCTXIE | UCNACKIE;               //Enable RX and TX interrupt
}

void Transmit(void){
    int i=0;
    UCB0I2CSA = CC_ADDRESS;
    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
    UCB0CTL1 |= UCTR + UCTXSTT;             // I2C Control Register
    for(i=0;i<10;i++);


}
void Receive(void){
        while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent. Its automatically cleared once sent
        UCB0CTL1 &= ~UCTR ;                     // Clear UCTR
        UCB0CTL1 |= UCTXSTT;                    // I2C start condition
        while (UCB0CTL1 & UCTXSTT);             // Start condition sent?
        UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
}

#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
    static interruptType inputI2C = {I2C,NUL};
    static interruptType outputI2C = {I2C,NUL};
    int i=0;

    switch(__even_in_range(UCB0IV,12))
      {
      case  0: break;                           // Vector  0: No interrupts
      case  2: break;                           // Vector  2: ALIFG
      case  4:                            // Vector  4: NACKIFG
        UCB0CTL1 |= UCTXSTP;
        UCB0IFG &= ~UCTXIFG;
        break;
      case  6: break;                           // Vector  6: STTIFG
      case  8: break;                           // Vector  8: STPIFG
      case 10:                            // Vector 10: RXIFG
        inputI2C.data = UCB0RXBUF;      // USCI_A0 TX buffer ready?
        enqueue(INPUT, inputI2C);                // TX -> RXed character
        break;
      case 12:                                  // Vector 12: TXIFG
        if (getOutputCountI2C())
        {
            dequeue(OUTPUT_I2C, &outputI2C);
            UCB0TXBUF = outputI2C.data;
            for(i=0;i<10;i++);
        }
        else
        {
            UCB0CTL1 |= UCTXSTP;// I2C stop condition
            UCB0IFG &= ~UCTXIFG;
        }
        break;
      default: break;
      }

}
