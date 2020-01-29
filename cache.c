//
// Created by Quinn on 2020-01-28.
//
#pragma once

#include <string.h>
#include "parser.h"
#include "cache.h"
#include <stdlib.h>
#include <stdio.h>

const uint32_t ID_SIZE = size_of_attribute(Row, id); // Can't just use the array size of the char array, char != byte
const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
const uint32_t ID_OFFSET = 0;
const uint32_t PAGE_SIZE = 4096;

// Hard-coded memory storage for the table structure we have defined. This will become dynamic
void serialize_row(Row* source, void* destination) {
    // Destination is a pointer to an address in memory. Void because we don't know what the data type is and don't care.
    // We just care the destination is free.

    // Store at the destination. Keep track of offset for each column to ensure columns don't overwrite.
    memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
    memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
    memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

// Takes memory from the source location and stores it into the program to print back.
void deserialize_row(void* source, Row* destination) {

    // Stores the source location's data from our offset back into the destination object in memory.
    memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
    memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

// Determine where to store a row in the table
void* get_memory_location(Table* table, uint32_t row_number) {
    // Determine what page we need to write to
    uint32_t page_num = row_number / ROWS_PER_PAGE;

    // Get the page
    void* page = table->pages[page_num];
    if (page == NULL) {
        // Allocate memory only when we try to access page
        page = table->pages[page_num] = malloc(PAGE_SIZE);
    }
    // If we access row 1414 and there are only 1000 rows per page, we actually want row 414 of the second page.
    // Get the row offset.
    uint32_t row_offset = row_number % ROWS_PER_PAGE;
    // Determine, based on the offset of the row, what byte we should read based on the row size.
    uint32_t byte_offset = row_offset * ROW_SIZE;

    // Return a pointer to the start of the row.
    return page + byte_offset;
}

// Operation to insert a row and store it in memory.
ExecuteResult insert_row(Table* table, Row* row) {
    // Inserting a row will always add a row to the table row count.

    // Before inserting, ensure the table is not past its maximum row count.
    if (table->num_rows >= TABLE_MAX_ROWS) {
        return EXECUTE_TABLE_FULL;
    }

    // Insert at current row count & increment
    void* memory_location = get_memory_location(table, table->num_rows);

    // Store the row
    serialize_row(row, memory_location);

    // Increment the row number
    table->num_rows += 1;

    return EXECUTE_SUCCESS;
}

void select_row(Table* table, Row* row, uint32_t row_number) {
    // Access the offset of the row number based on the size per row.
    void* destination = get_memory_location(table, row_number);

    // Deserialize the data into the row
    deserialize_row(destination, row);
}

ExecuteResult select_all(Table* table) {
    Row row;
    for(uint32_t i = 0; i < table->num_rows; i++ ){
        select_row(table, &row, i);
        printf("(%d, %s, %s)\n", row.id, row.username, row.email);
    }
    return EXECUTE_SUCCESS;
}

Table* create_table(){
    // Allocate memory for the table.
    Table* table = malloc(sizeof(Table));
    table->num_rows = 0;
    // Set each page of the table null.
    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        table->pages[i] = NULL;
    }
    return table;
}

void free_table(Table* table) {
    // Free each row of the table from memory.
    for (int i = 0; table->pages[i]; i++) {
        free(table->pages[i]);
    }
    // Free the table
    free(table);
}