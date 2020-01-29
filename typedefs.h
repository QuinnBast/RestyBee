//
// Created by Quinn on 2020-01-29.
//

#pragma once

#ifndef DBREST_TYPEDEFS_H
#define DBREST_TYPEDEFS_H

#include <endian.h>
#include <stdint-gcc.h>

typedef struct {
    char* stream; // Char pointer that contains the user's input stream
    size_t buffer_length;
    size_t input_length;
} InputBuffer;

// Set some temporary constants
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
// Sample database table definition
typedef struct {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
} Row;

typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_FAILURE,
    EXECUTE_TABLE_FULL
} ExecuteResult;

// database statement status
typedef enum { PREPARE_SUCCESS, PREPARE_UNRECOGNIZED_STATEMENT } PrepareResult;
// database statement types
typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

// Database statement
typedef struct {
    StatementType type;
    Row insert_row;
    struct Table* table;
} Statement;

#define TABLE_MAX_PAGES 100
typedef struct {
    uint32_t num_rows;
    void* pages[TABLE_MAX_PAGES];  // The memory is not allocated because it is a void pointer. We only store as needed.
} Table;

#endif //DBREST_TYPEDEFS_H
