#include <stdio.h>
#include <limits.h>
#include <getopt.h>
#include <stdlib.h>

typedef unsigned short bool;
#define true 1
#define false 0

#define SECURITY_KEY "-l"
#define EXTRA_PARAM_KEY "-p"
#define STATS_KEY "--stats"

/**
 * ASCII codes of symbols' categories:
 * non-printing symbols, lower letters, upper letters, numbers, special symbols
 */
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

/**
 * Type of a function for checking character category
 */
typedef bool(*func_ptr)(char);

/**
 * Type of a function for checking four rules
 */
typedef bool(*rule_func_ptr)(char *, int);

/**
 * Statistics object type
 */
typedef struct {
    int n_chars;
    int min;
    double avg;
} stats;

/**
 * Get character ASCII code
 * @param c character
 * @return ASCII code
 */
int char_code(char c) {
    return (int) c;
}

/**
 * Checks if character is non-printing symbol
 * @param c character to check
 * @return if character is non-printing symbol
 */
bool is_system_symbol(char c) {
    return char_code(c) >= MIN_SYSTEM_SYMBOL_CODE &&
           char_code(c) <= MAX_SYSTEM_SYMBOL_CODE;
}

/**
 * Checks if character is lower letter
 * @param c character to check
 * @return if character is lower letter
 */
bool is_lower_letter(char c) {
    return char_code(c) >= MIN_LOWER_LETTER_CODE &&
           char_code(c) <= MAX_LOWER_LETTER_CODE;
}

/**
 * Checks if character is upper letter
 * @param c character to check
 * @return if character is upper letter
 */
bool is_upper_letter(char c) {
    return char_code(c) >= MIN_UPPER_LETTER_CODE &&
           char_code(c) <= MAX_UPPER_LETTER_CODE;
}

/**
 * Checks if character is number
 * @param c character to check
 * @return if character is number
 */
bool is_number(char c) {
    return char_code(c) >= MIN_NUMBER_CODE &&
           char_code(c) <= MAX_NUMBER_CODE;
}

/**
 * Checks if character is special symbol
 * @param c character to check
 * @return if character is special symbol
 */
bool is_specific_symbol(char c) {
    return char_code(c) >= MIN_SPEC_SYMBOL_CODE &&
           char_code(c) <= MAX_SPEC_SYMBOL_CODE &&
           !is_lower_letter(c) &&
           !is_upper_letter(c) &&
           !is_number(c);
}

/**
 * Returns length of a string including non-printing characters
 * @param str string
 * @return length of a string `str`
 */
size_t str_length(char *str) {
    const char *s;
    for (s = str; *s; ++s);
    return (s - str);
}

/**
 * Returns length of a string excluding non-printing characters
 * @param str string
 * @return length of a string `str` without non-printing characters
 */
size_t str_length_only_chars(char *str) {
    int size = 0;

    for (int i = 0; str[i]; i++)
        if (!is_system_symbol(str[i]))
            size++;

    return size;
}

/**
 * Returns a substring of a string
 * @param str initial string
 * @param substring pointer to the string variable where substring will be placed
 * @param start starting position of a substring
 * @param length length of a substring
 */
void substring(const char *str, char *substring, int start, int length) {
    int c = 0;

    while (c < length) {
        substring[c] = str[start + c];
        c++;
    }
    substring[c] = '\0';
}

/**
 * Counts the numbers of substrings with size `kernel_size` in the string `str`
 * Example:
 *  str: 'ABCDEF'
 *  kernel_size: 3
 *
 *  The `str_substrings_count` will return 4, because there are such possible
 *  substrings in the `str`: {"ABC", "BCD", "CDE", "DEF"}
 * @param str string to find number of substrings
 * @param kernel_size size of a substring
 * @return number of possible substrings
 */
int str_substrings_count(char *str, int kernel_size) {
    return (int) str_length_only_chars(str) - kernel_size + 1;
}

/**
 * Checks if all characters in the string are same
 * @param str string to check
 * @return if all characters in the string are same
 */
bool str_no_diff(char *str) {
    for (int i = 0; i < str_length(str); i++)
        if (str[i] != str[0])
            return false;

    return true;
}

/**
 * Compares two string
 * @param a first string
 * @param b second string
 * @return if strings are equal
 */
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

