/** \file main.c
 */
#include "disassembler.h"
#include "../lib/strings_int.h"


int main()
{
    printf(BLUE"# Disassembler\n"
           "# (c) OlegZ 2018\n"RESET);

    //**

    char infilename[MAX_STR_LEN] = "../", actual_name[MAX_STR_LEN];

    printf(UNDERLINE"\nENTER INPUT FILENAME\n"RESET"==>\t"BLUE);

    scanf("%s", actual_name);
    strcat(infilename, actual_name);

    printf(RESET);

    //**

    size_t code_len;
    int* code = loadcode(infilename, &code_len);
    if(!code)
    {
        printf(RED"FATAL ERROR\n"RESET);
        return 1;
    }

    //**

    char outfilename[MAX_STR_LEN] = "../";

    printf(UNDERLINE"\nENTER OUTPUT FILENAME\n"RESET"==>\t"BLUE);

    scanf("%s", actual_name);
    strcat(outfilename, actual_name);

    printf(RESET);

    //**

    int result = print_prog(code, code_len, outfilename);
    if(result)
        printf(RED"FATAL ERROR\n"RESET);

    DELETE(code);
    return result;
}
