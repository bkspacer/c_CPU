#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include "../lib/stack.h"


/** \brief Prints code in hex form: | 0x** 0x** 0x** 0x** |
 *
 * \param code const int* [in] - code to print
 * \param code_len int [in] - number of bytes in the code
 * \param bytes_in_line int [in] - number of bytes in a line
 *
 */
void print_code(const int* code, int code_len, int bytes_in_line);


/** \brief Loads code from given binary file
 *
 * \param filename char* [in] - name of the code file to read from
 * \param code_len size_t* [in] - number of bytes in the code
 * \return int* - the binary code that has been read, divided to bytes
 *
 */
int* loadcode(char* filename, size_t* code_len);


/** \brief Processes the given code according to asm_commands.h
 *
 * \param PC0 int* [in] - the code to process
 * \return (1) if a bad error occurred; (0) if it did not
 *
 */
int process(int* PC0);


#endif // PROCESSOR_H_INCLUDED
