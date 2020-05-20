/*
 * @file    ADC.h
 * @brief   ADC function declarations
 *
 * @author  Liam JA MacDonald
 * @date    19-Dec-2019 (created)
 * @date    19-May-2020 (modified)
 */
#pragma once

#ifndef GLOBAL_ADC
#define GLOBAL_ADC

extern void init_ADC(void);
extern void startADC(void);
extern int returnResult(unsigned int, char*);

#else

#endif /* GLOBAL_ADC */
