/*
 * @file    HoldingBuffer.c
 * @brief   Defines the holding buffer used for
 *          holding characters received via UART
 *          and I2C serial communication protocols
 *
 * @author  Liam JA MacDonald
 * @date    24-Sep-2019 (Created)
 * @date    20-May-2020 (Modified)
 */

#include "Utilities.h"
#define GLOBAL_HOLDINGBUFFER
#include "HoldingBuffer.h"

/*holding buffer for UART and I2C*/
static holdingBuffer    holdingBuffer_UART = {{EMPTY},EMPTY};
static holdingBuffer    holdingBuffer_I2C = {{EMPTY},EMPTY};


/*
 * @brief   Adds a character to UART holding register
 *
 * @param   [in] char c: character to be added to holding register
 *
 * @return  int return used as a boolean value,
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
 * @brief   Removes a character from UART holding register
 *
 * @return  int return used as a boolean value,
 *          if returns 1 the character was successfully removed
 *          if returns 0 the holding buffer is empty
 */
int removeFromBuffer_UART(void)
{
    if(holdingBuffer_UART.writePtr>EMPTY)
    {
            holdingBuffer_UART.buffer[holdingBuffer_UART.writePtr] = NUL;
            holdingBuffer_UART.writePtr--;
            return SUCCESS;
    }
    return EMPTY;
}

/*
 * @brief   Adds a NUL character to the end of
 *          the holding register for UART, returns a pointer
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

/*
 * @brief   Adds a character to I2C holding register
 *
 * @param   [in] char c: character to be added to holding register
 *
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
 * @brief   Removes a character from I2C holding register
 *          *FUNCTION MAY NOT BE NEEDED*
 *
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
 * @brief   This function will be used for passing the contents
 *          of the I2C holding buffer when a reception is complete
 *          *THIS WILL CHANGE WHEN I2C RECPTION FUNTIONALITY IS ADDED*
 *
 * @return  char*: pointer to the first character in the
 *          holding register
 */
char* emptyBuffer_I2C(void)
{
   addToBuffer_I2C(NUL);
   holdingBuffer_I2C.writePtr=EMPTY;
   return holdingBuffer_I2C.buffer;
}
