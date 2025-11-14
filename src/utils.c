#include "utils.h"

// ============= Date and Time Functions =============

char* get_current_date() {
    static char date[MAX_DATE];
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    strftime(date, MAX_DATE, "%Y-%m-%d", tm_info);
    return date;
}

int is_valid_date_format(const char* date) {
    if (strlen(date) != 10) return 0;
    if (date[4] != '-' || date[7] != '-') return 0;
    
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return 0;
    }
    return 1;
}

int is_valid_date(const char* date) {
    if (!is_valid_date_format(date)) return 0;
    
    int year, month, day;
    sscanf(date, "%d-%d-%d", &year, &month, &day);
    
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (year < 1900 || year > 2100) return 0;
    
    // Check for valid day in month
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Check leap year
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
        days_in_month[1] = 29;
    
    if (day > days_in_month[month - 1]) return 0;
    
    return 1;
}

int date_compare(const char* date1, const char* date2) {
    int year1, month1, day1, year2, month2, day2;
    sscanf(date1, "%d-%d-%d", &year1, &month1, &day1);
    sscanf(date2, "%d-%d-%d", &year2, &month2, &day2);
    
    if (year1 != year2) return year1 - year2;
    if (month1 != month2) return month1 - month2;
    return day1 - day2;
}

// ============= File Functions =============

int file_exists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

int read_line(FILE* file, char* buffer, int size) {
    if (fgets(buffer, size, file) == NULL) return 0;
    
    // Remove trailing newline
    int len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
        buffer[len - 1] = '\0';
    
    return 1;
}

void create_data_directory() {
    FILE* test = fopen(DATA_DIR "test.tmp", "w");
    if (test) {
        fclose(test);
        remove(DATA_DIR "test.tmp");
    }
}

// ============= Validation Functions =============

int is_valid_email(const char* email) {
    if (strlen(email) < 5 || strlen(email) >= MAX_EMAIL) return 0;
    
    char* at = strchr(email, '@');
    if (!at) return 0;
    
    // Check for at least one character before @
    if (at == email) return 0;
    
    // Check for at least one character after @ and a dot
    char* dot = strchr(at + 1, '.');
    if (!dot || dot == at + 1) return 0;
    
    // Check for character after dot
    if (strlen(dot) < 2) return 0;
    
    return 1;
}

int is_digit_only(const char* str) {
    if (str == NULL || strlen(str) == 0) return 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

int is_valid_phone(const char* phone) {
    if (strlen(phone) < 7 || strlen(phone) > MAX_PHONE) return 0;
    return is_digit_only(phone);
}

// ============= String Functions =============

void trim_whitespace(char* str) {
    if (str == NULL) return;
    
    // Trim leading whitespace
    int start = 0;
    while (isspace(str[start])) start++;
    
    // Trim trailing whitespace
    int end = strlen(str) - 1;
    while (end >= start && isspace(str[end])) end--;
    
    // Copy trimmed string
    int i;
    for (i = 0; i <= end - start; i++)
        str[i] = str[start + i];
    str[i] = '\0';
}

void to_lowercase(char* str) {
    if (str == NULL) return;
    for (int i = 0; str[i] != '\0'; i++)
        str[i] = tolower(str[i]);
}

int string_contains(const char* str, const char* substr) {
    if (str == NULL || substr == NULL) return 0;
    return strstr(str, substr) != NULL;
}

// ============= ID Functions =============

int generate_next_id(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return 1;  // First ID is 1
    
    int max_id = 0;
    char line[1024];
    
    while (read_line(file, line, sizeof(line))) {
        if (strlen(line) == 0) continue;
        
        int id = atoi(line);
        if (id > max_id) max_id = id;
    }
    
    fclose(file);
    return max_id + 1;
}

int id_exists(const char* filename, int id, int id_column) {
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[1024];
    while (read_line(file, line, sizeof(line))) {
        if (strlen(line) == 0) continue;
        
        char* token = strtok(line, ",");
        for (int i = 0; i < id_column; i++) {
            token = strtok(NULL, ",");
        }
        
        if (token && atoi(token) == id) {
            fclose(file);
            return 1;
        }
    }
    
    fclose(file);
    return 0;
}

// ============= Error Handling =============

void print_error(const char* message) {
    fprintf(stderr, "[ERREUR] %s\n", message);
}

void print_warning(const char* message) {
    fprintf(stdout, "[ATTENTION] %s\n", message);
}

void print_success(const char* message) {
    fprintf(stdout, "[SUCCÈS] %s\n", message);
}

void print_info(const char* message) {
    fprintf(stdout, "[INFO] %s\n", message);
}
