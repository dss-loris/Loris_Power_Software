/*
 * GPIO.h
 *
 *  Created on: Dec 31, 2019
 *      Author: LiamMacDonald
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
