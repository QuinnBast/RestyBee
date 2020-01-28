#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// InputBuffer struct.
// For reading user input from the command line
typedef struct {
    char* stream; // Char pointer that contains the user's input stream
    size_t buffer_length;
    size_t input_length;
} InputBuffer;

InputBuffer* new_input_buffer();    // Constructor for a new input buffer
void print_prompt();                // Prints the REPL input line
void read_input(InputBuffer*);      // Reads the user input from the REPL
void close_input_buffer(InputBuffer*);   // Ends the input buffer once the user has entered some values

int main() {
    InputBuffer* input_buffer = new_input_buffer();
    while(true) {
        print_prompt();
        read_input(input_buffer);

        if(strcmp(input_buffer->stream, ".exit") == 0) {
            close_input_buffer(input_buffer);
            exit(EXIT_SUCCESS);
        } else {
            printf("Unrecognized command '%s'.\n", input_buffer->stream);
        }
    }
}

// Function to create a new InputBuffer.
InputBuffer* new_input_buffer() {
    // Store some memory for the input buffer.
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->stream = NULL;
    input_buffer->buffer_length = 0;
    input_buffer-> input_length = 0;

    return input_buffer;
}

void print_prompt() {
    printf("dbREST > ");
}

void read_input(InputBuffer* input_buffer) {
    // Use getline to get the user's input.
    // get the stream and length from stdin
    // Note: This stores the stdin stream into the inputbuffer's stream. Need to ensure the memory is cleared after.
    size_t bytes_read = getline(&(input_buffer->stream), &(input_buffer->buffer_length), stdin);

    // If no bytes were read, throw an error
    if(bytes_read <= 0) {
        printf("Error reading input.\n");
        exit(EXIT_FAILURE);
    }

    // Set the input length & stream
    input_buffer->input_length = bytes_read - 1;
    input_buffer->stream[bytes_read - 1] = 0; // Replace the trailing newline character
}

void close_input_buffer(InputBuffer* input_buffer) {
    // Remove any space in the input buffer
    free(input_buffer->stream);
    free(input_buffer);
}

