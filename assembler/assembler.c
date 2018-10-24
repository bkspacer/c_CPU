#include "assembler.h"
#include "./../lib/osm_comands.h"


//=====================================================================
//------------------PRINTING-FUNCTIONS---------------------------------
//=====================================================================

int width(int n)
{
    int i = 0;
    for(; n > 0; n /= 10, i++)
        ;
    return i;
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
        printf("\t%*d) \"%s\" - byte #%d\n", width(num_of_labels - 1), i + 1, Labels[i].name, Labels[i].pos);

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
    {
        printf("\t%*d) \"%s\" - ", width(num_of_labels - 1), i + 1, Variables[i].name);
        print_arg(Variables[i].type, Variables[i].val);
        printf("\n");
    }

    return;
}

void print_code(int* code, int byte_count, int bytes_in_line)
{
    assert(code != NULL);

    printf("\n========"UNDERLINE"CODE"RESET"========\n");

    for(int i = 0; i < byte_count; ++i)
        printf("[%*d] 0x%02X%c", width(byte_count - 1), i, code[i], ((i + 1) % bytes_in_line == 0 || i == byte_count - 1) ? '\n' : ' ');

    return;
}


//=====================================================================
//---------------------MINOR-FUNCTIONS---------------------------------
//=====================================================================

int alldigits(char* line)
{
    assert(line != NULL);

    while(*line)
        if(!isdigit(*line++))
            return 0;
    return 1;
}

int equal_arrays(const int* a, const int* b, int num)
{
    assert(a != NULL);
    assert(b != NULL);
    assert(a != b);

    for(int i = 0; i < num; ++i)
        if(a[i] != b[i])
            return 0;
    return 1;
}

char* skip_space(char* line)
{
    assert(line != NULL);

    while(isspace(*line))
        line++;
    return line;
}

char* skip_alnum_and_(char* line)
{
    assert(line != NULL);

    while(isalnum(*line) || *line == '_')
        line++;
    return line;
}

char* skip_space_and_comma(char* line)
{
    assert(line != NULL);

    line = skip_space(line);
    if(*line == ',')
        return skip_space(line + 1);
    return line;
}


//=====================================================================
//----------------------MAIN-FUNCTIONS---------------------------------
//=====================================================================

int get_arg(char* line, int* arg, int* argtype)
{
    assert(line != NULL);
    assert(arg != NULL);
    assert(argtype != NULL);
    assert(arg != argtype);

    char name[MAX_STR_LEN];
    sscanf(line, "%[^, ]s", name);

    if(strchr(name, ']'))
        *strchr(name, ']') = '\0';

    if(strchr(name, '+'))
        *strchr(name, '+') = '\0';

    // if it is a label
    for(int i = 0; i < num_of_labels; ++i)
        if(!strcmp(name, Labels[i].name))
        {
            *argtype = LABEL_t;
            *arg = Labels[i].pos;
            return 0;
        }

    // if it is just a number
    if(alldigits(name))
    {
        *argtype = NUM_t;
        *arg = atoi(name);
        return 0;
    }

    // if it is a register position
    if(tolower(*name) == 'r' && alldigits(name + 1))
    {
        *argtype = REG_t;
        *arg = atoi(name + 1);
        return 0;
    }

    // if it is a variable
    for(int i = 0; i < num_of_variables; ++i)
        if(!strcmp(name, Variables[i].name))
        {
            *argtype = Variables[i].type;
            *arg = Variables[i].val;
            return 0;
        }

    if(assembling == 1)
        // it may be OK
        *argtype = LABEL_t;
    else
    {
        // it is definitely not OK
        printf(RED ALERT"\tUNKNOWN TYPE OF %s\n"RESET, name);
        return 1;
    }


    return 0;
}

