

#include "../lib/stack.h"

#define MAX_STR_LEN    (100) /**< Maximun of string length */
#define MAX_OF_ARGC    (3)   /**< Maximum of number of arguments for commands */
#define SRAM_CAPACITY  (32)  /**< Capacity of SRAM array  */
#define REG_CAPACITY   (128) /**< Capacity of MAX_OF_ARGC array */

#define RED         "\033[0;31m"
#define GREEN       "\033[0;32m"
#define PURPLE      "\033[0;35m"
#define BLUE        "\033[0;34m"

#define RESET       "\033[0m"
#define UNDERLINE   "\033[4m"
#define ALERT       "\033[5m"

#define GET( var ) ({var = *PC++;/* printf("\t(%s is %d)", #var, var);*/})


//======================================
//======================================
//-----------ASSEMBLER-INFO-------------
//======================================
//======================================

enum {LABEL_t = -1,
      UNKNOWN_t,
      NUM_t,
      REG_t,
      S_NUM_t,
      S_REG_t};

/** \brief Structure describing an assembler command
 *
 * \param name[MAX_STR_LEN] char - name of the command
 * \param code int - hex-code of the program (ID in binary code)
 * \param argc int - number of arguments
 * \param argtypes[MAX_OF_ARGC] int - types on input arguments
 *
 */
struct ASM_COM
{
    char name[MAX_STR_LEN];
    int  code;
    int  argc;
    int  argtypes[MAX_OF_ARGC];
};


/** \brief All the commands of the assembler
 *
 * \warning The last command must have code (0xFF)
 *
 */
/* NOTHING HERE */
enum Commands
{
    #define DEF_CMD( name, code, argc, instruction ) CMD_##name = (code),
    #include "asm_commands.h"
    #undef DEF_CMD
    command_end
};

//=====================================================================

/** \brief Prints given type of an argument (according to asm_commands.h)
 *
 * \param argtype int [in] - type of some argument
 *
 */
void print_arg_type(int argtype);


/** \brief Prints given argument with its type (according to asm_commands.h)
 *
 * \param argtype int [in] - type of given argument
 * \param arg int [in] - the argument to print
 *
 */
void print_arg(int argtype, int arg);


/** \brief Writes a full description on given ASM_ command
 *
 * \param command struct ASM_COM [in] - command to describe
 *
 */
void print_command_info(struct ASM_COM command);


/** \brief Writes description on all included functions
 *
 */
void print_assembler_info();


//=====================================================================

void print_arg_type(int argtype)
{
    switch(argtype)
    {
    case UNKNOWN_t:
        printf(RED"UNKNOWN"RESET);
        break;

    case NUM_t:
        printf("const");
        break;

    case REG_t:
        printf("register");
        break;

    case S_NUM_t:
        printf("const(SRAM pos)");
        break;

    case S_REG_t:
        printf("register(SRAM pos)");
        break;

    case LABEL_t:
        printf("label");
        break;
    }
    return;
}

void print_arg(int argtype, int arg)
{
    switch(argtype)
    {
    case UNKNOWN_t:
        printf(RED"UNKNOWN (%d)"RESET, arg);
        break;

    case NUM_t:
        printf("const %d", arg);
        break;

    case REG_t:
        printf("register R%d", arg);
        break;

    case S_NUM_t:
        printf("const %d (SRAM pos)", arg);
        break;

    case S_REG_t:
        printf("register R%d SRAM pos", arg);
        break;

    case LABEL_t:
        printf("label #%d", arg);
        break;
    }
    return;
}

void print_assembler_info()
{
    #define DEF_CMD( name, code, argc, instruction ) printf(UNDERLINE"COMMAND NAME       "RESET BLUE": %s\n"RESET\
                                                            UNDERLINE"CODE               "RESET": 0x%02X\n"\
                                                            UNDERLINE"NUMBER OF ARGUMENTS"RESET": %d\n"\
                                                            UNDERLINE"INSTRUCTION        "RESET": \n\t%s\n\n", #name, code, argc, #instruction);
    #include "asm_commands.h"
    #undef DEF_CMD

    return;
}

