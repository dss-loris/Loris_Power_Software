/*
 * @file    CommandTable.h
 *
 * @details Contains process function definition
 *          the command structure definition,
 *          and the deserialize union definition
 *
 * @author  Liam JA MacDonald
 * @date    24-Sep-2019 (Created)
 * @date    20-May-2020 (Last Modified)
 */

#pragma once

/*
 * @brief   Command structure
 * @details Holds all variables needed to identify
 *          and execute a command.
 *
 *          type (8 bits): Requested Command
 *
 *          argument (8 bits): Additional information
 *          needed for command execution
 */
typedef struct command_
{
    unsigned type : 8;
    unsigned argument: 8;
}command;
/*
 * @brief   Deserialize Union
 *
 * @details Allows a string to be refferenced
 *          as a command object
 */
typedef union deserialize_command_
{
    char* input;
    command * command;
}deserialize;

#ifndef GLOBAL_COMMANDTABLE
#define GLOBAL_COMMANDTABLE

    extern void process(char*);

#endif //GLOBAL_COMMANDTABLE
