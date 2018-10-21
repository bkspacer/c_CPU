#ifndef OSM_COMANDS_H_INCLUDED
#define OSM_COMANDS_H_INCLUDED

#define MAX_COM_NAME   (10)
#define MAX_STRLEN     (100)
#define NUM_OF_COMANDS (30)
#define MAX_CODE_LEN   (100000)
#define SRAM_CAPACITY  (32)
#define REG_CAPACITY   (128)

#define RESET       "\033[0m"
#define RED         "\033[0;31m"
#define GREEN       "\033[0;32m"
#define PURPLE      "\033[0;35m"
#define BLUE        "\033[0;34m"
#define UNDERLINE   "\033[4m"
#define ALERT       "\033[5m"

#include "../lib/stack.h"

//======================================
//======================================
//-----------ASSEMBLER-INFO-------------
//======================================
//======================================

enum {LABEL_t = -1,
      UNKNOWN_t,
      NUM_t,
      REG_t};

struct OSM_COM
{
    char name[MAX_COM_NAME];
    int code;
    int argc;
    int argtypes[10];
};

const struct OSM_COM OSM_COMANDS[] = {
//=====================================================================
//-------------------ARITHMETIC-INSTRUCTIONS---------------------------
//=====================================================================

#define OSM_ADD_REG_CODE   (0x01)
{          "ADD",           0x01, 2, {REG_t, REG_t}}, // Register
#define OSM_ADD_STACK_CODE (0x0A)
{          "ADD",           0x0A, 0, { }}, // Stack
#define OSM_ADD_COM( )                  \
({                                      \
    switch(com_code)                    \
    {                                   \
    case OSM_ADD_REG_CODE:              \
        OSM_GET(Rd);                    \
        OSM_GET(Rr);                    \
        REG[Rd] += REG[Rr];             \
        break;                          \
                                        \
    case OSM_ADD_STACK_CODE:            \
        Stack_pop(STACK, &k);           \
        Stack_pop(STACK, &K);           \
        Stack_push(STACK, k + K);       \
        break;                          \
                                        \
    default:                            \
        printf("undefined ADD code");   \
        return 1;                       \
        break;                          \
    }                                   \
})


#define OSM_SUB_REG_CODE   (0x02)
{          "SUB",           0x02, 2, {REG_t, REG_t}}, // Register
#define OSM_SUB_STACK_CODE (0x0B)
{          "SUB",           0x0B, 0, { }}, // Stack
#define OSM_SUB_COM( )                  \
({                                      \
    switch(com_code)                    \
    {                                   \
    case OSM_SUB_REG_CODE:              \
        OSM_GET(Rd);                    \
        OSM_GET(Rr);                    \
        REG[Rd] -= REG[Rr];             \
        break;                          \
                                        \
    case OSM_SUB_STACK_CODE:            \
        Stack_pop(STACK, &k);           \
        Stack_pop(STACK, &K);           \
        Stack_push(STACK, k - K);       \
        break;                          \
                                        \
    default:                            \
        printf("undefined SUB code");   \
        return 1;                       \
        break;                          \
    }                                   \
})


#define OSM_INC_CODE (0x03)
{          "INC",     0x03, 1, {REG_t}},
#define OSM_INC_COM( )  \
({                      \
    OSM_GET(Rd);        \
    REG[Rd]++;          \
})


#define OSM_DEC_CODE (0x04)
{          "DEC",     0x04, 1, {REG_t}},
#define OSM_DEC_COM( )  \
({                      \
    OSM_GET(Rd);        \
    REG[Rd]++;          \
})

#define OSM_CLR_CODE (0x05)
{          "CLR",     0x05, 1, {REG_t}},
#define OSM_CLR_COM( )  \
({                      \
    OSM_GET(Rd);        \
    REG[Rd] ^= REG[Rd]; \
})

#define OSM_SER_CODE (0x06)
{          "SER",     0x06, 1, {REG_t}},
#define OSM_SER_COM( ) \
({                     \
    OSM_GET(Rd);       \
    REG[Rd] = 0;       \
})


#define OSM_MUL_REG_CODE   (0x07)
{          "MUL",           0x07, 2, {REG_t, REG_t}}, // Register
#define OSM_MUL_STACK_CODE (0x0C)
{          "MUL",           0x0C, 0, { }}, // Stack
#define OSM_MUL_COM( )                  \
({                                      \
    switch(com_code)                    \
    {                                   \
    case OSM_MUL_REG_CODE:              \
        OSM_GET(Rd);                    \
        OSM_GET(Rr);                    \
        REG[Rd] *= REG[Rr];             \
        break;                          \
                                        \
    case OSM_MUL_STACK_CODE:            \
        Stack_pop(STACK, &K);           \
        Stack_pop(STACK, &k);           \
        Stack_push(STACK, k * K);       \
        break;                          \
    default:                            \
        printf("undefined MUL code");   \
        return 1;                       \
        break;                          \
    }                                   \
})


#define OSM_DIV_REG_CODE   (0x08)
{          "DIV",           0x08, 2, {REG_t, REG_t}}, // Register
#define OSM_DIV_STACK_CODE (0x0D)
{          "DIV",           0x0D, 0, { }}, // Stack
#define OSM_DIV_COM( )                  \
({                                      \
    switch(com_code)                    \
    {                                   \
    case OSM_DIV_REG_CODE:              \
        OSM_GET(Rd);                    \
        OSM_GET(Rr);                    \
        REG[Rd] /= REG[Rr];             \
        break;                          \
                                        \
    case OSM_DIV_STACK_CODE:            \
        Stack_pop(STACK, &K);           \
        Stack_pop(STACK, &k);           \
        Stack_push(STACK, K / k);       \
        break;                          \
    default:                            \
        printf("undefined DIV code");   \
        return 1;                       \
        break;                          \
    }                                   \
})


//=====================================================================
//----------------------BRANCH-INSTRUCTIONS----------------------------
//=====================================================================
{          "RET",     0x00, 0, { }},
#define OSM_RET_CODE (0x00)
#define OSM_RET_COM( ) \
({                     \
    Stack_pop(CALL_STACK, &K);\
    PC = PC0 + K;\
})

#define OSM_JMP_CODE (0x11)
{          "JMP",     0x11, 1, {LABEL_t}},
#define OSM_JMP_COM( ) \
({                     \
    OSM_GET(K);        \
    PC = PC0 + K;      \
    /*Stack_dump(STACK);*/\
})


#define OSM_CALL_CODE (0x12)
{          "CALL",     0x12, 1, {LABEL_t}},
#define OSM_CALL_COM( ) \
({                     \
    Stack_push(CALL_STACK, PC - PC0 + 1);\
    OSM_GET(K);        \
    PC = PC0 + K;      \
})


#define OSM_CP_REG_CODE   (0x13)
{          "CP",           0x13, 2, {REG_t, REG_t}}, // Register
#define OSM_CP_STACK_CODE (0x14)
{          "CP",           0x14, 0, { }}, // Stack
#define OSM_CP_COM( ) \
({                     \
    switch(com_code)\
    {\
    case OSM_CP_REG_CODE:\
        OSM_GET(Rd);\
        OSM_GET(Rr);\
        cp_res = REG[Rd] - REG[Rr];\
        break;\
        \
    case OSM_CP_STACK_CODE:\
        Stack_pop(STACK, &K);\
        Stack_pop(STACK, &k);\
        printf("(%d ? %d)", K, k);\
        cp_res = K - k;\
        Stack_push(STACK, k);\
        Stack_push(STACK, K);\
        break;\
        \
    default:\
        printf("undefined CP code");\
        return 1;\
        break;\
    }\
    printf("(%d)", cp_res);\
})

#define OSM_BREQ_CODE (0x15)
{          "BREQ",     0x15, 1, {LABEL_t}},
#define OSM_BREQ_COM( ) \
({                     \
    OSM_GET(K);\
    if(cp_res == 0)\
        PC = PC0 + K;\
})

#define OSM_BRNE_CODE (0x16)
{          "BRNE",     0x16, 1, {LABEL_t}},
#define OSM_BRNE_COM( ) \
({                     \
    OSM_GET(K);\
    if(cp_res != 0)\
        PC = PC0 + K;\
})


#define OSM_BRPL_CODE (0x17)
{          "BRPL",     0x17, 1, {LABEL_t}},
#define OSM_BRPL_COM( ) \
({                     \
    OSM_GET(K);\
    if(cp_res > 0)\
        PC = PC0 + K;\
})

//=====================================================================
//-------------------DATA-TRANSFER-INSTRUCTIONS------------------------
//=====================================================================

#define OSM_MOV_CODE (0x31)
{          "MOV",     0x31, 2, {REG_t, REG_t}},
#define OSM_MOV_COM( ) \
({                     \
    OSM_GET(Rd);       \
    OSM_GET(Rr);       \
    REG[Rd] = REG[Rr]; \
})


#define OSM_LDI_CODE (0x32)
{          "LDI",     0x32, 2, {REG_t, NUM_t}}, // Load immediate
#define OSM_LD_CODE  (0x33)
{          "LD",      0x33, 2, {REG_t, REG_t}}, // Load indirect from SRAM
#define OSM_LDD_CODE (0x34)
{          "LDD",     0x34, 3, {REG_t, REG_t, NUM_t}}, // Load indirect with displacement from SRAM
#define OSM_LDS_CODE (0x35)
{          "LDS",     0x35, 2, {REG_t, NUM_t}}, // Load direct from SRAM
#define OSM_LD_COM( ) \
({                     \
    switch(com_code)\
    {\
    case OSM_LDI_CODE:\
        OSM_GET(Rd);\
        OSM_GET(K);\
        REG[Rd] = K;\
        break;\
        \
    case OSM_LD_CODE:\
        OSM_GET(Rd);\
        OSM_GET(Rr);\
        REG[Rd] = SRAM[REG[Rr]];\
        break;\
        \
    case OSM_LDD_CODE:\
        OSM_GET(Rd);\
        OSM_GET(Rr);\
        OSM_GET(K);\
        REG[Rd] = SRAM[REG[Rr] + K];\
        break;\
    \
    case OSM_LDS_CODE:\
        OSM_GET(Rd);\
        OSM_GET(k);\
        REG[Rd] = SRAM[k];\
        break;\
    \
    default:\
        printf("undefined LD code");\
        return 1;\
        break;\
    }\
})


#define OSM_ST_CODE  (0x36)
{          "ST",      0x36, 2, {REG_t, REG_t}}, // Store indirect to SRAM
#define OSM_STD_CODE (0x37)
{          "STD",     0x37, 3, {REG_t, NUM_t, REG_t}}, // Store indirect with displacement to SRAM
#define OSM_STS_CODE (0x38)
{          "STS",     0x38, 2, {NUM_t, REG_t}}, // Store direct to SRAM
#define OSM_ST_COM( ) \
({                     \
    switch(com_code)\
    {\
    case OSM_ST_CODE:\
        OSM_GET(Rd);\
        OSM_GET(Rr);\
        SRAM[REG[Rd]] = REG[Rr];\
        break;\
    \
    case OSM_STD_CODE:\
        OSM_GET(Rd);\
        OSM_GET(K);\
        OSM_GET(Rr);\
        SRAM[REG[Rd] + K] = REG[Rr];\
        break;\
    \
    case OSM_STS_CODE:\
        OSM_GET(k);\
        OSM_GET(Rr);\
        SRAM[k] = REG[Rr];\
        break;\
        \
    default:\
        printf("undefined ST code");\
        return 1;\
        break;\
    }\
})


#define OSM_IN_REG_CODE   (0x39)
{          "IN",           0x39, 1, {REG_t}}, // Register
#define OSM_IN_STACK_CODE (0x3F)
{          "IN",           0x3F, 0, { }}, // Stack
#define OSM_IN_COM( )                   \
({                                      \
    printf("\n");                       \
    switch(com_code)                    \
    {                                   \
    case OSM_IN_REG_CODE:               \
        OSM_GET(Rd);                    \
        scanf("%i", REG + Rd);          \
        break;                          \
                                        \
    case OSM_IN_STACK_CODE:             \
        scanf("%i", &K);                \
        Stack_push(STACK, K);           \
        break;                          \
                                        \
    default:                            \
        printf("undefined IN code");    \
        return 1;                       \
        break;                          \
    }                                   \
})


#define OSM_OUT_REG_CODE   (0x3A)
{          "OUT",           0x3A, 1, {REG_t}}, // Register
#define OSM_OUT_STACK_CODE (0x3E)
{          "OUT",           0x3E, 0, { }}, // Stack
#define OSM_OUT_COM( )                  \
({                                      \
    switch(com_code)                    \
    {                                   \
    case OSM_OUT_REG_CODE:              \
        OSM_GET(Rr);                    \
        printf("\n%i", REG[Rr]);          \
        break;                          \
                                        \
    case OSM_OUT_STACK_CODE:            \
        Stack_pop(STACK, &K);           \
        printf("\n%i", K);                 \
        break;                          \
                                        \
    default:                            \
        printf("undefined OUT code");   \
        return 1;                       \
        break;                          \
    }                                   \
})


#define OSM_PUSH_REG_CODE   (0x41)
{          "PUSH",           0x41, 1, {REG_t}}, // Register
#define OSM_PUSH_STACK_CODE (0x4A)
{          "PUSH",           0x4A, 1, {NUM_t}}, // Stack
#define OSM_PUSH_COM( )                 \
({                                      \
    switch(com_code)                    \
    {                                   \
    case OSM_PUSH_REG_CODE:             \
        OSM_GET(Rr);                    \
        Stack_push(STACK, REG[Rr]);     \
        break;                          \
                                        \
    case OSM_PUSH_STACK_CODE:           \
        OSM_GET(K);                     \
        Stack_push(STACK, K);           \
        break;                          \
                                        \
    default:                            \
        printf("undefined PUSH code");  \
        return 1;                       \
        break;                          \
    }                                   \
})


#define OSM_POP_REG_CODE   (0x42)
{          "POP",           0x42, 1, {REG_t}}, // Register
#define OSM_POP_STACK_CODE (0x4B)
{          "POP",           0x4B, 0, { }}, // Stack
#define OSM_POP_COM( )                  \
({                                      \
    switch(com_code)                    \
    {                                   \
    case OSM_POP_REG_CODE:              \
        OSM_GET(Rd);                    \
        Stack_pop(STACK, REG + Rd);     \
        break;                          \
                                        \
    case OSM_POP_STACK_CODE:            \
        Stack_pop(STACK, &K);           \
        break;                          \
                                        \
    default:                            \
        printf("undefined POP code");   \
        return 1;                       \
        break;                          \
    }                                   \
})


//=====================================================================
//-------------------MCU-CONTROL-INSTRUCTIONS--------------------------
//=====================================================================

#define OSM_NOP_CODE (0xF0)
{          "NOP",     0xF0, 0, { }},
#define OSM_NOP_COM( ) \
({                     \
                       \
})

#define OSM_END_CODE (0xFF)
{          "END",     0xFF, 0, { }}
#define OSM_END_COM( ) \
({                     \
    processing = 0;    \
})

};

