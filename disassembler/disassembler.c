#include "disassembler.h"

int width(int n)
{
    int i = 0;
    for(; n > 0; n /= 10, i++)
        ;
    return i;
}

int print_prog(int* code, int code_len, char* filename)
{
    printf(UNDERLINE"\nPROGRAMM WRITING\n"RESET);

    int max_name_len = 0;
    #define DEF_CMD( name, byte, argc, instruction )    \
    {                                                   \
        if(strlen(#name) > max_name_len)                \
            max_name_len = strlen(#name);               \
    }
    #include "../lib/asm_commands.h"
    #undef DEF_CMD

    FILE* outfile = fopen(filename, "w");
    if(!outfile)
    {
        printf(RED"\tNO SUCH FILE IN THE DIRECTORY\n"RESET);
        return 1;
    }

    int i = 0, cur_byte = 0;
    while(i < code_len)
    {
        cur_byte = code[i];

        #define DEF_CMD( name, byte, argc, instruction )        \
        {                                                       \
            if(cur_byte == byte)                                \
            {                                                   \
                printf("[%*d]%*s ", width(code_len - 1), i, max_name_len, #name); \
                fprintf(outfile, "%*s ", max_name_len, #name);  \
                for(int j = 0; j < argc; ++j)                   \
                {                                               \
                    ++i;                                        \
                    printf("%d ", code[i]);                     \
                    fprintf(outfile, "%d ", code[i]);           \
                }                                               \
                                                                \
                fprintf(outfile, "\t; [%*d]", width(code_len - 1), i); \
                printf("\n");                                   \
                fprintf(outfile, "\n");                         \
                                                                \
                if(byte == 0 || byte == 0xFF)                   \
                {                                               \
                    printf("\n");                               \
                    fprintf(outfile, "\n");                     \
                }                                               \
            }                                                   \
        }
        #include "../lib/asm_commands.h"
        #undef DEF_CMD

        i++;
    }

    fclose(outfile);

    printf("\t[  "GREEN"OK"RESET"  ]\n");
    return 0;
}
