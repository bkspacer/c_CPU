/** \file stack.h
 */
#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DEBUG

#define RESET   "\033[0m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"

#define ERROR   (0)
#define OK      (1)

#define LEN_OF_CANARY (3) /**< Length of used canary */

#define ECHO_STR( var )         (printf("%s = \"%s\"", strchr(#var, '>') + 1, var))
#define ECHO_INT( var )         (printf("%s = %d"    , strchr(#var, '>') + 1, var))
#define ECHO_DBL( var )         (printf("%s = %f"    , strchr(#var, '>') + 1, var))
#define ECHO_VAR( var, format) ({printf("%s = "      , strchr(#var, '>') + 1     ); \
                                 printf(      format                        , var);})

#ifdef DEBUG
#define ASSERT_OK( ) ({if(!Stack_OK(this)){\
                                Stack_dump(this); \
                                assert(!"Object not OK");\
                            return ERROR;}});
#else
#define ASSERT_OK( ) {if(!Stack_OK(this)) return ERROR;}
#endif


typedef double stack_val_type;/**< Type of items in stack, may be changed to char/int/double in the code file*/

/** \brief Realization of stack as a structure
 *
 *
 * \param int using_state - current state of using stack: RELEASE / TESTING / DEBUG
 *
 * \param char name[50] - name of the stack by which it is used in the function that created the stack
 * \param int size - current number of items in the stack
 * \param capacity - current maximum of size
 * \param stack_val_type hash_sum - control sum that is calculated every time stack is modified legally
 *
 * \param stack_val_type* data - place of all the stack info
 * \param stack_val_type* items - actual place of stack items
 *
 * \param char print_format[3] - format by which items are printed
 * \param stack_val_type canary[LEN_OF_CANARY] - canary of the stack
 *
 * \note Operates with char/int/double only
 */
struct Stack
{
    char name[50];
    int size;
    int capacity;
    stack_val_type hash_sum;

    stack_val_type* data;
    stack_val_type* items;

    char print_format[3];
    stack_val_type canary[LEN_OF_CANARY];
};

//==========__CONSTRUCTOR__===========
#define Stack_construct( var ) (Stack_construct_with_name(&var, __FUNCTION__, #var))
/** \brief Initializes stack
 *
 * \param this struct Stack* [in] - place of stack to initialize
 * \param creating_function const char* [in] - name of the function that called stack construction
 * \param stack_name const char* [in] - name of the stack
 *
 * \note the define "Stack_construct" allows users to fill last 2 parameters automatically
 * \warning Must to use before working with stack
 */
void Stack_construct_with_name(struct Stack* this, const char* creating_function, const char* stack_name);

//==========__DESTRUCTOR__============
/** \brief Destroys stack
 *
 * \param this struct Stack* [in] - place of stack to destroy
 *
 * \warning Must to use after working with stack
 */
void Stack_destruct(struct Stack* this);

//==========__PUSH__==================
/** \brief Pushes new item into stack
 *
 * \param this struct Stack* [in] - stack to push the item into
 * \param new_item stack_val_type [in] - new item to be pushed
 * \return (ERROR) - if an error occurred, else (OK)
 *
 * \warning Calls ABORT if (using_state) is RELEASE or DEBUG
 */
int Stack_push(struct Stack* this, stack_val_type new_item);

//==========__POP__===================
/** \brief Pops last item from stack if available
 *
 * \param this struct Stack* [in] - stack to pop from
 * \param new_item stack_val_type [out] - the item that was popped
 * \return (ERROR) - if an error occurred, else (OK)
 *
 * \warning Calls ABORT if (using_state) is RELEASE or DEBUG
 */
int Stack_pop(struct Stack* this, stack_val_type* out_item);

//==========__DUMP__==================
/** \brief Prints detailed info if (using_state) = DEBUG
 *
 * \param this const struct Stack* [in] - stack to be printed
 */
void Stack_dump(const struct Stack* this);

//==========__OK__====================
/** \brief Checks either the stack works fine or not
 *
 * \param this const struct Stack* [in] - stack to check
 * \return (ERROR) if something is wrong with the stack
 */
int Stack_OK(const struct Stack* this);

//==========__HASH__==================
/** \brief Calculates current hash sum of the (items) array of the stack
 *
 * \param this const structStack* [in] - stack which sum must be calculated
 * \return stack_val_type - current hash sum
 *
 * \note Returns (0) if (size) < 0
 */
stack_val_type Stack_hash(const struct Stack* this);

//==========__SET_CANARY__============
/** \brief Initializes canary for the stack
 *
 * \param this struct Stack* [in] - stack to create canary for
 */
void Stack_set_canary(struct Stack* this);

#endif // STACK_H_INCLUDED
