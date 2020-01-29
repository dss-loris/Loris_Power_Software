/*
 * @file    UART.h
 * @details UART Function Prototypes
 * @author  Liam JA MacDonald
 * @date    18-DEC-2019 (created)
 * @date    18-DEC-2019 (modified)
 */
#pragma once

#ifndef GLOBAL_UART
#define GLOBAL_UART

extern void init_UART_A1(void);
extern void force_UART_A1_Output(char);
extern void printString(char*);

#else

#endif /* GLOBAL_UART */
