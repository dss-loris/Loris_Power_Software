/*
 * @file    HoldingBuffer.c
 * @brief   Defines the holding buffer used for
 *          holding characters before the ENTER
 *          character is received; contains all
 *          functionality to use the holding buffer
 * @author  Liam JA MacDonald
 * @date    24-Sep-2019 (Created)
 * @date    9-Oct-2019 (Modified)
 */

#include "Utilities.h"
#define GLOBAL_HOLDINGBUFFER
#include "HoldingBuffer.h"
/* Define an empty holding buffer*/
static holdingBuffer    holdingBuffer_UART = {{EMPTY},EMPTY};
static holdingBuffer    holdingBuffer_I2C = {{EMPTY},EMPTY};

/*UART0 (TERMINAL) holding buffer, holds user train command*/

/*
 * @brief   Adds a character to UART 0's holding register
 * @param   char c: character to be added to holding register
 * @return  @return  int return used as a boolean value,
 *          if returns 1 the character was successfully added
 *          if returns 0 the holding buffer is full
 */
int addToBuffer_UART(char c)
{
    if(holdingBuffer_UART.writePtr<MAX_BUFFER)
    {
        holdingBuffer_UART.buffer[holdingBuffer_UART.writePtr++] = c;
        return SUCCESS;
    }
    return FULL;
}

/*
 * @brief   Removes a character from UART 0's holding register
 * @return  int return used as a boolean value,
 *          if returns 1 the character was successfully removed
 *          if returns 0 the holding buffer is empty
 */
int removeFromBuffer_UART(void)
{
    if(holdingBuffer_UART.writePtr>EMPTY)
    {
            holdingBuffer_UART.writePtr--;
            return SUCCESS;
    }
    return EMPTY;
}

/*
 * @brief   Adds a NUL character to the end of
 *          the holding register for UART 0 , returns a pointer
 *          to the start
 * @return  char*: pointer to the first character in the
 *          holding register
 */
char* emptyBuffer_UART(void)
{
   addToBuffer_UART(NUL);
   holdingBuffer_UART.writePtr=EMPTY;
   return holdingBuffer_UART.buffer;
}

/*UART1 (Trainset) holding buffer, receives command from trainset*/
/*
 * @brief   Adds a character to UART1's holding register
 * @param   char c: character to be added to holding register
 * @return  @return  int return used as a boolean value,
 *          if returns 1 the character was successfully added
 *          if returns 0 the holding buffer is full
 */
int addToBuffer_I2C(char c)
{
    if(holdingBuffer_I2C.writePtr<MAX_BUFFER)
    {
        holdingBuffer_I2C.buffer[holdingBuffer_I2C.writePtr++] = c;
        return SUCCESS;
    }
    return FULL;
}

/*
 * @brief   Removes a character from UART1's holding register
 * @return  int return used as a boolean value,
 *          if returns 1 the character was successfully removed
 *          if returns 0 the holding buffer is empty
 */
int removeFromBuffer_I2C(void)
{
    if(holdingBuffer_I2C.writePtr>EMPTY)
    {
            holdingBuffer_I2C.writePtr--;
            return SUCCESS;
    }
    return EMPTY;
}

/*
 * @brief   Adds a NUL character to the end of
 *          the holding register UART1's, returns a pointer
 *          to the start
 * @return  char*: pointer to the first character in the
 *          holding register
 */
char* emptyBuffer_I2C(void)
{
   addToBuffer_I2C(NUL);
   holdingBuffer_I2C.writePtr=EMPTY;
   return holdingBuffer_I2C.buffer;
}