int load_argv(char* line, int* argc, int* argv, int* argtypes)
{
    assert(line != NULL);
    assert(argc != NULL);
    assert(argv != NULL);
    assert(argtypes != NULL);

    int argc_ = 0;

    while(*line)
    {
        // a SRAM position
        if(*line == '[')
        {
            line = skip_space(line + 1); // to argument

            get_arg(line, argv + argc_, argtypes + argc_);
            line = skip_alnum_and_(line); // skipping argument

            switch(argtypes[argc_])
            {
            case NUM_t:
                argtypes[argc_] = S_NUM_t;
                break;

            case REG_t:
                argtypes[argc_] = S_REG_t;
                break;
            }

            line = skip_space(line); // to + or ]
            if(*line == '+')
            {
                // its [Rr + k] form
                argc_++;
                line = skip_space(line + 1); // to argument

                if(get_arg(line, argv + argc_, argtypes + argc_))
                    return 1;
                line = skip_alnum_and_(line); // skipping argument

                switch(argtypes[argc_])
                {
                case NUM_t:
                    argtypes[argc_] = S_NUM_t;
                    break;

                case REG_t:
                    argtypes[argc_] = S_REG_t;
                    break;
                }

                line = skip_space(line); // to ]
            }
            line++; // skipping ]
        }
        // a simple argument
        else if(isalnum(*line) || *line == '_')
        {
            if(get_arg(line, argv + argc_, argtypes + argc_))
                return 1;
            line = skip_alnum_and_(line); // skipping argument
        }
        // some weird symbol
        else
        {
            printf(RED ALERT"\tUNKNOWN ARGUMENT TYPE\n"RESET);
            return 1;
        }

        argc_++;
        line = skip_space_and_comma(line); // to next argument
    }

    *argc = argc_;
    return 0;
}

