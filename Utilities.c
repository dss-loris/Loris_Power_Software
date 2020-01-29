/*
 * @file    Utilities.c
 * @brief   Contains functions frequently used
 *          over several modules used to format
 *          convert values and printing multiple
 *          characters in varying formats
 * @author  Liam JA MacDonald
 * @date    18-DEC-2019 (created)
 * @date    18-DEC-2019 (modified)
 */

#define GLOBAL_UTILITIES
#include "Utilities.h"

/*
 * @brief   converts ascii to decimal value
 * @param   [in] char* str: string that will be converted from ascii
 *          [out] int* num: integer passed reference to store decimal value
 * @return  int return used as a boolean value,
 *          if returns 1 string was successfully converted
 *          if returns 0 characters were not digits or the string
 *          too long
 */
int myAtoi(int * num, char str)
{
    int total = 0;

        if((str>='0')&&(str<='9'))
        {
          total= str-'0';
        }
        else
        {return FAILURE;}

    *num = total;
    return SUCCESS;
}

