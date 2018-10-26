/** \file main.c
 */
#include "assembler.h"
#include "../lib/strings_int.h"


int main()
{
    print_assembler_info();

    printf(RED"! IF YOU HAVE NOT YET READ THE LANGUAGE MANUAL, CHECK \"FULL_ASM_MANUAL.txt\"\n"
           "! READ \"BRIEF_ASM_MANUAL.txt\" TO CHECK ALL DESCRIBED FUNCTIONS\n\n"RESET);

    printf(BLUE"# Assembler\n"
           "# (c) OlegZ 2018\n"RESET);

    //**

    char infilename[MAX_STR_LEN] = "../", actual_name[MAX_STR_LEN];

    printf(UNDERLINE"\nENTER INPUT FILENAME\n"RESET"==>\t"BLUE);

    scanf("%s", actual_name);
    strcat(infilename, actual_name);

    printf(RESET);

    //**

    int num_of_lines = 0;

    char* program = fgetfile(infilename, &num_of_lines);
    if(!program)
    {
        printf(RED ALERT"FATAL ERROR"RESET);
        return 1;
    }


    char** prog_arr = slicedtext(program, num_of_lines);
    if(!prog_arr)
    {
        printf(RED ALERT"FATAL ERROR"RESET);
        DELETE(program);
        return 1;
    }

    //**

    //printf(UNDERLINE"PROGRAMM FILE\n"RESET); printtext(prog_arr, num_of_lines);

    //** Assembling

    int* code = (int*) calloc(MAX_OF_ARGC * num_of_lines, sizeof(int)); // There are no functions that need more than 3 arguments
    int byte_count;

    int result = assemble(prog_arr, num_of_lines, code, &byte_count);

    //**

    Labels_dump();
    Variables_dump();
    if(!result)
        print_code(code, byte_count, 7);

    //** Uploading to binary file
    char outfilename[MAX_STR_LEN] = "../";

    printf(UNDERLINE"\nENTER OUTPUT FILENAME\n"RESET"==>\t"BLUE);

    scanf("%s", actual_name);
    strcat(outfilename, actual_name);

    printf(RESET);

    //**

    if(!result)
        upload_code(outfilename, code, byte_count);
    else
        printf(RED ALERT"FATAL ERROR"RESET);

    //**

    DELETE(code);
    DELETE(program);
    DELETE(prog_arr);
    return result;
}
