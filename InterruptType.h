/*
 * @file    InterruptType.h
 * @brief   Defines the interruptType structure
 *          Enumerates Protocols
 */
#pragma once
/*
 * @brief   Interrupt Type structure
 *
 * @details int protocol:   0 UART
 *                          1 I2C
 *                          2 FAULT
 *
 *          char data:      where interrupts store
 *                          data being passed
 */
enum protocolType { UART, I2C, FAULT};

typedef struct interruptType_
{
    int protocol;
    char data;

}interruptType;

