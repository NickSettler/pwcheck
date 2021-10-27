#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <math.h>

typedef unsigned short bool;
#define true 1
#define false 0

#define MIN_SYSTEM_SYMBOL_CODE 0
#define MAX_SYSTEM_SYMBOL_CODE 31
#define MIN_LOWER_LETTER_CODE 97
#define MAX_LOWER_LETTER_CODE 122
#define MIN_UPPER_LETTER_CODE 65
#define MAX_UPPER_LETTER_CODE 90
#define MIN_NUMBER_CODE 48
#define MAX_NUMBER_CODE 57
#define MIN_SPEC_SYMBOL_CODE 32
#define MAX_SPEC_SYMBOL_CODE 126

typedef bool(*func_ptr)(char);

typedef struct {
    int n_chars;
    int min;
    int avg;
} stats;

int char_code(char c) {
    return (int) c;
}

bool is_system_symbol(char c) {
    return char_code(c) > MIN_SYSTEM_SYMBOL_CODE && char_code(c) < MAX_SYSTEM_SYMBOL_CODE;
}

bool is_lower_letter(char c) {
    return char_code(c) > MIN_LOWER_LETTER_CODE && char_code(c) < MAX_LOWER_LETTER_CODE;
}

bool is_upper_letter(char c) {
    return char_code(c) > MIN_UPPER_LETTER_CODE && char_code(c) < MAX_UPPER_LETTER_CODE;
}

bool is_number(char c) {
    return char_code(c) > MIN_NUMBER_CODE && char_code(c) < MAX_NUMBER_CODE;
}

bool is_specific_symbol(char c) {
    return char_code(c) > MIN_SPEC_SYMBOL_CODE && char_code(c) < MAX_SPEC_SYMBOL_CODE;
}

size_t str_length(char *str) {
    const char *s;
    for (s = str; *s; ++s);
    return (s - str);
}

size_t str_length_only_chars(char *str) {
    int size = 0;
    for (int i = 0; str[i]; i++)
        if (!is_system_symbol(str[i]))
            size++;

    return size;
}

char *substring(const char *str, char *substring, int start, int length) {
    int c = 0;
    while (c < length) {
        substring[c] = str[start + c];
        c++;
    }
    substring[c] = '\0';
}

char *str_copy(char *dest, const char *src) {
    char *ptr = dest;

    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }

    *dest = '\0';

    return ptr;
}

bool is_equal(const char *a, const char *b) {
    bool is_equal = true;
    int i = 0;

    printf("[IS_EQUAL]: %s - %s\n", a, b);

    while (a[i] != '\0' || b[i] != '\0') {
        printf("[IS_EQUAL]: %c = %c\n", a[i], b[i]);
        if (a[i] != b[i]) {
            printf("[IS_EQUAL]: Not equal\n");
            is_equal = false;
            break;
        }

        i++;
    }

    return is_equal;
}

bool str_no_diff(char *str) {
    for (int i = 0; i < str_length(str); i++)
        if (str[i] != str[0])
            return false;

    return true;
}

bool has_system_symbols(char *str) {
    bool has_symbols = false;

    for (char *c = str; *c; ++c) {
        if (is_system_symbol(*c)) {
            has_symbols = true;
            break;
        }

    }

    return has_symbols;
}

//int unique_chars(const char *str) {
//    int table[128] = {0};
//    int c = 0;
//
//    for (int i = 0; str[i]; i++) table[str[i]] = 1;
//
//    for (int i = 0; i < 128; i++)
//        if (!(i > MIN_SYSTEM_SYMBOL_CODE && i < MAX_SYSTEM_SYMBOL_CODE))
//            c += table[i];
//
//    return c;
//}

bool first_rule_check(char *str) {
    bool has_lower_letter = false;
    bool has_upper_letter = false;

    for (char *c = str; *c; ++c) {
        if (is_lower_letter(*c)) has_lower_letter = true;
        if (is_upper_letter(*c)) has_upper_letter = true;
    }

    return has_lower_letter && has_upper_letter;
}

bool second_rule_check(char *str, int level) {
    bool conditions[level];
    func_ptr rule_functions[4] = {is_lower_letter, is_upper_letter, is_number, is_specific_symbol};

    for (int i = 0; i < level; i++) {
        conditions[i] = false;
    }

    for (int i = 0; i < level; i++) {
        func_ptr current_function = rule_functions[i];

        bool is_rule_success = false;

        for (char *c = str; *c; ++c) {
            if (current_function(*c) == true) is_rule_success = true;
        }

        conditions[i] = is_rule_success;
    }

    bool has_wrong_rules = false;

    for (int i = 0; i < level; i++) {
        if (conditions[i] == false) has_wrong_rules = true;
    }

    return !has_wrong_rules;
}

