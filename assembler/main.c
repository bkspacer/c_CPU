#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"


int main()
{
    //** Reading program

    int num_of_lines = 0;

    char* program = fgetfile("./../linear_solver2.txt", &num_of_lines);
    if(!program)
        return 1;

    char** prog_arr = slicedtext(program, num_of_lines);
    if(!prog_arr)
    {
        DELETE(program);
        return 1;
    }

    //**

    printf(UNDERLINE"PROGRAMM FILE\n"RESET);
    printtext(prog_arr, num_of_lines);

    //** Assembling

    int* code = (int*) calloc(3 * num_of_lines, sizeof(int)); // There are no functions that need more than 3 arguments
    int byte_count;

    assemble(prog_arr, num_of_lines, code, &byte_count);

    //**

    Labels_dump();
    Variables_dump();
    print_code(code, byte_count, 7);

    //** Uploading to binary file

    upload_code("./../code_b", code, byte_count);

    //**

    DELETE(code);
    DELETE(program);
    DELETE(prog_arr);
    return 0;
}
