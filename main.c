/*
 * @file    main.c
 *
 * @brief   The entry point of the Loris Satelite
 *          EPS's interrupt driven operating system.
 *
 * @author  Liam JA MacDonald
 * @date    19-Dec-2019 (created)
 * @date    20-May-2020 (modified)
 */
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
  //On start up all the initialization routines are called

  interruptType receivedInterrupt ={{NULL}};
  char* cmd;
  WDTCTL = WDTPW+WDTHOLD;      // Stop watchdog timer
  init_UART_A1();
  init_ADC();
  init_I2C_B0();

  setOutputPins();
  setInputPins();

  __bis_SR_register(GIE);

  //Checks the interrupt queue, if empty,
  //MSP enters low power mode
  while(1)
  {

      if(dequeue(INPUT, &receivedInterrupt))
      {
          //this will need to be altered for the operational version of the code

         switch(receivedInterrupt.protocol)
                      {
                      case UART :                       //adds UART characters to
                       switch(receivedInterrupt.data)   //holding buffer until ENTER char
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
