#ifndef OSM_COMANDS_H_INCLUDED
#define OSM_COMANDS_H_INCLUDED

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

#define ASM_GET( var ) ({var = *PC++; printf("\t(%s is %d)", #var, var);})


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
const struct ASM_COM ASM_COMMANDS[] = {

//=====================================================================
//-------------------ARITHMETIC-INSTRUCTIONS---------------------------
//=====================================================================

#define ASM_ADD_CODE       (0x01)
{          "ADD",           0x01, 2, {REG_t, REG_t}},

#define ASM_ADD_STACK_CODE (0x0A)
{          "ADD",           0x0A, 0, { }},

#define ASM_ADD_COM( )                  \
({                                      \
    switch(com_code)                    \
    {                                   \
    case ASM_ADD_CODE:                  \
        ASM_GET(Rd);                    \
        ASM_GET(Rr);                    \
        REG[Rd] += REG[Rr];             \
        break;                          \
                                        \
    case ASM_ADD_STACK_CODE:            \
        Stack_pop(STACK, &k);           \
        Stack_pop(STACK, &K);           \
        Stack_push(STACK, k + K);       \
        break;                          \
                                        \
    default:                            \
        printf("\nundefined ADD code"); \
        return 1;                       \
        break;                          \
    }                                   \
})

//====================================================

#define ASM_SUB_CODE       (0x02)
{          "SUB",           0x02, 2, {REG_t, REG_t}},

#define ASM_SUB_STACK_CODE (0x0B)
{          "SUB",           0x0B, 0, { }},

#define ASM_SUB_COM( )                  \
({                                      \
    switch(com_code)                    \
    {                                   \
    case ASM_SUB_CODE:                  \
        ASM_GET(Rd);                    \
        ASM_GET(Rr);                    \
        REG[Rd] -= REG[Rr];             \
        break;                          \
                                        \
    case ASM_SUB_STACK_CODE:            \
        Stack_pop(STACK, &k);           \
        Stack_pop(STACK, &K);           \
        Stack_push(STACK, k - K);       \
        break;                          \
                                        \
    default:                            \
        printf("\nundefined SUB code"); \
        return 1;                       \
        break;                          \
    }                                   \
})

//====================================================

#define ASM_INC_CODE (0x03)
{          "INC",     0x03, 1, {REG_t}},

#define ASM_INC_COM( )  \
({                      \
    ASM_GET(Rd);        \
    REG[Rd]++;          \
})

//====================================================

#define ASM_DEC_CODE (0x04)
{          "DEC",     0x04, 1, {REG_t}},

#define ASM_DEC_COM( )  \
({                      \
    ASM_GET(Rd);        \
    REG[Rd]--;          \
})

//====================================================

#define ASM_CLR_CODE (0x05)
{          "CLR",     0x05, 1, {REG_t}},

#define ASM_CLR_COM( )  \
({                      \
    ASM_GET(Rd);        \
    REG[Rd] = 0;        \
})

//====================================================

#define ASM_SER_CODE (0x06)
{          "SER",     0x06, 1, {REG_t}},

#define ASM_SER_COM( ) \
({                     \
    ASM_GET(Rd);       \
    REG[Rd] = 0;       \
})

//====================================================

#define ASM_MUL_CODE       (0x07)
{          "MUL",           0x07, 2, {REG_t, REG_t}},

#define ASM_MUL_STACK_CODE (0x0C)
{          "MUL",           0x0C, 0, { }},

#define ASM_MUL_COM( )                  \
({                                      \
    switch(com_code)                    \
    {                                   \
    case ASM_MUL_CODE:                  \
        ASM_GET(Rd);                    \
        ASM_GET(Rr);                    \
        REG[Rd] *= REG[Rr];             \
        break;                          \
                                        \
    case ASM_MUL_STACK_CODE:            \
        Stack_pop(STACK, &K);           \
        Stack_pop(STACK, &k);           \
        Stack_push(STACK, k * K);       \
        break;                          \
                                        \
    default:                            \
        printf("\nundefined MUL code"); \
        return 1;                       \
        break;                          \
    }                                   \
})

//====================================================

#define ASM_DIV_CODE       (0x08)
{          "DIV",           0x08, 2, {REG_t, REG_t}},

#define ASM_DIV_STACK_CODE (0x0D)
{          "DIV",           0x0D, 0, { }},

#define ASM_DIV_COM( )                  \
({                                      \
    switch(com_code)                    \
    {                                   \
    case ASM_DIV_CODE:                  \
        ASM_GET(Rd);                    \
        ASM_GET(Rr);                    \
        REG[Rd] /= REG[Rr];             \
        break;                          \
                                        \
    case ASM_DIV_STACK_CODE:            \
        Stack_pop(STACK, &K);           \
        Stack_pop(STACK, &k);           \
        Stack_push(STACK, K / k);       \
        break;                          \
                                        \
    default:                            \
        printf("\nundefined DIV code"); \
        return 1;                       \
        break;                          \
    }                                   \
})


//=====================================================================
//----------------------BRANCH-INSTRUCTIONS----------------------------
//=====================================================================

#define ASM_RET_CODE (0x00)
{          "RET",     0x00, 0, { }},

#define ASM_RET_COM( )          \
({                              \
    Stack_pop(CALL_STACK, &K);  \
    PC = PC0 + K;               \
})

//====================================================

#define ASM_JMP_CODE (0x11)
{          "JMP",     0x11, 1, {LABEL_t}},

#define ASM_JMP_COM( ) \
({                     \
    ASM_GET(K);        \
    PC = PC0 + K;      \
})

//====================================================

#define ASM_CALL_CODE (0x12)
{          "CALL",     0x12, 1, {LABEL_t}},

#define ASM_CALL_COM( )                     \
({                                          \
    Stack_push(CALL_STACK, PC - PC0 + 1);   \
    ASM_GET(K);                             \
    PC = PC0 + K;                           \
})

//====================================================

#define ASM_CP_CODE       (0x13)
{          "CP",           0x13, 2, {REG_t, REG_t}},

#define ASM_CP_STACK_CODE (0x14)
{          "CP",           0x14, 0, { }},

#define ASM_CP_COM( )                       \
({                                          \
    switch(com_code)                        \
    {                                       \
    case ASM_CP_CODE:                       \
        ASM_GET(Rd);                        \
        ASM_GET(Rr);                        \
        cp_res = REG[Rd] - REG[Rr];         \
        break;                              \
                                            \
    case ASM_CP_STACK_CODE:                 \
        Stack_pop(STACK, &K);               \
        Stack_pop(STACK, &k);               \
        cp_res = K - k;                     \
        Stack_push(STACK, k);               \
        Stack_push(STACK, K);               \
        break;                              \
                                            \
    default:                                \
        printf("\nundefined COMP code");    \
        return 1;                           \
        break;                              \
    }                                       \
})

//====================================================

#define ASM_BREQ_CODE (0x15)
{          "BREQ",     0x15, 1, {LABEL_t}},

#define ASM_BREQ_COM( ) \
({                      \
    ASM_GET(K);         \
    if(cp_res == 0)     \
        PC = PC0 + K;   \
})

//====================================================

#define ASM_BRNE_CODE (0x16)
{          "BRNE",     0x16, 1, {LABEL_t}},

#define ASM_BRNE_COM( ) \
({                      \
    ASM_GET(K);         \
    if(cp_res != 0)     \
        PC = PC0 + K;   \
})

//====================================================

#define ASM_BRPL_CODE (0x17)
{          "BRPL",     0x17, 1, {LABEL_t}},

#define ASM_BRPL_COM( ) \
({                      \
    ASM_GET(K);         \
    if(cp_res > 0)      \
        PC = PC0 + K;   \
})

//====================================================

#define ASM_BRMI_CODE (0x18)
{          "BRMI",     0x18, 1, {LABEL_t}},

#define ASM_BRMI_COM( ) \
({                      \
    ASM_GET(K);         \
    if(cp_res < 0)      \
        PC = PC0 + K;   \
})


//=====================================================================
//-------------------DATA-TRANSFER-INSTRUCTIONS------------------------
//=====================================================================

#define ASM_MOV_CODE (0x31)
{          "MOV",     0x31, 2, {REG_t, REG_t}},

#define ASM_MOV_COM( ) \
({                     \
    ASM_GET(Rd);       \
    ASM_GET(Rr);       \
    REG[Rd] = REG[Rr]; \
})

//====================================================

#define ASM_LDI_CODE (0x32)
{          "LDI",     0x32, 2, {REG_t, NUM_t}}, // LoaD Immediate

#define ASM_LD_CODE  (0x33)
{          "LD",      0x33, 2, {REG_t, S_REG_t}}, // LoaD  indirect (from SRAM)

#define ASM_LDD_CODE (0x34)
{          "LDD",     0x34, 3, {REG_t, S_REG_t, S_NUM_t}}, // LoaD indirect with Displacement (from SRAM)

#define ASM_LDS_CODE (0x35)
{          "LDS",     0x35, 2, {REG_t, S_NUM_t}}, // LoaD direct (from SRAM)

#define ASM_LD_COM( )                       \
({                                          \
    switch(com_code)                        \
    {                                       \
    case ASM_LDI_CODE:                      \
        ASM_GET(Rd);                        \
        ASM_GET(K);                         \
        REG[Rd] = K;                        \
        break;                              \
                                            \
    case ASM_LD_CODE:                       \
        ASM_GET(Rd);                        \
        ASM_GET(Rr);                        \
        REG[Rd] = SRAM[REG[Rr]];            \
        break;                              \
                                            \
    case ASM_LDD_CODE:                      \
        ASM_GET(Rd);                        \
        ASM_GET(Rr);                        \
        ASM_GET(k);                         \
        REG[Rd] = SRAM[REG[Rr] + k];        \
        break;                              \
                                            \
    case ASM_LDS_CODE:                      \
        ASM_GET(Rd);                        \
        ASM_GET(k);                         \
        REG[Rd] = SRAM[k];                  \
        break;                              \
                                            \
    default:                                \
        printf("\nundefined LOAD code");    \
        return 1;                           \
        break;                              \
    }                                       \
})

//====================================================

#define ASM_ST_CODE  (0x36)
{          "ST",      0x36, 2, {S_REG_t, REG_t}}, // STore indirect (to SRAM)

#define ASM_STD_CODE (0x37)
{          "STD",     0x37, 3, {S_REG_t, S_NUM_t, REG_t}}, // STore indirect with Displacement (to SRAM)

#define ASM_STS_CODE (0x38)
{          "STS",     0x38, 2, {S_NUM_t, REG_t}}, // STore direct (to SRAM)

#define ASM_ST_COM( )                       \
({                                          \
    switch(com_code)                        \
    {                                       \
    case ASM_ST_CODE:                       \
        ASM_GET(Rd);                        \
        ASM_GET(Rr);                        \
        SRAM[REG[Rd]] = REG[Rr];            \
        break;                              \
                                            \
    case ASM_STD_CODE:                      \
        ASM_GET(Rd);                        \
        ASM_GET(k);                         \
        ASM_GET(Rr);                        \
        SRAM[REG[Rd] + k] = REG[Rr];        \
        break;                              \
                                            \
    case ASM_STS_CODE:                      \
        ASM_GET(k);                         \
        ASM_GET(Rr);                        \
        SRAM[k] = REG[Rr];                  \
        break;                              \
                                            \
    default:                                \
        printf("\nundefined STORE code");   \
        return 1;                           \
        break;                              \
    }                                       \
})

//====================================================

#define ASM_IN_CODE       (0x39)
{          "IN",           0x39, 1, {REG_t}},

#define ASM_IN_STACK_CODE (0x3F)
{          "IN",           0x3F, 0, { }},

#define ASM_IN_COM( )                   \
({                                      \
    switch(com_code)                    \
    {                                   \
    case ASM_IN_CODE:                   \
        ASM_GET(Rd);                    \
        scanf("%i", REG + Rd);          \
        break;                          \
                                        \
    case ASM_IN_STACK_CODE:             \
        scanf("%i", &K);                \
        Stack_push(STACK, K);           \
        break;                          \
                                        \
    default:                            \
        printf("\nundefined IN code");  \
        return 1;                       \
        break;                          \
    }                                   \
})

//====================================================

#define ASM_OUT_CODE (0x3A)
{          "OUT",     0x3A, 1, {REG_t}},

#define ASM_OUT_STACK_CODE (0x3E)
{          "OUT",           0x3E, 0, { }},

#define ASM_OUT_COM( )                  \
({                                      \
    switch(com_code)                    \
    {                                   \
    case ASM_OUT_CODE:                  \
        ASM_GET(Rr);                    \
        printf("\n%i", REG[Rr]);        \
        break;                          \
                                        \
    case ASM_OUT_STACK_CODE:            \
        Stack_pop(STACK, &K);           \
        printf("\n%i", K);              \
        break;                          \
                                        \
    default:                            \
        printf("\nundefined OUT code"); \
        return 1;                       \
        break;                          \
    }                                   \
})

//====================================================

#define ASM_PUSH_CODE (0x41)
{          "PUSH",     0x41, 1, {REG_t}},

#define ASM_PUSH_STACK_NUM_CODE        (0x4A)
{          "PUSH",                      0x4A, 1, {NUM_t}},

#define ASM_PUSH_STACK_S_N_CODE        (0x4B)
{          "PUSH",                      0x4B, 1, {S_NUM_t}},

#define ASM_PUSH_STACK_S_R_CODE        (0x4C)
{          "PUSH",                      0x4C, 1, {S_REG_t}},

#define ASM_PUSH_STACK_S_R_PLUS_N_CODE (0x4D)
{          "PUSH",                      0x4D, 2, {S_REG_t, S_NUM_t}},

#define ASM_PUSH_COM( )                         \
({                                              \
    switch(com_code)                            \
    {                                           \
    case ASM_PUSH_CODE:                         \
        ASM_GET(Rr);                            \
        Stack_push(STACK, REG[Rr]);             \
        break;                                  \
                                                \
    case ASM_PUSH_STACK_NUM_CODE:               \
        ASM_GET(K);                             \
        Stack_push(STACK, K);                   \
        break;                                  \
                                                \
    case ASM_PUSH_STACK_S_N_CODE:               \
        ASM_GET(k);                             \
        Stack_push(STACK, SRAM[k]);             \
        break;                                  \
                                                \
    case ASM_PUSH_STACK_S_R_CODE:               \
        ASM_GET(Rr);                            \
        Stack_push(STACK, SRAM[REG[Rr]]);       \
        break;                                  \
                                                \
    case ASM_PUSH_STACK_S_R_PLUS_N_CODE:        \
        ASM_GET(Rr);                            \
        ASM_GET(k);                             \
        Stack_push(STACK, SRAM[REG[Rr] + k]);   \
        break;                                  \
                                                \
    default:                                    \
        printf("\nundefined PUSH code");        \
        return 1;                               \
        break;                                  \
    }                                           \
})

//====================================================

#define ASM_POP_CODE       (0x42)
{          "POP",           0x42, 1, {REG_t}},

#define ASM_POP_STACK_CODE (0x4E)
{          "POP",           0x4E, 0, { }},

#define ASM_POP_COM( )                  \
({                                      \
    switch(com_code)                    \
    {                                   \
    case ASM_POP_CODE:                  \
        ASM_GET(Rd);                    \
        Stack_pop(STACK, REG + Rd);     \
        break;                          \
                                        \
    case ASM_POP_STACK_CODE:            \
        Stack_pop(STACK, &K);           \
        break;                          \
                                        \
    default:                            \
        printf("\nundefined POP code"); \
        return 1;                       \
        break;                          \
    }                                   \
})


//=====================================================================
//-------------------MCU-CONTROL-INSTRUCTIONS--------------------------
//=====================================================================

#define ASM_NOP_CODE (0xF0)
{          "NOP",     0xF0, 0, { }},

#define ASM_NOP_COM( ) \
({                     \
                       \
})

//====================================================

#define ASM_END_CODE (0xFF)
{          "END",     0xFF, 0, { }}

#define ASM_END_COM( ) \
({                     \
    processing = 0;    \
})
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

void print_command_info(struct ASM_COM command)
{
    printf(UNDERLINE"COMMAND\n"RESET"\t%s (0x%02X)\n", command.name, command.code);
    if(!command.argc)
    {
        printf("\n");
        return;
    }

    printf("\t{\n");
    for(int i = 0; i < command.argc; ++i)
    {
        printf("\t\t%d) ", i + 1);
        print_arg_type(command.argtypes[i]);
        printf("\n");
    }
    printf("}\n\n");
    return;
}

void print_assembler_info()
{
    int i = 0;

    while(ASM_COMMANDS[i].code != 0xFF)
        print_command_info(ASM_COMMANDS[i++]);
    print_command_info(ASM_COMMANDS[i]);

    return;
}


#endif // OSM_COMANDS_H_INCLUDED