bool third_rule_check(char *str, int level) {
    bool has_repeats = false;

    for (int i = 0; i < str_length(str); i++) {
        char c = str[i];
        if (char_code(c) > MIN_SYSTEM_SYMBOL_CODE && char_code(c) < MAX_SYSTEM_SYMBOL_CODE) continue;

        char kernel[level];
        substring(str, kernel, i, level);

        if (has_system_symbols(kernel)) continue;
        if (i + level > str_length_only_chars(str)) continue;

        if (str_no_diff(kernel)) {
            has_repeats = true;
            break;
        }
    }

    return !has_repeats;
}

bool fourth_rule_check(char *str, int level) {
//    int repeats_count = 0;
//    char *first_repeat_kernel = "";
//
//    for (int i = 0; i < str_length(str); i++) {
//        char c = str[i];
//        if (char_code(c) > MIN_SYSTEM_SYMBOL_CODE && char_code(c) < MAX_SYSTEM_SYMBOL_CODE) continue;
//
//        char kernel[level];
//        substring(str, kernel, i, level);
//
//        if (has_system_symbols(kernel)) continue;
//        if (i + level > str_length_only_chars(str)) continue;
//
//        if (str_no_diff(kernel)) {
//            repeats_count += 1;
//            if (is_equal(first_repeat_kernel, "")) {
//                printf("firstly setting kernel\n");
//                char temp_repeat_kernel[level];
//
//                first_repeat_kernel = temp_repeat_kernel;
//            }
//
//            printf("FPK: %s\n", first_repeat_kernel);
//        }
//    }
}

int unique_chars_from_table(const int *table) {
    int c = 0;
    for (int i = 0; i < 128; i++)
        if (!(i > MIN_SYSTEM_SYMBOL_CODE && i < MAX_SYSTEM_SYMBOL_CODE))
            c += table[i];

    return c;
}

void update_chars_table(int *table, const char *str) {
    for (int i = 0; str[i]; i++) table[str[i]] = 1;
}

stats calc_stats(char *passwords) {
    stats resulting_stats;
    resulting_stats.n_chars = 0;
    resulting_stats.min = 0;
    resulting_stats.avg = 0;

    return resulting_stats;
}

int main(int argc, char *argv[]) {
    char input_passwords[100];

    int chars_table[128] = {0};
    int min = INFINITY;
    double passwords_count = 0;
    double passwords_chars_count = 0;

    while (fgets(input_passwords, 100, stdin)) {
        third_rule_check(input_passwords, 2);
        printf("\n");

//        WORKS!
//        if (third_rule_check(input_passwords, 2))
//            printf("%s - %s\n", input_passwords, "third rule success");
//        else
//            printf("%s - %s\n", input_passwords, "third rule error");

//        WORKS!
//        update_chars_table(chars_table, input_passwords);
//
//        printf("\n\n");
//
//        int password_length = (int) str_length_only_chars(input_passwords);
//
//        if (password_length < min) min = password_length;
//
//        passwords_count += 1;
//        passwords_chars_count += (int) str_length_only_chars(input_passwords);
//
//        if (feof(stdin)) {
//            printf("feof\n\n");
//            double avg = passwords_chars_count / passwords_count;
//
//            printf("NCHARS: %d\n", unique_chars_from_table(chars_table));
//            printf("MIN: %d\n", min);
//            printf("AVG: %.1f", avg);
//        }


//        WORKS!
//        if (second_rule_check(input_passwords, 1))
//            printf("%s - %s\n", input_passwords, "second rule success");
//        else
//            printf("%s - %s\n", input_passwords, "second rule error");

//        WORKS!
//        if (first_rule_check(input_passwords))
//            printf("%s - %s\n", input_passwords, "first rule success");
//        else
//            printf("%s - %s\n", input_passwords, "first rule error");
    }

//    int opt;

//    while ((opt = getopt(argc, argv, "lf")) != -1) {
//        switch (opt) {
//            case 'f':
//                printf("Option f was specified\n");
//                break;
//            case 'l':
//                printf("Option l was specified\n");
//                break;
//        }
//    }

    return 0;
}
