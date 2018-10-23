#include "processor.h"
#include "../lib/osm_comands.h"


//=====================================================================
//------------------PRINTING-FUNCTIONS---------------------------------
//=====================================================================

void print_code(const int* code, int code_len, int bytes_in_line)
{
    printf("\n========"UNDERLINE"CODE"RESET"========\n");
    for(size_t i = 0; i < code_len; ++i)
        printf("0x%02X %c%c", code[i], ((i + 1) % 4 == 0) ? '|' : ' ', ((i + 1) % bytes_in_line == 0 || i == code_len - 1) ? '\n' : ' ');
    return;
}


//=====================================================================
//----------------------MAIN-FUNCTIONS---------------------------------
//=====================================================================

int* loadcode(char* filename, size_t* code_len)
{
    struct stat st = {};
    stat(filename, &st);
    *code_len = st.st_size;

    int* code = (int*) calloc(*code_len, sizeof(int));

    FILE* codefile = fopen(filename, "rb");
    for(size_t i = 0; i < *code_len; ++i)
        code[i] = getc(codefile);
    fclose(codefile);

    return code;
}

int process(int* PC0)
{
    int REG[REG_CAPACITY];
    int SRAM[SRAM_CAPACITY];

    struct Stack STACK_ = {};
    Stack_construct(STACK_);
    struct Stack* STACK = &STACK_;

    struct Stack CALL_STACK_ = {};
    Stack_construct(CALL_STACK_);
    struct Stack* CALL_STACK = &CALL_STACK_;

    int* PC = PC0;
    int Rd = 0, Rr = 0, K = 0, k = 0, cp_res = 0;
    int processing = 1;
    while(processing)
    {
        int com_code = *PC++;
        printf("0x%02X is ", com_code);
        switch(com_code)
        {
        case ASM_ADD_CODE:
        case ASM_ADD_STACK_CODE:
            printf("ADD");
            ASM_ADD_COM();
            break;

        case ASM_SUB_CODE:
        case ASM_SUB_STACK_CODE:
            printf("SUB");
            ASM_SUB_COM();
            break;



        case ASM_INC_CODE:
            printf("INC");
            ASM_INC_COM();
            break;

        case ASM_DEC_CODE:
            printf("DEC");
            ASM_DEC_COM();
            break;

        case ASM_CLR_CODE:
            printf("CLR");
            ASM_CLR_COM();
            break;

        case ASM_SER_CODE:
            printf("SER");
            ASM_SER_COM();
            break;



        case ASM_MUL_CODE:
        case ASM_MUL_STACK_CODE:
            printf("MUL");
            ASM_MUL_COM();
            break;

        case ASM_DIV_CODE:
        case ASM_DIV_STACK_CODE:
            printf("DIV");
            ASM_DIV_COM();
            break;



        case ASM_RET_CODE:
            printf("RET");
            ASM_RET_COM();
            break;

        case ASM_JMP_CODE:
            printf("JMP");
            ASM_JMP_COM();
            break;

        case ASM_CALL_CODE:
            printf("CALL");
            ASM_CALL_COM();
            break;



        case ASM_CP_CODE:
        case ASM_CP_STACK_CODE:
            printf("COMP");
            ASM_CP_COM();
            break;



        case ASM_BREQ_CODE:
            printf("BREQ");
            ASM_BREQ_COM();
            break;

        case ASM_BRNE_CODE:
            printf("BRNE");
            ASM_BRNE_COM();
            break;

        case ASM_BRPL_CODE:
            printf("BRPL");
            ASM_BRPL_COM();
            break;

        case ASM_BRMI_CODE:
            printf("BRMI");
            ASM_BRMI_COM();
            break;



        case ASM_MOV_CODE:
            printf("MOV");
            ASM_MOV_COM();
            break;



        case ASM_LDI_CODE:
        case ASM_LD_CODE:
        case ASM_LDD_CODE:
        case ASM_LDS_CODE:
            printf("LOAD");
            ASM_LD_COM();
            break;



        case ASM_ST_CODE:
        case ASM_STD_CODE:
        case ASM_STS_CODE:
            printf("STORE");
            ASM_ST_COM();
            break;



        case ASM_IN_CODE:
        case ASM_IN_STACK_CODE:
            printf("IN");
            ASM_IN_COM();
            break;



        case ASM_OUT_CODE:
        case ASM_OUT_STACK_CODE:
            printf("OUT");
            ASM_OUT_COM();
            break;



        case ASM_PUSH_CODE:
        case ASM_PUSH_STACK_NUM_CODE:
        case ASM_PUSH_STACK_S_N_CODE:
        case ASM_PUSH_STACK_S_R_CODE:
        case ASM_PUSH_STACK_S_R_PLUS_N_CODE:
            printf("PUSH");
            ASM_PUSH_COM();
            break;



        case ASM_POP_CODE:
        case ASM_POP_STACK_CODE:
            printf("POP");
            ASM_POP_COM();
            break;



        case ASM_NOP_CODE:
            printf("NOP");
            ASM_NOP_COM();
            break;

        case ASM_END_CODE:
            printf("END\n");
            ASM_END_COM();
            break;

        default:
            printf(RED ALERT"AN UNDEFINED COMMAND CODE"RESET);
            processing = 0;
            break;
        }
        printf("\n");

    }
    Stack_destruct(STACK);
    Stack_destruct(CALL_STACK);
    return 0;
}

