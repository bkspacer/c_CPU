#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../lib/strings_int.h"
#include "./../lib/osm_comands.h"


#define RESET       "\033[0m"
#define UNDERLINE   "\033[4m"
#define ALERT       "\033[5m"
#define RED         "\033[0;31m"
#define GREEN       "\033[0;32m"
#define PURPLE      "\033[0;35m"
#define BLUE        "\033[0;34m"

#define MAX_STR_LEN (100)
#define MAX_LBL_NUM (100)
#define MAX_VAR_NUM (100)


struct Label
{
    int pos;
    char name[MAX_STR_LEN];
} Labels[MAX_LBL_NUM];
int num_of_labels;

struct Variable
{
    int val;
    int type;
    char name[MAX_STR_LEN];
} Variables[MAX_VAR_NUM];
int num_of_variables;

int assembling;

char* skip_space(char* line)
{
    while(isspace(*line))
        line++;
    return line;
}

char* skip_alnum_and_(char* line)
{
    while(isalnum(*line) || *line == '_')
        line++;
    return line;
}

char* skip_space_and_comma(char* line)
{
    line = skip_space(line);
    if(*line == ',')
        return skip_space(line + 1);
    return line;
}

int alldigits(char* line)
{
    while(*line)
        if(!isdigit(*line++))
            return 0;
    return 1;
}

void get_arg(char* line, int* arg, int* argtype)
{
    char name[MAX_STR_LEN];
    sscanf(line, "%[^, ]s", name);

    if(strchr(name, ']'))
        *strchr(name, ']') = '\0';

    if(strchr(name, '+'))
        *strchr(name, '+') = '\0';

    for(int i = 0; i < num_of_labels; ++i)
        if(!strcmp(name, Labels[i].name))
        {
            *argtype = LABEL_t;
            *arg = Labels[i].pos;
            return;
        }

    if(alldigits(name))
    {
        *argtype = NUM_t;
        *arg = atoi(name);
        return;
    }

    if(tolower(*name) == 'r' && alldigits(name + 1))
    {
        *argtype = REG_t;
        *arg = atoi(name + 1);
        return;
    }

    for(int i = 0; i < num_of_variables; ++i)
        if(!strcmp(name, Variables[i].name))
        {
            *argtype = Variables[i].type;
            *arg = Variables[i].val;
            return;
        }

    if(assembling == 1)
        *argtype = LABEL_t;
    else
        printf(RED ALERT"\tUNKNOWN TYPE OF %s\n"RESET, name);
    return;
}

void load_argv(char* line, int* argc_out, int* argv, int* argtypes)
{
    int argc = 0;

    while(*line && *line != ';')
    {
        if(*line == '[')
        {
            line = skip_space(line + 1); // to argument

            get_arg(line, argv + argc, argtypes + argc);
            line = skip_alnum_and_(line); // skipping argument

            switch(argtypes[argc])
            {
            case NUM_t:
                argtypes[argc] = S_NUM_t;
                break;

            case REG_t:
                argtypes[argc] = S_REG_t;
                break;
            }

            line = skip_space(line); // to + or ]
            if(*line == '+')
            {
                argc++;
                line = skip_space(line + 1); // to argument

                get_arg(line, argv + argc, argtypes + argc);
                line = skip_alnum_and_(line); // skipping argument

                switch(argtypes[argc])
                {
                case NUM_t:
                    argtypes[argc] = S_NUM_t;
                    break;

                case REG_t:
                    argtypes[argc] = S_REG_t;
                    break;
                }

                line = skip_space(line); // to ]
            }
            line++; // skipping ]
        }
        else if(isalnum(*line) || *line == '_')
        {
            get_arg(line, argv + argc, argtypes + argc);
            line = skip_alnum_and_(line); // skipping argument
        }
        else
        {
            printf(RED ALERT"\tUNKNOWN ARGUMENT TYPE\n"RESET);
            return;
        }
        argc++;
        line = skip_space_and_comma(line); // to next argument
    }
    *argc_out = argc;
    return;
}

int equal_arrays(const int* a, const int* b, int num)
{
    for(int i = 0; i < num; ++i)
        if(a[i] != b[i])
            return 0;
    return 1;
}