/**
 * Returns array items count
 * @param array array to count
 * @return length of the array
 */
int array_length(char *array[]) {
    int count = 0;

    while (*(array + count) != NULL)
        count++;

    return count;
}

/**
 * Finds out if the array contains a string
 * @param array array to look for the string
 * @param str string searching for
 * @return if the string is in the array
 */
bool array_contains_string(char *array[], char *str) {
    bool contains = false;

    if (!array[0]) return contains;

    int size = array_length(array);

    for (int i = 0; i < size; i++) {
        if (is_equal(array[i], str)) {
            contains = true;
            break;
        }
    }

    return contains;
}

/**
 * Checks if the first rule succeeds for the string.
 * First Rule: The string has at least one lower and one upper letter
 * @param str string to check
 * @param level level of checking for the rule
 * @return if the string succeeds the first rule
 */
bool first_rule_check(char *str, int level) {
    bool has_lower_letter = false;
    bool has_upper_letter = false;

    for (char *c = str; *c; ++c) {
        if (is_lower_letter(*c)) has_lower_letter = true;
        if (is_upper_letter(*c)) has_upper_letter = true;
    }

    return has_lower_letter && has_upper_letter;
}

/**
 * Checks if the second rule succeeds for the string.
 * Second Rule: The string completes condition depending on the level parameter.
 *  1. The string includes lower letters
 *  2. The string includes upper letters
 *  3. The string includes numbers
 *  4. The string includes special characters
 * @param str string to check
 * @param level level of checking for the rule
 * @return if the string succeeds the second rule
 */
