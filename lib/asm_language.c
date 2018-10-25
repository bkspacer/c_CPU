#include "asm_language.h"


//=====================================================================
//------------------PRINTING-FUNCTIONS---------------------------------
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
    case LABEL_t:
        printf("label #%d", arg);
        break;

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
        printf("register R%d (SRAM pos)", arg);
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
