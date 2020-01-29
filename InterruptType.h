/*
 * @file    InterruptType.h
 * @brief   Defines the interruptType structure
 */
#pragma once
/*
 * @brief   Interrupt Type structure
 * @details int type:       0 is a INPUT interrupt
 *                          1 is a OUTPUT interrupt
 *
 *          int protocol:   0 UART
 *                          1 I2C
 *
 *          char data:      where interrupts store
 *                          data being passed
 */
enum protocolType { UART, I2C};

typedef struct interruptType_
{
    int protocol;
    char data;

}interruptType;
