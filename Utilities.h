/*
 * @file    Utilities.h
 * @brief   frequently used constants
 *          Utilities.c function prototypes
 * @author  Liam JA MacDonald
 * @date    18-DEC-2019 (created)
 * @date    18-DEC-2019 (modified)
 */
#pragma once

#define     ENTER       0x0d //ASCII Characters
#define     BS          0x08
#define     NUL         0x00
#define     TRUE        1   // For using Integers as booleans
#define     FALSE       0
#define     SUCCESS     1   // To clarify returns values where possible
#define     FAILURE     0
#define     RESET       0   //reset value for all date and time values other then days
#define     EMPTY       0       //Queue return values
#define     FULL       0       //Queue return values
#define     BACKSPACE       "\b \b"     //to replace the backspaced char with whitespace
#define     NEW_LINE  "\n\r"

#ifndef GLOBAL_UTILITIES
#define GLOBAL_UTILITIES

extern int myAtoi(int *, char);


#endif /* GLOBAL_UTILITIES */
