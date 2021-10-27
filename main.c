#include <stdio.h>
#include <limits.h>
#include <getopt.h>
#include <stdlib.h>

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
    double avg;
} stats;

int char_code(char c) {
    return (int) c;
}

bool is_system_symbol(char c) {
    return char_code(c) >= MIN_SYSTEM_SYMBOL_CODE && char_code(c) <= MAX_SYSTEM_SYMBOL_CODE;
}

bool is_lower_letter(char c) {
    return char_code(c) >= MIN_LOWER_LETTER_CODE && char_code(c) <= MAX_LOWER_LETTER_CODE;
}

bool is_upper_letter(char c) {
    return char_code(c) >= MIN_UPPER_LETTER_CODE && char_code(c) <= MAX_UPPER_LETTER_CODE;
}

bool is_number(char c) {
    return char_code(c) >= MIN_NUMBER_CODE && char_code(c) <= MAX_NUMBER_CODE;
}

bool is_specific_symbol(char c) {
    return char_code(c) >= MIN_SPEC_SYMBOL_CODE && char_code(c) <= MAX_SPEC_SYMBOL_CODE && !is_lower_letter(c) &&
           !is_upper_letter(c) && !is_number(c);
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

void *substring(const char *str, char *substring, int start, int length) {
    int c = 0;
    while (c < length) {
        substring[c] = str[start + c];
        c++;
    }
    substring[c] = '\0';
}

int str_substrings_count(char *str, int kernel_size) {
    return (int) str_length_only_chars(str) - kernel_size + 1;
}

bool is_equal(const char *a, const char *b) {
    bool is_equal = true;
    int i = 0;

    while (a[i] != '\0' || b[i] != '\0') {
        if (a[i] != b[i]) {
            is_equal = false;
            break;
        }

        i++;
    }

    return is_equal;
}

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
    if (level > 4) level = 4;
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

    int substrings_count = str_substrings_count(str, level);

    for (int i = 0; i < substrings_count; i++) {
        for (int j = 0; j < substrings_count; j++) {
            if (i < j) {
                char substring_a[level], substring_b[level];
                substring(str, substring_a, i, level);
                substring(str, substring_b, j, level);

                if (is_equal(substring_a, substring_b)) {
                    has_repeats = true;
                    break;
                }
            }
        }

        if (has_repeats) break;
    }

    return !has_repeats;
}

bool fourth_rule_check(char *str, int level) {
    int first_repeat_index_a = -1;
    int first_repeat_index_b = -1;

    bool has_multiple_repeats = false;

    int substrings_count = str_substrings_count(str, level);

    for (int i = 0; i < substrings_count; i++) {
        for (int j = 0; j < substrings_count; j++) {
            if (i < j) {
                char substring_a[level], substring_b[level];
                substring(str, substring_a, i, level);
                substring(str, substring_b, j, level);

                if (is_equal(substring_a, substring_b)) {
                    if (first_repeat_index_a == -1 && first_repeat_index_b == -1) {
                        first_repeat_index_a = i;
                        first_repeat_index_b = j;
                    } else {
                        has_multiple_repeats = true;
                        break;
                    }
                }
            }
        }

        if (has_multiple_repeats) break;
    }

    return !has_multiple_repeats;
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

void print_stats(stats statistics) {
    printf(
            "Statistika:\n"
            "Ruznych znaku: %d\n"
            "Minimalni delka: %d\n"
            "Prumerna delka: %.1f", statistics.n_chars, statistics.min, statistics.avg);
}

int main(int argc, char *argv[]) {
    char input_passwords[100];

    int chars_table[128] = {0};
    int min = INT_MAX;
    double passwords_count = 0;
    double passwords_chars_count = 0;

    while (fgets(input_passwords, 100, stdin)) {
//        WORKS!
//        if (fourth_rule_check(input_passwords, 2))
//            printf("%s - %s\n", input_passwords, "fourth rule success");
//        else
//            printf("%s - %s\n", input_passwords, "fourth rule error");

//        WORKS!
//        if (third_rule_check(input_passwords, 2))
//            printf("%s - %s\n", input_passwords, "third rule success");
//        else
//            printf("%s - %s\n", input_passwords, "third rule error");

        update_chars_table(chars_table, input_passwords);

        int password_length = (int) str_length_only_chars(input_passwords);
        if (password_length < min) min = password_length;

        passwords_count += 1;
        passwords_chars_count += (int) str_length_only_chars(input_passwords);

        if (feof(stdin)) {
            double avg = passwords_chars_count / passwords_count;

            stats statistics;
            statistics.n_chars = unique_chars_from_table(chars_table);
            statistics.min = min;
            statistics.avg = avg;
//            print_stats(statistics);
        }


//        WORKS!

//        WORKS!
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
