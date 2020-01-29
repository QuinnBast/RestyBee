#pragma once

#include <stdint-gcc.h>

// The size of an attribute is the size of the struct's reference to that attribute.
// Makes sense.
#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

const uint32_t ID_SIZE;
const uint32_t USERNAME_SIZE;
const uint32_t EMAIL_SIZE;
const uint32_t ID_OFFSET;
#define USERNAME_OFFSET (ID_OFFSET + ID_SIZE)
#define EMAIL_OFFSET (USERNAME_OFFSET + USERNAME_SIZE)
#define ROW_SIZE (ID_SIZE + USERNAME_SIZE + EMAIL_SIZE)

// Struct to define a table and how many 'pages' of the table we can hold in memory.
const uint32_t PAGE_SIZE;
#define ROWS_PER_PAGE (PAGE_SIZE / ROW_SIZE)
#define TABLE_MAX_ROWS (ROWS_PER_PAGE * TABLE_MAX_PAGES)

// Code to store the table data in memory as compact as we can.
void serialize_row(Row* source, void* destination);
void deserialize_row(void* source, Row* destination);
void* get_memory_location(Table*, uint32_t);
ExecuteResult insert_row(Table*, Row*);
void select_row(Table*, Row*, uint32_t);
ExecuteResult select_all(Table*);
Table* create_table();
void free_table(Table*);