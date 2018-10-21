#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "../lib/stack.h"
#include "../lib/osm_comands.h"
#include "../lib/strings_int.h"


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

void print_code(int* code, int code_len, int bytes_in_line)
{
    printf("\n========"UNDERLINE"CODE FILE"RESET"========\n");
    for(size_t i = 0; i < code_len; ++i)
        printf("0x%02X %c%c", code[i], ((i + 1) % 4 == 0) ? '|' : ' ', ((i + 1) % bytes_in_line == 0 || i == code_len - 1) ? '\n' : ' ');
    return;
}

int main()
{
    size_t code_len;
    int* code = loadcode("./../code2_b", &code_len);

    //**//

    print_code(code, code_len, 8);
    printf("\n");

    //**//

    int result = process(code);

    //**//

    DELETE(code);
    return result;
}
