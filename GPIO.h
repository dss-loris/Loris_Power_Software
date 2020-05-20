/*
 * @file    GPIO.h
 *
 * @brief   System Type enumeration,
 *          Power Mode enumeration,
 *          and GPIO function declarations
 *
 * @author  Liam JA MacDonald
 * @date    31-Dec-2019 (created)
 * @date    20-May-2020 (modified)
 */
#pragma once
enum    subsystemType { OBC, PAYLOAD, RF, ACS};
enum    powerMode {NORMAL,RADIO,PICTURE};

#ifndef GLOBAL_GPIO
#define GLOBAL_GPIO

extern void setInputPins(void);
extern void setOutputPins(void);
extern void setPin(int);
extern void clearPin(int);
extern void enterPowerMode(int);


#endif /* GLOBAL_GPIO */
