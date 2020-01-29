#pragma once
#include <stdint.h>
#include "typedefs.h"

InputBuffer* new_input_buffer();    // Constructor for a new input buffer
void print_prompt();                // Prints the REPL input line
void read_input(InputBuffer*);      // Reads the user input from the REPL
void close_input_buffer(InputBuffer*);   // Ends the input buffer once the user has entered some values
PrepareResult prepare_statement(InputBuffer*, Statement*);
ExecuteResult execute_statement(Table*, Statement*);