#define OSM_GET( var ) ({var = *PC++; printf("\t(%s is %d)", #var, var);})


int process(int* PC)
{
    int REG[REG_CAPACITY];
    int SRAM[SRAM_CAPACITY];

    struct Stack STACK_ = {};
    Stack_construct(STACK_);
    struct Stack* STACK = &STACK_;

    struct Stack CALL_STACK_ = {};
    Stack_construct(CALL_STACK_);
    struct Stack* CALL_STACK = &CALL_STACK_;

    int* PC0 = PC;
    int Rd = 0, Rr = 0, K = 0, k = 0, cp_res = 0;
    int processing = 1;
    while(processing)
    {
        int com_code = *PC++;
        printf("0x%02X is ", com_code);
        switch(com_code)
        {
        case OSM_RET_CODE:
            printf("RET");
            OSM_RET_COM();
            break;

        case OSM_JMP_CODE:
            printf("JMP");
            OSM_JMP_COM();
            break;

        case OSM_CP_REG_CODE:
        case OSM_CP_STACK_CODE:
            printf("CP");
            OSM_CP_COM();
            break;

        case OSM_BRNE_CODE:
            printf("BRNE");
            OSM_BRNE_COM();
            break;

        case OSM_BREQ_CODE:
            printf("BREQ");
            OSM_BREQ_COM();
            break;

        case OSM_BRPL_CODE:
            printf("BRPL");
            OSM_BRPL_COM();
            break;

        case OSM_CALL_CODE:
            printf("CALL");
            OSM_CALL_COM();
            break;

        case OSM_CLR_CODE:
            printf("CLR");
            OSM_CLR_COM();
            break;

        case OSM_MOV_CODE:
            printf("MOV");
            OSM_MOV_COM();
            break;

        case OSM_LDI_CODE:
        case OSM_LD_CODE:
        case OSM_LDD_CODE:
        case OSM_LDS_CODE:
            printf("LD");
            OSM_LD_COM();
            break;

        case OSM_ST_CODE:
        case OSM_STD_CODE:
        case OSM_STS_CODE:
            printf("ST");
            OSM_ST_COM();
            break;

        case OSM_OUT_REG_CODE:
        case OSM_OUT_STACK_CODE:
            printf("OUT");
            OSM_OUT_COM();
            break;

        case OSM_PUSH_REG_CODE:
        case OSM_PUSH_STACK_CODE:
            printf("PUSH");
            OSM_PUSH_COM();
            break;

        case OSM_POP_REG_CODE:
        case OSM_POP_STACK_CODE:
            printf("POP");
            OSM_POP_COM();
            break;

        case OSM_SUB_REG_CODE:
        case OSM_SUB_STACK_CODE:
            printf("SUB");
            OSM_SUB_COM();
            break;

        case OSM_MUL_REG_CODE:
        case OSM_MUL_STACK_CODE:
            printf("MUL");
            OSM_MUL_COM();
            break;

        case OSM_DIV_REG_CODE:
        case OSM_DIV_STACK_CODE:
            printf("DIV");
            OSM_DIV_COM();
            break;

        case OSM_IN_REG_CODE:
        case OSM_IN_STACK_CODE:
            printf("IN");
            OSM_IN_COM();
            break;

        case OSM_NOP_CODE:
            printf("NOP");
            OSM_NOP_COM();
            break;

        case OSM_SER_CODE:
            printf("SER");
            OSM_SER_COM();
            break;

        case OSM_END_CODE:
            printf("END\n");
            OSM_END_COM();
            break;

        default:
            printf(RED ALERT"AN UNDEFINED COMMAND CODE\n"RESET);
            Stack_destruct(STACK);
            Stack_destruct(CALL_STACK);
            return 1;
            break;
        }
        printf("\n");

    }
    Stack_destruct(STACK);
    Stack_destruct(CALL_STACK);
    return 0;
}


#endif // OSM_COMANDS_H_INCLUDED
