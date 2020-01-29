//
// Created by Quinn on 2020-01-28.
//

#pragma once

#include <endian.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "cache.h"

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

// Create a new statement based on the input buffer.
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement){

    // Check the statement types
    if(strncmp(input_buffer->stream, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        int arguments_assigned = sscanf(input_buffer->stream,
                "insert %d %s %s",  // Input format scanner. Will adjust this based on insert table.
                // Next params are the assignment of the % placeholders
                &(statement->insert_row.id),
                statement->insert_row.username,
                statement->insert_row.email);
        return PREPARE_SUCCESS;
    }

    // Check the statement types
    if(strncmp(input_buffer->stream, "select", 6) == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult execute_statement(Table* table, Statement* statement) {
    switch (statement->type) {
        case (STATEMENT_INSERT):
            return insert_row(table, &(statement->insert_row));
        case (STATEMENT_SELECT):
            return select_all(table);
    }
}
