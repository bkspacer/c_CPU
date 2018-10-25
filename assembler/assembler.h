/** \file assembler.h
 */
#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/strings_int.h"
#include "../lib/asm_language.h"

#define RED         "\033[0;31m"
#define GREEN       "\033[0;32m"
#define PURPLE      "\033[0;35m"
#define BLUE        "\033[0;34m"

#define RESET       "\033[0m"
#define UNDERLINE   "\033[4m"
#define ALERT       "\033[5m"

#define MAX_STR_LEN (100) /**< Maximum of string length */
#define MAX_LBL_NUM (100) /**< Maximum allowed variables in a program */
#define MAX_VAR_NUM (100) /**< Maximum allowed labels in a program */


/** \brief
 *
 * \param pos int - number of the byte to which the label is attached
 * \param name[MAX_STR_LEN] char - name of the label
 *
 */
struct Label
{                                                   /*it's more corretly to use union construction (described in K&N, para 6.8)*/
    int pos;                                        /*they were included into ANSI standard special for solving this type of problems*/
    char name[MAX_STR_LEN];
} Labels[MAX_LBL_NUM];

/** \brief
 *
 * \param val int - the actual value of the variable (needs type to be understood correctly)
 * \param type int - type of the variable (according to asm_commands.h)
 * \param name[MAX_STR_LEN] char - name of the variable
 *
 */
struct Variable
{
    int val;
    int type;
    char name[MAX_STR_LEN];
} Variables[MAX_VAR_NUM];

int num_of_labels;      /**< The number of labels that were found so far */
int num_of_variables;   /**< The number of variables that were found so far */
int assembling;         /**< (1) if it is the first assembling, (2) - if it the second one (importing labels values) */


//=====================================================================
//------------------PRINTING-FUNCTIONS---------------------------------
//=====================================================================

/** \brief Calculates length of a number when interpreted as string
 *
 * \param n int [in] - number to size
 * \return int - number of digits in the number
 *
 */
int width(int n);


/** \brief Gives all the information about labels that were found so far
 *
 */
void Labels_dump();


/** \brief Gives all the information about variables that were found so far
 *
 */
void Variables_dump();


/** \brief Prints binary code as [number of byte] (hex-form of the byte)
 *
 * \param code int* [in] - the code to print
 * \param byte_count int [in] - number of bytes in the code
 * \param bytes_in_line int [in] - number of bytes in a line
 *
 */
void print_code(int* code, int byte_count, int bytes_in_line);


//=====================================================================
//---------------------MINOR-FUNCTIONS---------------------------------
//=====================================================================

/** \brief Checks if all of symbols in given line are digits
 *
 * \param line char* [in] - position of of the string
 * \return (1) if no non-digit symbols detected; (0) if is a non-digit symbol
 *
 */
int alldigits(char* line);

/** \brief Checks if given number of first items of integer arrays are equal
 *
 * \param a const int* [in] - first array to compare
 * \param b const int* [in] - second array to compare
 * \param num int [in] - number of items to compare
 * \return (1) if first (num) numbers are equal; (0) if they are not
 *
 */
int equal_arrays(const int* a, const int* b, int num);


/** \brief Skips all 'space' symbols in given string
 *
 * \param line char* [in] - position of of the string
 * \return char* - position the first non-space symbol
 *
 * \note To find out what are 'space' symbols, check ctype.h
 *
 */
char* skip_space(char* line);


/** \brief Skips all 'alnum' or '_' symbols in given string
 *
 * \param line char* [in] - position of of the string
 * \return char* - position the first non-alnum-or-'_'  symbol
 *
 * \note To find out what are 'alnum' symbols, check ctype.h
 *
 */
char* skip_alnum_and_(char* line);


/** \brief Skips all 'space' symbols and one comma if it exists in given string
 *
 * \param line char* [in] - position of of the string
 * \return char* - position the first non-space-or-comma symbol
 *
 * \note To find out what are 'space' symbols, check ctype.h
 *
 */
char* skip_space_and_comma(char* line);


//=====================================================================
//----------------------MAIN-FUNCTIONS---------------------------------
//=====================================================================

/** \brief Interprets the first word from given as an argument, calculate it and determines its type
 *
 * \param line char* [in] - position of of the string
 * \param arg int* [out] - the argument which has been read out
 * \param argtype int* [out] - the type of the argument which has been read out
 *
 */
int get_arg(char* line, int* arg, int* argtype);


/** \brief Interprets given line as argument and calculates all of them
 *
 * \param line char* [in] - position of of the string
 * \param argc int* [out] - number of arguments that have been read out
 * \param argv int* [out] - arguments that have been read out
 * \param argtypes int* [out] - types of the arguments that have been read out
 *
 */
int load_argv(char* line, int* argc, int* argv, int* argtypes);


/** \brief Interprets given string as a line of an assembler program (according to asm_commands.h), converts it to binary code and adds to given code line
 *
 * \param line char* [in] - string to interpret
 * \param byte_num int* [in/out] - number of bytes in the code so far (before / after)
 * \param code int* [in] - the code to add new binary to
 * \return (1) if some bad error occurred; (0) if it did not
 *
 */
int interprete(char* line, int* byte_num, int* code);


/** \brief Interprets array of strings as as a program file (according to asm_commands.h), converts it to binary code
 *
 * \param prog_arr char** [in] - the array of program code lines
 * \param num_of_lines int [in] - number of lines in the array
 * \param code int* [out] - the result - binary code of the program code
 * \param byte_num int* [out] - number of bytes in the binary
 * \return (1) if some bad error occurred; (0) if it did not
 *
 */
int assemble(char** prog_arr, int num_of_lines, int* code, int* byte_num);


/** \brief Writes code in the given file (as binary)
 *
 * \param filename char* [in] - name of the file to load the code to
 * \param code int* [in] - code to print
 * \param byte_count int [in] - number of bytes in the code
 *
 */
void upload_code(char* filename, int* code, int byte_count);


#endif // ASSEMBLER_H_INCLUDED
