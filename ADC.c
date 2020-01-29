/*
 * ADC.c
 *
 *  Created on: Dec 19, 2019
 *      Author: LiamMacDonald
 */
#define GLOBAL_ADC
#include "ADC.h"
#include "Utilities.h"
#include <stdio.h>
#include <msp430.h>

volatile static double results[2];

void init_ADC(void)
{
    REFCTL0 &= ~ REFON;
    P6SEL = 0x0F;                             // Enable A/D channel inputs
    ADC12CTL0 = ADC12ON + ADC12MSC + ADC12SHT0_2 + ADC12REFON; // Turn on ADC12, set sampling time
    ADC12CTL1 = ADC12CSTARTADD_0 + ADC12SHP + ADC12CONSEQ_1; // Use sampling timer, single sequence
    ADC12CTL2 = ADC12RES_2;
    ADC12MCTL0 = ADC12INCH_2;                 // ref+=AVcc, channel = A0
    ADC12MCTL1 = ADC12INCH_1+ ADC12EOS;                 // ref+=AVcc, channel = A1
    ADC12IE =   ADC12IE1;                           // Enable ADC12IFG.3
    ADC12CTL0 |= ADC12ENC;                    // Enable conversions
}

void startADC(void)
{
    ADC12CTL0 |= ADC12ENC;
    ADC12CTL0 |= ADC12SC;                   // Start convn - software trigger
    __delay_cycles(1000);
}

int returnResult(unsigned int subsystem, char* toReturn)
{
    double resultValue;

    if(subsystem<2)
    {
        resultValue = 3.333*(results[subsystem]/4095);
        sprintf(toReturn,"%.3f",resultValue);
        return SUCCESS;
    }

    return FAILURE;

}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(ADC12IV,34))
  {
  case  0: break;                           // Vector  0:  No interrupt
  case  2: break;                           // Vector  2:  ADC overflow
  case  4: break;                           // Vector  4:  ADC timing overflow
  case  6: break;                           // Vector  6:  ADC12IFG0
  case  8:                           // Vector  8:  ADC12IFG1
      results[0] = ADC12MEM0;                 // Move results, IFG is cleared
      results[1] = ADC12MEM1;                 // Move results, IFG is cleared
      ADC12CTL0 &= ~ADC12SC;
      ADC12CTL0 &= ~ADC12ENC;
      break;
  case 10: break;                           // Vector 10:  ADC12IFG2
  case 12: break;                                 // Vector 12:  ADC12IFG3
  case 14: break;                           // Vector 14:  ADC12IFG4
  case 16: break;                           // Vector 16:  ADC12IFG5
  case 18: break;                           // Vector 18:  ADC12IFG6
  case 20: break;                           // Vector 20:  ADC12IFG7
  case 22: break;                           // Vector 22:  ADC12IFG8
  case 24: break;                           // Vector 24:  ADC12IFG9
  case 26: break;                           // Vector 26:  ADC12IFG10
  case 28: break;                           // Vector 28:  ADC12IFG11
  case 30: break;                           // Vector 30:  ADC12IFG12
  case 32: break;                           // Vector 32:  ADC12IFG13
  case 34: break;                           // Vector 34:  ADC12IFG14
  default: break;
  }
}
