#include "processor.h"


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
    PRINTF(UNDERLINE"\nLOADING FORM FILE"RESET" %s\n", filename);
    struct stat st = {};
    stat(filename, &st);
    *code_len = st.st_size;

    FILE* codefile = fopen(filename, "rb");

    if(!codefile)
    {
        printf(RED"\tNO SUCH FILE IN THE DIRECTORY\n"RESET);
        return NULL;
    }

    int* code = (int*) calloc(*code_len, sizeof(int));

    if(!code)
    {
        printf(RED"\tCOULD NOT FIND SPACE FOR THE CODE\n"RESET);
        return NULL;
    }

    for(size_t i = 0; i < *code_len; ++i)
        code[i] = getc(codefile);
    fclose(codefile);

    PRINTF("\t[  "GREEN"OK"RESET"  ]\n");
    return code;
}

int process(int* PC0)
{
    int REG[REG_CAPACITY];      // register array
    int SRAM[SRAM_CAPACITY];    // SRAM array

    struct Stack STACK_ = {};   // stack
    Stack_construct(STACK_);
    struct Stack* STACK = &STACK_;

    struct Stack CALL_STACK_ = {}; // stack of called commands
    Stack_construct(CALL_STACK_);
    struct Stack* CALL_STACK = &CALL_STACK_;

    int* PC = PC0;      // first byte position
    int Rd = 0, Rr = 0; // registers positions
    int K = 0;          // constant number
    int k = 0;          // constant SRAM address
    int LBL = 0;        // code label
    int cp_res = 0;     // comparison flag
    double st_1 = 0, st_2 = 0; // variables to operate with stack

    PRINTF(UNDERLINE"\nPROCESSING\n"RESET);
    int processing = 1; // is 1 while code is running
    int com_code = 0; // current pro
    while(processing)
    {
        switch(com_code = *PC++)
        {

        #define DEF_CMD( name, code, argc, instruction) case CMD_##name: PRINTF("0x%02X: %s\t", code, #name); instruction; break;
            #include "../lib/asm_commands.h"
        #undef DEF_CMD

        default:
            printf(RED ALERT"AN UNDEFINED COMMAND CODE: 0x%02X\n"RESET, com_code);
            Stack_destruct(STACK);
            Stack_destruct(CALL_STACK);
            return 1;
            break;
        }
        PRINTF("\n");
    }



    PRINTF(UNDERLINE"\nPROCESSING COMPLETED\n"RESET);
    return 0;
}

