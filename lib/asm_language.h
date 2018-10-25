/** \file asm_language.h
 */
#ifndef ASM_LANGUAGE_H_INCLUDED
#define ASM_LANGUAGE_H_INCLUDED

#include <stdio.h>

#define MAX_STR_LEN    (100) /**< Maximun of string length */
#define MAX_OF_ARGC    (3)   /**< Maximun of commands arguments */
#define SRAM_CAPACITY  (32)  /**< Capacity of SRAM array  */
#define REG_CAPACITY   (128) /**< Capacity of MAX_OF_ARGC array */

#define RED         "\033[0;31m"
#define GREEN       "\033[0;32m"
#define PURPLE      "\033[0;35m"
#define BLUE        "\033[0;34m"

#define RESET       "\033[0m"
#define UNDERLINE   "\033[4m"
#define ALERT       "\033[5m"

//#define DEBUG_PRINTING

#ifdef DEBUG_PRINTING
#define PRINTF printf
#else
#define PRINTF(...)
#endif // DEBUG

#define GET( var ) ({var = *PC++; PRINTF("(%s is %d)", #var, var);})


/** \brief All the types used in the assembler
 *
 * \param LABEL_t  (-1) - position in the in the program binary code
 * \param UNKNOWN_t (0) - in case something has indefinite type
 * \param NUM_t     (1) - constant number
 * \param REG_t     (2) - constant register
 * \param S_NUM_t   (3) - position in SRAM to which the register points
 * \param S_REG_t   (4) - position in SRAM to which the constant points
 *
 */
enum {LABEL_t = -1,
      UNKNOWN_t,
      NUM_t,
      REG_t,
      S_NUM_t,
      S_REG_t};

/** \brief All the commands of the assembler (according to asm_commands.h)
 *
 */
enum Commands
{
    #define DEF_CMD( name, code, argc, instruction ) CMD_##name = (code),
    #include "asm_commands.h"
    #undef DEF_CMD
    command_end
};


//=====================================================================

/** \brief Prints given type of an argument
 *
 * \param argtype int [in] - type of some argument
 *
 */
void print_arg_type(int argtype);


/** \brief Prints given argument with its type
 *
 * \param argtype int [in] - type of given argument
 * \param arg int [in] - the argument to print
 *
 */
void print_arg(int argtype, int arg);


/** \brief Writes description on all included functions
 *
 */
void print_assembler_info();


#endif // ASM_LANGUAGE_H_INCLUDED
