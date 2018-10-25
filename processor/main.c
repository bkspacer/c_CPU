/** \file main.c
 */
#include "../lib/strings_int.h"
#include "processor.h"


int main()
{
    printf(BLUE"# Virtual processor (uses definite assembler language)\n"
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

    //* ECHO *//

    //print_code(code, code_len, 8);

    // Processing the code

    int result = process(code);
    if(result)
        printf(RED"FATAL ERROR\n"RESET);

    //**//

    DELETE(code);
    return result;
}