int interprete(char* line, int* byte_num, int* code)
{
    assert(line != NULL);
    assert(byte_num != NULL);
    assert(code != NULL);

    if(strchr(line, ';'))
        *strchr(line, ';') = '\0'; // deleting comments

    line = skip_space(line); // now line[0] is the first symbol
    if(!*line)
        return 0;

    printf(UNDERLINE"CODE LINE"RESET":\t%s\n", line);

    char com_name[MAX_STR_LEN];
    sscanf(line, "%s", com_name); // reading the first word

    // if command is a new label name
    if(strchr(line, ':'))
    {
        printf("\tidentified as a label\n");

        if(num_of_labels >= MAX_LBL_NUM && assembling == 1)
        {
            printf(RED ALERT"\tNO MORE LABELS ALLOWED\n"RESET);
            return 1;
        }

        sscanf(line, "%[^: ]s", com_name);
        line = skip_space(strchr(line, ':') + 1);

        // checking if such label already exists
        for(int i = 0; i < num_of_labels; ++i)
            if(!strcmp(com_name, Labels[i].name))
            {
                if(!strcmp(Labels[i].name, com_name) && assembling == 1)
                {
                    printf(RED ALERT"\tTHIS LABEL NAME IS ALREADY USED\n"RESET);
                    return 1;
                }

                line = skip_space(line);
                return interprete(line, byte_num, code);
            }

               Labels[num_of_labels].pos = *byte_num;
        strcpy(Labels[num_of_labels].name,  com_name);

        printf("\tLabels[%d] = %s (byte #%d)\n", num_of_labels, Labels[num_of_labels].name, *byte_num);

        ++num_of_labels;

        printf("\t[  "GREEN"OK"RESET"  ]\n");

        line = skip_space(line);
        return interprete(line, byte_num, code);
    }

    line = skip_alnum_and_(line); // skipping command name
    line = skip_space(line); // skipping to first argument

    int argv[MAX_OF_ARGC], argtypes[MAX_OF_ARGC], argc = 0;
    if(load_argv(line, &argc, argv, argtypes))
        return 1;

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

    int command_found = 0;
    #define DEF_CMD( name, byte, numofargs, instruction) \
    {\
        if(!strcmp(#name, com_name)) \
        {\
            printf("\tidentified as command %s (code 0x%02X)\n\t with ", #name, byte);\
            *code++ = byte;\
            ++*byte_num;\
            command_found = 1;\
        }\
    }
    #include "../lib/asm_commands.h"
    #undef DEF_CMD

    // checking if the command was not found
    if(!command_found)
    {
        //if there is one argument, it may be a new variable
        if(argc == 1)
        {
            printf("\tidentified as a variable\n");

            if(num_of_variables >= MAX_VAR_NUM && assembling == 1)
            {
                printf(RED ALERT"\tNO MORE VARIABLES ALLOWED\n"RESET);
                return 1;
            }

            // checking if such variable already exists
            for(int i = 0; i < num_of_variables; ++i)
                if(!strcmp(Variables[i].name, com_name))
                {
                    if(Variables[i].type != argtypes[0])
                    {
                        printf(RED ALERT"\tTHIS VARIABLE NAME IS ALREADY USED\n"RESET);
                        return 1;
                    }

                    return 0;
                }
                else
                {
                    if(Variables[i].type == argtypes[0] &&
                       Variables[i].type == REG_t &&
                       Variables[i].val == argv[0])
                    {
                        printf(RED ALERT"\tTHIS REGISTER VARIABLE IS ALREADY USED SOMEWHERE\n"RESET);
                        return 1;
                    }
                }

                   Variables[num_of_variables].val  = argv[0];
                   Variables[num_of_variables].type = argtypes[0];
            strcpy(Variables[num_of_variables].name,  com_name);

            printf("\tVariables[%d] %s = ", num_of_variables, Variables[num_of_variables].name);
            print_arg(Variables[num_of_variables].type, Variables[num_of_variables].val);
            printf("\n");

            num_of_variables++;
            printf("\t[  "GREEN"OK"RESET"  ]\n");
            return 0;
        }

        printf(RED ALERT"\tCOMMAND NOT FOUND\n"RESET);
        return 1;
    }

    // so we have found the command

    if(argc)
    {
        printf("%d arguments:\n", argc);
        for(int i = 0; i < argc; ++i)
        {
            *code++ = argv[i];
            ++*byte_num;

            printf("\t%d) ", i + 1);
            print_arg(argtypes[i], argv[i]);
            printf(" (0x%02X)\n", argv[i]);
        }
    }
    else
        printf("no arguments\n");

    printf("\t[  "GREEN"OK"RESET"  ]\n");
    return 0;
}

int assemble(char** prog_arr, int num_of_lines, int* code, int* byte_num)
{
    assert(prog_arr != NULL);
    assert(code != NULL);
    assert(byte_num != NULL);

    printf("\n================"UNDERLINE"FIRST ASSEMBLING"RESET"================\n");

    *byte_num = 0;
    int last_byte_num = 0;
    num_of_labels = 0;
    num_of_variables = 0;
    assembling = 1;

    for(int i = 0; i < num_of_lines; ++i)
    {
        if(interprete(prog_arr[i], byte_num, code + *byte_num))
        {
            printf("First assembling: ["RED"FAILED"RESET"]\n");
            return 1;
        }


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
        {
            printf("Second assembling: ["RED"FAILED"RESET"]\n");
            return 1;
        }

    printf("Second assembling: [  "GREEN"OK"RESET"  ]\n");

    return 0;
}

void upload_code(char* filename, int* code, int byte_count)
{
    assert(filename != NULL);
    assert(code != NULL);

    printf(UNDERLINE"\nUPLOADING TO FILE"RESET" \"%s\": ", filename);

    FILE* codefile = fopen(filename, "wb");
    if(!codefile)
    {
        printf(RED ALERT"NO SUCH FILE IN THE DIRECTORY"RESET);
        return;
    }

    for(int ip = 0; ip < byte_count; ++ip)
        putc(code[ip], codefile);
    fclose(codefile);

    printf("[  "GREEN"OK"RESET"  ]\n");
    return;
}