void print_arg_type(int argtype)
{
    switch(argtype)
    {
    case NUM_t:
        printf("const");
        break;

    case REG_t:
        printf("register");
        break;

    case S_NUM_t:
        printf("const(SRAM pos)");
        break;

    case S_REG_t:
        printf("register(SRAM pos)");
        break;

    case LABEL_t:
        printf("label");
        break;
    }
    return;
}

void print_arg(int argtype, int arg)
{
    switch(argtype)
    {
    case NUM_t:
        printf("const %d", arg);
        break;

    case REG_t:
        printf("register R%d", arg);
        break;

    case S_NUM_t:
        printf("const %d (SRAM pos)", arg);
        break;

    case S_REG_t:
        printf("register R%d SRAM pos", arg);
        break;


    case LABEL_t:
        printf("label #%d", arg);
        break;
    }
    return;
}

int interprete(char* line, int* byte_num, int* code)
{
    if(strchr(line, ';'))
        *strchr(line, ';') = '\0';

    line = skip_space(line);
    if(!*line)
        return 0;

    printf(UNDERLINE"CODE LINE"RESET":\t%s\n", line);

    char com_name[MAX_STR_LEN];
    char name[MAX_STR_LEN];
    sscanf(line, "%s", com_name);

    if(strchr(line, ':'))
    {
        printf("\tidentified as a label\n");

        sscanf(line, "%[^: ]s", com_name);
        line = skip_space(strchr(line, ':') + 1);

        for(int i = 0; i < num_of_labels; ++i)
            if(!strcmp(com_name, Labels[i].name))
            {
                if(*byte_num != Labels[i].pos)
                {
                    printf(RED ALERT"THIS LABEL NAME IS ALREADY USED\n"RESET);
                    return 1;
                }

                line = skip_space(line);
                return interprete(line, byte_num, code);
            }

        Labels[num_of_labels].pos = *byte_num;
        sscanf(com_name, "%s", name);
        strcpy(Labels[num_of_labels].name, name);

        printf("\tLabels[%d] = %s (byte #%d)\n", num_of_labels, Labels[num_of_labels].name, *byte_num);

        ++num_of_labels;
        printf("\t[  "GREEN"OK"RESET"  ]\n");

        line = skip_space(line);
        return interprete(line, byte_num, code);
    }


    line = skip_alnum_and_(line);
    line = skip_space(line);

    int argv[100], argtypes[100], argc = 0;
    load_argv(line, &argc, argv, argtypes);

    printf("Command %s with ", com_name);

    if(argc)
    {
        printf("%d arguments with types: ", argc);
        for(int i = 0; i < argc; i++)
        {
            print_arg_type(argtypes[i]);
            printf(" ");
        }
        printf("\n");
    }
    else
        printf("no arguments\n");


    int i = 0;
    int command_found = 0;
    while(OSM_COMANDS[i].code != 0xFF)
    {
        if(!strcmp(OSM_COMANDS[i].name, com_name))
        {
            command_found = 1;
            if(OSM_COMANDS[i].argc == argc && equal_arrays(OSM_COMANDS[i].argtypes, argtypes, argc))
                break;
        }
        i++;
    }

    if(strcmp(OSM_COMANDS[i].name, com_name) ||
       !(OSM_COMANDS[i].argc == argc) ||
       !equal_arrays(OSM_COMANDS[i].argtypes, argtypes, argc))
    {
        if(argc == 1 && !command_found)
        {
            printf("\tidentified as a variable\n");

            for(int i = 0; i < num_of_variables; ++i)
                if(!strcmp(com_name, Variables[i].name))
                {
                    if(Variables[i].type != argtypes[0] ||
                       Variables[i].val != argv[0])
                       {
                           printf(RED ALERT"THIS VARIABLE NAME IS ALREADY USED\n"RESET);
                           return 1;
                       }
                    return 0;
                }

            Variables[num_of_variables].val = argv[0];
            Variables[num_of_variables].type = argtypes[0];
            strcpy(Variables[num_of_variables].name, com_name);

            printf("\tVariables[%d] %s = ", num_of_variables, Variables[num_of_variables].name);
            print_arg(Variables[num_of_variables].type, Variables[num_of_variables].val);
            printf("\n");

            num_of_variables++;
            printf("\t[  "GREEN"OK"RESET"  ]\n");
            return 0;
        }

        if(command_found)
        {
            printf(RED ALERT"\tINCORRECT NUMBER OF ARGUMENTS\n"RESET);
            return 1;
        }

        printf(RED ALERT"\tCOMMAND NOT FOUND\n"RESET);
        return 1;
    }

    printf("\tidentified as command %s (code 0x%02X)\n\t with ", OSM_COMANDS[i].name, OSM_COMANDS[i].code);
    *code++ = OSM_COMANDS[i].code;
    ++*byte_num;

    if(!OSM_COMANDS[i].argc)
    {
        printf("no arguments\n");
        printf("\t[  "GREEN"OK"RESET"  ]\n");
        return 0;
    }

    printf("%d arguments:\n", argc);
    for(int i = 0; i < argc; ++i)
    {
        *code++ = argv[i];
        printf("\t%d) ", i + 1);
        print_arg(argtypes[i], argv[i]);
        printf(" (0x%02X)\n", argv[i]);
        ++*byte_num;
    }

    printf("\t[  "GREEN"OK"RESET"  ]\n");
    return 0;
}

