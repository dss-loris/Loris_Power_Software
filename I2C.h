/*
 * @file    I2C.h
 * @brief   I2C function declarations
 *
 * @author  Liam JA MacDonald
 * @date    14-Feb-2019 (created)
 * @date    20-May-2020 (modified)
 */
#pragma once

#ifndef GLOBAL_I2C
#define GLOBAL_I2C

extern void init_I2C_B0(void);
extern void Transmit(void);
extern void Receive(void);
#else


#endif /* GLOBAL_I2C */
