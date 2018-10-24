#include "stack.h"


//==========__HASH__==================
// Calculates current hash sum of the (items) array of the stack
stack_val_type Stack_hash(const struct Stack* this)
{
    stack_val_type sum = 0;

    for(int i = 0, sign = 1; i < this->size; i++, sign = -sign)
        sum += sign * (i + 1) * (this->items[i]);
    return sum;
}

//==========__SET_CANARY__============
// Initializes canary for the stack
void Stack_set_canary(struct Stack* this)
{
    for(int i = 0; i < LEN_OF_CANARY; ++i)
        this->data[this->capacity + 2 * LEN_OF_CANARY - 1 - i] = this->data[i] = this->canary[i];
    return;
}

//==========__CONSTRUCTOR__===========
// Initializes stack
void Stack_construct_with_name(struct Stack* this, const char* creating_function, const char* stack_name)
{
    assert(this);
    assert(creating_function);
    assert(stack_name);

    strcpy(this->name, creating_function);
    strcat(this->name, "(): ");
    strcat(this->name, stack_name);

    this->size = 0;

    this->capacity = 1;

    this->data = (stack_val_type*) calloc(this->capacity + 2 * LEN_OF_CANARY, sizeof(stack_val_type));
    this->items = this->data + LEN_OF_CANARY;

    this->print_format[0] = '%';
    this->print_format[2] ='\0';
    switch (sizeof(stack_val_type))
    {
    case sizeof(char):
        this->print_format[1] = 'c';
        break;

    case sizeof(int):
        this->print_format[1] = 'd';
        break;

    case sizeof(double):
        this->print_format[1] = 'f';
        break;

    default:
        this->print_format[1] = '%';
        break;
    }

    for(int i = 0; i < LEN_OF_CANARY; ++i)
        this->canary[i] = (stack_val_type) (42 - i);

    Stack_set_canary(this);

    //this->hash_sum -= this->items[this->size];
    //this->hash_sum = Stack_hash(this);
    this->hash_sum = 0;
    return;
}

//==========__DESTRUCTOR__============
// Destroys stack
void Stack_destruct(struct Stack* this)
{
    assert(this);

    this->size = -1;

    this->capacity = -1;

    free(this->data);
    this->data = NULL;

    return;
}

//==========__PUSH__==================
// Pushes new item into stack
int Stack_push(struct Stack* this, stack_val_type new_item)
{
    ASSERT_OK();

    if(this->size >= this->capacity)
    {
        this->capacity *= 2;
        this->data = (stack_val_type*) realloc(this->data, (this->capacity + 2 * LEN_OF_CANARY) * sizeof(stack_val_type));
        this->items = this->data + LEN_OF_CANARY;
        Stack_set_canary(this);
    }

    this->items[this->size++] = new_item;

    this->hash_sum -= ((this->size % 2 == 0) ? 1 : -1) * (this->size) * this->items[this->size - 1];
    ASSERT_OK();

    return OK;
}

//==========__POP__===================
// Pops last item from stack if available
int Stack_pop(struct Stack* this, stack_val_type* out_item)
{
    assert(out_item);
    ASSERT_OK();

    if(this->size - 1 <= this->capacity / 4)
    {
        this->capacity /= 2;
        if(!this->capacity)
            this->capacity = 1;
        this->data = (stack_val_type*) realloc(this->data, (this->capacity + 2 * LEN_OF_CANARY) * sizeof(stack_val_type));
        this->items = this->data + LEN_OF_CANARY;
        Stack_set_canary(this);
    }

    --this->size;

    this->hash_sum -= ((this->size % 2 == 0) ? 1 : -1) * (this->size + 1) * this->items[this->size];
    ASSERT_OK();

    *out_item = this->items[this->size];

    return OK;
}

//==========__OK__====================
// Checks either the stack works fine or not
int Stack_OK(const struct Stack* this)
{
    if(this->size < 0)
        return 0;

    if(this->size > this->capacity)
        return 0;

    if(this->capacity < 0)
        return 0;

    if(this->print_format[1] == '%')
        return 0;

    //if(this->hash_sum != Stack_hash(this))return 0;

    for(int i = 0; i < LEN_OF_CANARY; ++i)
        if(this->data[i] != this->canary[i] || this->data[this->capacity + 2 * LEN_OF_CANARY - 1 - i] != this->canary[i])
            return 0;

    if(!this->data)
        return 0;

    return 1;
}

