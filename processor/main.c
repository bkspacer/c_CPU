#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "../lib/strings_int.h"
#include "processor.h"


int main()
{
    // Reading code from binary file

    size_t code_len;
    int* code = loadcode("./../code_b", &code_len);

    //* ECHO *//

    print_code(code, code_len, 8);
    printf("\n");

    // Processing the code

    int result = process(code);

    //**//

    DELETE(code);
    return result;
}
