/*
 * @file    Queue.c
 * @brief   Defines interrupt queues
 *          Contains enqueue and dequeue functionality
 *          The ISRs use these queues to minimize the
 *          amount of time in the ISR. The main function
 *          then dequeues from the Input interrupt queue
 *
 * @author  Liam JA MacDonald
 * @date    23-Sep-2019 (Created)
 * @date    10-Oct-2019 (Modified)
 */
#include <string.h>
#define GLOBAL_QUEUES
#include "Queue.h"
#include "Utilities.h"
#include "UART.h"
#include "HoldingBuffer.h"

/*  Declare queue array for interrupts
 *
 *  INPUT (0),
 *  OUTPUT_UART (1),
 *  OUTPUT_I2C (2)
 *
 */
static queue interruptQueues[NUMBER_OF_QUEUES] =
    {{{RESET},RESET,RESET},{{RESET},RESET,RESET},{{RESET},RESET,RESET}};

static int outputCountI2C = 0;

int getOutputCountI2C(void)
{
    return outputCountI2C;
}
/*
 * @brief   Adds an interrupt to an interrupt queue.
 * @param   [in] int queueType: specifies which queue to add to
 *                              INPUT (0); OUTPUT(1)
 *          [in] interruptType it: incoming interrupt
 * @return  int return Used as a boolean value,
 *          if returns 1; interrupt successfully queued.
 *          if returns 0; queue full or empty.
 * @detail  if it's for OUTPUT queue and it's empty
 *          the data is forced out to the screen
 *          interrupts are disabled before changing the
 *          write pointer to avoid race conditions
 */
int enqueue(int queueType, interruptType it)
{
    if((queueType==OUTPUT_UART)&&(interruptQueues[queueType].writePtr==interruptQueues[queueType].readPtr))
    {   //buffer is empty
        force_UART_A1_Output(it.data);
        return EMPTY;
    }else if(queueType==OUTPUT_I2C)
    {
        outputCountI2C++;
    }

    /* gives circular queue functionality*/
    unsigned int tmpPtr = (interruptQueues[queueType].writePtr+1)&(MAX_QUEUE_SIZE-1);
    if((tmpPtr == interruptQueues[queueType].readPtr)){return FULL;}
    /* put character in queue and increment write ptr */
    interruptQueues[queueType].fifo[interruptQueues[queueType].writePtr].data = it.data;
    interruptQueues[queueType].fifo[interruptQueues[queueType].writePtr].protocol = it.protocol;
    interruptQueues[queueType].writePtr = tmpPtr;

    return SUCCESS;
}

/*
 * @brief   Removes an interrupt from an interrupt queue.
 * @param   [in] int queueType: specifies which queue to add to
 *                              INPUT (0); OUTPUT(1)
 *          [in] interruptType it: incoming interrupt
 * @return  int return Used as a boolean value,
 *          if returns 1; interrupt successfully queued.
 *          if returns 0; queue full or empty.
 * @detail  if it's for OUTPUT queue and it's empty
 *          the data is forced out to the screen
 *          interrupts are disabled before changing the
 *          write pointer to avoid race conditions
 */
int dequeue( int queueType, interruptType* it)
{
    if((interruptQueues[queueType].writePtr == interruptQueues[queueType].readPtr))//buffer is empty
    {
        return EMPTY;
    }else if(queueType==OUTPUT_I2C)
    {
        outputCountI2C--;
    }
    it->data = interruptQueues[queueType].fifo[interruptQueues[queueType].readPtr].data;
    it->protocol = interruptQueues[queueType].fifo[interruptQueues[queueType].readPtr].protocol;
    /* gives circular queue functionality*/
    interruptQueues[queueType].readPtr=(interruptQueues[queueType].readPtr+1)&(MAX_QUEUE_SIZE-1);

    return SUCCESS;
}