//==========__DUMP__==================
// Prints detailed info if (using_state) = DEBUG
void Stack_dump(const struct Stack* this)
{
    printf("\nStack \"%s\": ", this->name);
    if(Stack_OK(this))
        printf("[%s%s%s]\n", GREEN, "  OK  ", RESET);
    else
        printf("[%s%s%s]\n", RED  , "GLITCH", RESET);

    //if(!Stack_OK(this))
    {
        printf("{\n");

        { // size
            printf("\t");
            ECHO_INT(this->size);

            if(this->size < 0)
                printf(RED"\n\t\t! size (%d) < 0"RESET, this->size);

            if(this->size > this->capacity)
                printf(RED"\n\t\t! size (%d) > capacity (%d)"RESET, this->size, this->capacity);

            printf("\n");
        }

        { // capacity
            printf("\t");
            ECHO_INT(this->capacity);

            if(this->capacity <= 0)
                printf(RED"\n\t\t! capacity (%d) <= 0"RESET, this->capacity);

            printf("\n");
        }

        { // print_format
            printf("\t");
            ECHO_STR(this->print_format);

            if(this->print_format[1] == '%')
                printf(RED"\n\t\t! undefined printing format"RESET);

            printf("\n");
        }

        /*if(!this->data)
        {
            printf("\tdata pointer is NULL\n");
        }
        else*/
        { // data
            printf("\tdata [%d]:\n"
                   "\t{\n", this->capacity + 2 * LEN_OF_CANARY);
            int i_width = 0;
            for(int fullsize = this->capacity + 2 * LEN_OF_CANARY - 1; fullsize > 0; fullsize /= 10, ++i_width)
                ;

            if(LEN_OF_CANARY > 0)
            { // canary
                printf("\t\tcanary [%d]:\n"
                       "\t\t{\n", LEN_OF_CANARY);
                for(int i = 0; i < LEN_OF_CANARY; ++i)
                {
                    printf("\tdata[%*d] = \t\t  [%*d] = ", i_width, i, i_width, i);
                    printf(this->print_format, this->data[i]);
                    if(this->data[i] != this->canary[i])
                        printf(RED"\t! canary is dead"RESET);
                    printf("\n");
                }
                printf("\t\t}\n");
            }

            { // items
                printf("\t\titems [%d]:\n"
                       "\t\t{\n", this->capacity);
                for(int i = 0; i < this->capacity; ++i)
                {
                    printf("\tdata[%*d] = \t\t%c [%*d] = ", i_width, i + LEN_OF_CANARY, (i < this->size) ? '*' : ' ', i_width, i);
                    printf(this->print_format, this->items[i]);
                    printf("\n");
                }
                printf("\t\t}\n");
            }

            if(LEN_OF_CANARY > 0)
            { // canary
                printf("\t\tcanary [%d]:\n"
                       "\t\t{\n", LEN_OF_CANARY);
                for(int i = 0; i < LEN_OF_CANARY; ++i)
                {
                    printf("\tdata[%*d] = \t\t  [%*d] = ", i_width, i + LEN_OF_CANARY + this->capacity, i_width, i);
                    printf(this->print_format, this->data[i + LEN_OF_CANARY + this->capacity]);
                    if(this->data[this->capacity + LEN_OF_CANARY + i] != this->canary[LEN_OF_CANARY - 1 - i])
                        printf(RED"\t! canary is dead"RESET);
                    printf("\n");
                }
                printf("\t\t}\n");
            }
            printf("\t}\n");
        }

        { // hash_sum
            printf("\t");
            ECHO_VAR(this->hash_sum, this->print_format);

            if(this->hash_sum != Stack_hash(this))
            {
                printf(RED"\n\t\t! current sum = ");
                printf(this->print_format, Stack_hash(this));
                printf("\n\t\t! items array seems to have been changed"RESET);
            }

            printf("\n");
        }

        printf("}\n");
    }
    return;
}

