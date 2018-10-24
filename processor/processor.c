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
    int Rd = 0, Rr = 0, K = 0, k = 0, cp_res = 0, LBL = 0;
    double st_1 = 0, st_2 = 0;
    int processing = 1;
    while(processing)
    {
        int com_code = *PC++;
        switch(com_code)
        {

        #define DEF_CMD( name, code, argc, instruction) case CMD_##name: instruction; break;
            #include "../lib/asm_commands.h"
        #undef DEF_CMD

        default:
            printf(RED ALERT"AN UNDEFINED COMMAND CODE"RESET);
            processing = 0;
            break;
        }
    }
    Stack_destruct(STACK);
    Stack_destruct(CALL_STACK);
    return 0;
}

