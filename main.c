/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430F552x Demo - ADC12, Sequence of Conversions (non-repeated)
//
//  Description: This example shows how to perform A/D conversions on a sequence
//  of channels. A single sequence of conversions is performed - one conversion
//  each on channels A0, A1, A2, and A3. Each conversion uses AVcc and AVss for
//  the references. The conversion results are stored in ADC12MEM0, ADC12MEM1,
//  ADC12MEM2, and ADC12MEM3 respectively and are moved to 'results[]' upon
//  completion of the sequence. Test by applying voltages to pins A0, A1, A2,
//  and A3, then setting and running to a break point at the "_BIC..."
//  instruction in the ISR. To view the conversion results, open a watch window
//  in debugger and view 'results' or view ADC12MEM0, ADC12MEM1, ADC12MEM2, and
//  ADC12MEM3 in an ADC12 SFR window.
//  This can run even in LPM4 mode as ADC has its own clock
//  Note that a sequence has no restrictions on which channels are converted.
//  For example, a valid sequence could be A0, A3, A2, A4, A2, A1, A0, and A7.
//  See the MSP430x5xx User's Guide for instructions on using the ADC12.
//
//               MSP430F552x
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//    Vin0 -->|P6.0/CB0/A0      |
//    Vin1 -->|P6.1/CB1/A1      |
//    Vin2 -->|P6.2/CB2/A2      |
//    Vin3 -->|P6.3/CB3/A3      |
//            |                 |
//
//   Bhargavi Nisarga
//   Texas Instruments Inc.
//   April 2009
//   Built with CCSv4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************
#include <msp430.h>
#include <string.h>
#include <ctype.h>
#include "UART.h"
#include "ADC.h"
#include "Queue.h"
#include "HoldingBuffer.h"
#include "InterruptType.h"
#include "CommandTable.h"
#include "Utilities.h"
#include "GPIO.h"
#include "I2C.h"


int main(void)
{
  interruptType receivedInterrupt ={{NULL}};
  char* cmd;
  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer
  init_UART_A1();
  init_ADC();
  init_I2C_B0();

  setOutputPins();
  setInputPins();

  __bis_SR_register(GIE);

  printString("COMMANDS:\n\r\n");

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
  printString("ARGUMENTS = 0 (CONTROL REGISTER; NORMAL MODE)\n\r\n");
  printString("**FOR TESTING**\n\r");

  printString("ENTER AS: (command)(argument)\n\r\n");


  while(1)
  {

      if(dequeue(INPUT, &receivedInterrupt))
      {

         switch(receivedInterrupt.protocol)
                      {
                      case UART :
                       switch(receivedInterrupt.data)
                          {
                          case ENTER:

                              cmd = emptyBuffer_UART();
                              process(cmd);
                              printString(NEW_LINE);
                              printString(NEW_LINE);

                              break;
                          case BS:

                              if(removeFromBuffer_UART()==SUCCESS){printString(BACKSPACE);}

                              break;
                          default:
                              if(addToBuffer_UART(toupper(receivedInterrupt.data))==SUCCESS)
                              {
                                  enqueue(OUTPUT_UART,receivedInterrupt);
                              }

                          }
                          break;
                      case I2C:
                          break;
                      case FAULT:
                          printString("FAULT ON RF LINE\n\r");

                      default:
                          break;
                      }
      }
      else
      {
          //__bis_SR_register(GIE);     // Enter LPM4, Enable interrupts
//          __no_operation();                       // For debugger
      }
  }
}
