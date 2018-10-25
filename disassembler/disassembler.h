/** \file disassembler.h
 */
#ifndef DISASSEMBLER_H_INCLUDED
#define DISASSEMBLER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../processor/processor.h"
#include "../lib/asm_language.h"


/** \brief Prints program interpreted from given code in console and in given file
 *
 * \param code int* [in] - code to print
 * \param code_len int [in] - number of bytes in the code
 * \param filename char* [in] - name of file to print the program into
 * \return int
 *
 */
int print_prog(int* code, int code_len, char* filename);


#endif // DISASSEMBLER_H_INCLUDED
