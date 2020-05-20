/*
 * @file    HoldingBuffer.h
 * @brief   Contains HoldingBuffer function declaration
 *          and the HoldingBuffer structure definition
 *
 * @author  Liam JA MacDonald
 * @date    24-Sep-2019 (Created)
 * @date    20-May-2020 (Last Modified)
 */
#pragma once
#define MAX_BUFFER 80

/*
 * @brief   Holding Buffer structure
 * @details char buffer[]: array for folding the received
 *                         characters from UART.c
 *          int writePtr:  the index where the next character
 *                         can be added
 */
typedef struct holdingBuffer_
{
    char buffer[MAX_BUFFER];
    int writePtr;
} holdingBuffer;

#ifndef GLOBAL_HOLDINGBUFFER
#define GLOBAL_HOLDINGBUFFER

        extern int addToBuffer_UART(char);
        extern int removeFromBuffer_UART(void);
        extern char* emptyBuffer_UART(void);
        extern int addToBuffer_I2C(char);
        extern int removeFromBuffer_I2C(void);
        extern char* emptyBuffer_I2C(void);
#else

        int addToBuffer_UART(char);
        int removeFromBuffer_UART(void);
        char* emptyBuffer_UART(void);
        int addToBuffer_I2C(char);
        int removeFromBuffer_I2C(void);
        char* emptyBuffer_I2C(void);
#endif// GLOBAL_HOLDINGBUFFER