bool second_rule_check(char *str, int level) {
    bool first_rule_succeeded = first_rule_check(str, level);

    if (!first_rule_succeeded) return false;

    if (level > 4) level = 4;

    bool conditions[level];
    func_ptr rule_functions[4] = {
            is_lower_letter,
            is_upper_letter,
            is_number,
            is_specific_symbol
    };

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

/**
 * Checks if the third rule succeeds for the string.
 * Third Rule: The string does not have a substring of the same characters with
 * at least X length
 * @param str string to check
 * @param level level of checking for the rule
 * @return if the string succeeds the third rule
 */
bool third_rule_check(char *str, int level) {
    bool first_rule_succeeded = first_rule_check(str, level);
    bool second_rule_succeeded = second_rule_check(str, level);

    if (!first_rule_succeeded || !second_rule_succeeded) return false;

    bool has_repeating_symbols = false;

    for (int i = level; i <= str_length_only_chars(str); i++) {
        int substrings_count = str_substrings_count(str, i);

        for (int j = 0; j < substrings_count; j++) {
            char substr[i];
            substring(str, substr, j, i);

            if (str_no_diff(substr)) {
                has_repeating_symbols = true;
                break;
            }
        }

        if (has_repeating_symbols) break;
    }

    return !has_repeating_symbols;
}

/**
 * Checks if the fourth rule succeeds for the string.
 * Fourth Rule: The string does not have two same substrings with at least X length
 * @param str string to check
 * @param level level of checking for the rule
 * @return if the string succeeds the fourth rule
 */
bool fourth_rule_check(char *str, int level) {
    bool first_rule_succeeded = first_rule_check(str, level);
    bool second_rule_succeeded = second_rule_check(str, level);
    bool third_rule_succeeded = third_rule_check(str, level);

    if (!first_rule_succeeded ||
        !second_rule_succeeded ||
        !third_rule_succeeded)
        return false;

    bool has_multiple_repeats = false;

    for (int i = level; i < str_length_only_chars(str); i++) {
        int substrings_count = str_substrings_count(str, i);

        for (int j = 0; j < substrings_count; j++) {
            char substring_a[i];
            substring(str, substring_a, j, i);
            for (int k = 0; k < substrings_count; k++) {
                char substring_b[i];
                substring(str, substring_b, k, i);
                if (j < k) {
                    if (is_equal(substring_a, substring_b)) {
                        has_multiple_repeats = true;
                        break;
                    }
                }
            }

            if (has_multiple_repeats) break;
        }

        if (has_multiple_repeats) break;
    }

    return !has_multiple_repeats;
}

/**
 * Returns a program argument
 * @param argv array of program arguments
 * @param argument argument name
 * @param default_position default argument position (if argument name not exists)
 * @param default_value default value
 * @return option value as a number
 */
int get_program_argument(
        char *argv[],
        char *argument,
        int default_position,
        int default_value) {
    int arg = 1;

    int size = array_length(argv);

    if (array_contains_string(argv, argument))
        for (int i = 0; i < size; i++) {
            if (is_equal(argv[i], argument)) {
                arg = argv[i + 1] ? atoi(argv[i + 1]) : default_value;
                break;
            }
        }
    else
        arg = argv[default_position] ?
              atoi(argv[default_position]) :
              default_value;

    return arg;
}

/**
 * Returns a security param
 * @param argv array of program arguments
 * @return security level integer
 */
int get_security_level(char *argv[]) {
    int default_position = array_contains_string(argv, SECURITY_KEY) ? 2 : 1;

    return get_program_argument(argv, SECURITY_KEY, default_position, 1);
}

/**
 * Returns an extra param
 * @param argv array of program arguments
 * @return extra param integer
 */
int get_extra_param(char *argv[]) {
    bool has_security_option_key = array_contains_string(argv, SECURITY_KEY);
    bool has_extra_param_key = array_contains_string(argv, EXTRA_PARAM_KEY);

    int default_position = array_contains_string(argv, EXTRA_PARAM_KEY) ? 3 : 2;
    if (has_security_option_key)
        default_position += 1;
    else if (has_extra_param_key)
        default_position -= 1;

    return get_program_argument(argv, EXTRA_PARAM_KEY, default_position, 1);
}

/**
 * Returns true if statistics needed
 * @param argv array of program arguments
 * @return if statistics needed
 */
bool is_stats_needed(char *argv[]) {
    return array_contains_string(argv, STATS_KEY);
}

/**
 * Counts number of symbols valued as 1 from ASCII table
 * @param table ASCII characters table
 * @return number of unique symbols
 */
int unique_chars_from_table(const int *table) {
    int c = 0;
    for (int i = 0; i < 128; i++)
        if (!(i > MIN_SYSTEM_SYMBOL_CODE && i < MAX_SYSTEM_SYMBOL_CODE))
            c += table[i];

    return c;
}

/**
 * Updates characters in ASCII table
 * @param table ASCII characters table
 * @param str string with characters
 */
void update_chars_table(int *table, const char *str) {
    for (int i = 0; str[i]; i++) table[str[i]] = 1;
}

/**
 * Print statistics to stdout
 * @param statistics statistics object to print
 */
void print_stats(stats statistics) {
    printf(
            "\n"
            "Statistika:\n"
            "Ruznych znaku: %d\n"
            "Minimalni delka: %d\n"
            "Prumerna delka: %.1f",
            statistics.n_chars,
            statistics.min,
            statistics.avg
    );
}

/**
 * Main program function
 * @param argc number of the arguments
 * @param argv arguments' array
 * @return exit code
 */
int main(int argc, char *argv[]) {
    int level = get_security_level(argv);
    int param = get_extra_param(argv);
    bool need_stats = is_stats_needed(argv);

    char input_passwords[100];

    int chars_table[128] = {0};
    int min = INT_MAX;
    double passwords_count = 0;
    double passwords_chars_count = 0;

    rule_func_ptr check_functions[4] = {
            &first_rule_check,
            &second_rule_check,
            &third_rule_check,
            &fourth_rule_check
    };

    printf("SEC_LVL: %d, PARAM: %d, STATS: %d\n", level, param, need_stats);

    while (fgets(input_passwords, 100, stdin)) {
        if (check_functions[level - 1](input_passwords, param))
            printf("%s", input_passwords);


        update_chars_table(chars_table, input_passwords);

        int password_length = (int) str_length_only_chars(input_passwords);
        if (password_length < min) min = password_length;

        passwords_count += 1;
        passwords_chars_count += (int) str_length_only_chars(input_passwords);

        if (feof(stdin) && need_stats) {
            double avg = passwords_chars_count / passwords_count;

            stats statistics;
            statistics.n_chars = unique_chars_from_table(chars_table);
            statistics.min = min;
            statistics.avg = avg;
            print_stats(statistics);
        }
    }

    return 0;
}