int assemble(char** prog_arr, int num_of_lines, int* code, int* byte_num)
{
    printf("\n================"UNDERLINE"FIRST ASSEMBLING"RESET"================\n");

    *byte_num = 0;
    int last_byte_num = 0;
    num_of_labels = 0;
    num_of_variables = 0;
    assembling = 1;

    for(int i = 0; i < num_of_lines; ++i)
    {
        if(interprete(prog_arr[i], byte_num, code + *byte_num))
            return 1;
        if(last_byte_num != *byte_num)
        {
            last_byte_num = *byte_num;
            printf(BLUE"==--%d bytes used--==\n"RESET, last_byte_num);
        }
    }
    printf("First assembling: [  "GREEN"OK"RESET"  ]\n");

    printf("\n================"UNDERLINE"SECOND ASSEMBLING"RESET"================\n");

    *byte_num = 0;
    assembling = 2;

    for(int i = 0; i < num_of_lines; ++i)
        if(interprete(prog_arr[i], byte_num, code + *byte_num))
            return 1;
    printf("Second assembling: [  "GREEN"OK"RESET"  ]\n");

    return 0;
}

void Labels_dump()
{
    printf("\n========"UNDERLINE"PROGRAM LABELS"RESET"========\n");
    if(num_of_labels == 0)
    {
        printf("\tno found labels\n");
        return;
    }

    for(int i = 0; i < num_of_labels; ++i)
        printf("\t%d) \"%s\" (byte #%d)\n", i + 1, Labels[i].name, Labels[i].pos);
    return;
}

void Variables_dump()
{
    printf("\n========"UNDERLINE"PROGRAM VARIABLES"RESET"========\n");
    if(num_of_variables == 0)
    {
        printf("\tno declared variables\n");
        return;
    }

    for(int i = 0; i < num_of_variables; ++i)
        printf("\t%d) %s %s = %c%d\n", i + 1, (Variables[i].type == NUM_t) ? "const   " : "register", Variables[i].name, (Variables[i].type == NUM_t) ? '\0' : 'R', Variables[i].val);
    return;
}

int width(int n)
{
    int i = 0;
    for(; n > 0; n /= 10, i++)
        ;
    return i;
}

void print_code(int* code, int byte_count, int bytes_in_line)
{
    printf("\n");
    for(int i = 0; i < byte_count; ++i)
        printf("[%*d] 0x%02X%c", width(byte_count - 1), i, code[i], ((i + 1) % bytes_in_line == 0 || i == byte_count - 1) ? '\n' : ' ');
    return;
}

void upload_code(char* filename, int* code, int byte_count)
{
    printf(UNDERLINE"\nUPLOADING TO FILE"RESET" \"%s\": ", filename);
    FILE* codefile = fopen(filename, "wb");
    for(int ip = 0; ip < byte_count; ++ip)
        putc(code[ip], codefile);
    fclose(codefile);

    printf("[  "GREEN"OK"RESET"  ]\n");
    return;
}

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

    int* code = (int*) calloc(3 * num_of_lines, sizeof(int));
    int byte_count;

    assemble(prog_arr, num_of_lines, code, &byte_count);

    //**

    Labels_dump();
    Variables_dump();
    print_code(code, byte_count, 7);

    //** Uploading to binary file

    upload_code("./../code2_b", code, byte_count);

    //**

    DELETE(code);
    DELETE(program);
    DELETE(prog_arr);
    return 0;
}
