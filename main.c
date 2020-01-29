#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// File imports
#include "parser.h"
#include "cache.h"

int main(int argc, char* argv[]) {
    // TODO: Add command line arguments here with argv[]

    InputBuffer* input_buffer = new_input_buffer();
    Table* table = create_table();

    printf("Table rows %d", table->num_rows);
    while(true) {
        print_prompt();
        read_input(input_buffer);

        if(strcmp(input_buffer->stream, ".exit") == 0) {
            close_input_buffer(input_buffer);
            exit(EXIT_SUCCESS);
        } else {
            // Determine if the input is a system command or a database operation
            // System commands will start with a .
            if (input_buffer->stream[0] == '.') {
                printf("Do system command here.\n");
            } else {
                // Parser logic to determine what command was input
                Statement statement;
                switch(prepare_statement(input_buffer, &statement)){
                    case PREPARE_SUCCESS:
                        switch(execute_statement(table, &statement)){
                            case EXECUTE_SUCCESS:
                                printf("Operation success.\n");
                                break;
                            case EXECUTE_TABLE_FULL:
                                printf("Table full.\n");
                                break;
                            case EXECUTE_FAILURE:
                            default:
                                printf("Unexpected Failure.\n");
                                break;

                        }
                        break;
                    case PREPARE_UNRECOGNIZED_STATEMENT:
                    default:
                        printf("Unrecognized command '%s'.\n", input_buffer->stream);
                        break;
                }
            }
        }
    }
}