#define MAX_LEN 1024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * Смирнова Анита АПО-12
 * RUN ID 1357
 * Программа построчной обработки текста, в результате которой
 * каждая группа повторяющихся пробелов заменяется на один пробел.
 * Текстовые строки подаются на стандартный ввод программы,
 * результат программы должен подаваться на стандартный вывод.
 */

void delete_spaces_from_string(char *string, int space_amount, int j) {
    int string_length = (int) strlen(string);
    for (int k = j + 2; k < string_length; k++) {
        string[k] = string[k + space_amount - 1];
        if (k == string_length - 1) {
            break;
        }
    }
    string[string_length - space_amount + 1] = '\0';
}

void delete_spaces(char **strings, int strings_amount) {
    for (int i = 0; i < strings_amount; i++) {
        int space_amount = 0;
        int string_length = (int) strlen(strings[i]);
        for (int j = string_length - 2; j >= 0; j--) {
            if (strings[i][j] == ' ') {
                space_amount++;
            } else {
                if (space_amount > 1) {
                    delete_spaces_from_string(strings[i], space_amount, j);
                }
                space_amount = 0;
            }
        }
        if (space_amount > 1) {
            delete_spaces_from_string(strings[i], space_amount, -1);
        }
    }
}

void free_memory(char **strings, int strings_capacity) {
    for (int i = 0; i < strings_capacity; i++)
        free(strings[i]);
    free(strings);
}

char **realloc_memory(char **strings, int strings_capacity) {
    char **new_strings = realloc(strings, (strings_capacity) * sizeof(*strings));
    if (new_strings == NULL) {
        free_memory(strings, strings_capacity);
        printf("[error]");
        return NULL;
    }
    for (int i = strings_capacity / 2; i < strings_capacity; i++) {
        new_strings[i] = malloc(MAX_LEN * sizeof(char *));
        if (new_strings[i] == NULL) {
            printf("[error]");
            return NULL;
        }
    }
    return new_strings;
}

int main() {
    int strings_capacity = 2;

    char **strings = (char **) malloc(strings_capacity * sizeof(*strings));
    if (strings == NULL) {
        printf("[error]");
        return 0;
    }
    for (int i = 0; i < strings_capacity; i++) {
        strings[i] = malloc(MAX_LEN * sizeof(char));
        if (strings[i] == NULL) {
            printf("[error]");
            return 0;
        }
    }

    int strings_amount = 0;
    while (1) {
        if (strings_amount >= strings_capacity) {
            strings_capacity *= 2;
            strings = realloc_memory(strings, strings_capacity);
            if (strings == NULL) {
                printf("[error]");
                return 0;
            }
        }
        char *fgets_result = fgets(strings[strings_amount], MAX_LEN, stdin);
        if (fgets_result == NULL) {
            break;
        }
        strings_amount++;
    }

    delete_spaces(strings, strings_amount);
    for (int i = 0; i < strings_amount; i++) {
        printf("%s", strings[i]);
    }
    free_memory(strings, strings_capacity);
    return 0;
}