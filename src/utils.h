#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Constants
#define MAX_STRING 256
#define MAX_EMAIL 100
#define MAX_PHONE 20
#define MAX_DATE 11
#define MAX_RECORDS 1000
#define DATA_DIR "data/"

// Date and Time utilities
char* get_current_date();
int is_valid_date(const char* date);
int date_compare(const char* date1, const char* date2);

// File utilities
int file_exists(const char* filename);
int read_line(FILE* file, char* buffer, int size);
void create_data_directory();

// Validation utilities
int is_valid_email(const char* email);
int is_valid_phone(const char* phone);
int is_digit_only(const char* str);
int is_valid_date_format(const char* date);

// String utilities
void trim_whitespace(char* str);
void to_lowercase(char* str);
int string_contains(const char* str, const char* substr);

// ID utilities
int generate_next_id(const char* filename);
int id_exists(const char* filename, int id, int id_column);

// Error handling
void print_error(const char* message);
void print_success(const char* message);
void print_info(const char* message);

#endif // UTILS_H
