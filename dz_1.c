#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define MAX_LEN 1024
#define STRINGS_AMOUNT_BEFORE_RELOCATION 128

void delete_spaces(char **strings, int space_amount, int string_number, int j) {
    int string_length = (int) strlen(strings[string_number]);
    for (int k = j + 2; k < string_length; k++) {
        strings[string_number][k] = strings[string_number][k + space_amount - 1];
        if (k == string_length - 1) {
            break;
        }
    }
    strings[string_number][string_length - space_amount + 1] = '\0';
}

void process_strings(char **strings, int strings_amount) {
    for (int i = 0; i < strings_amount; i++) {
        int space_amount = 0;
        bool is_space = false;
        int string_length = (int) strlen(strings[i]);
        for (int j = string_length - 2; j >= 0; j--) {
            if (strings[i][j] == ' ') {
                space_amount++;
                is_space = true;
            } else {
                if (is_space) {
                    if (space_amount > 1) {
                        delete_spaces(strings, space_amount, i, j);
                    }
                    is_space = false;
                    space_amount = 0;
                }
            }
        }
        if (space_amount > 1) {
            delete_spaces(strings, space_amount, i, -1);
        }
    }
    for (int i = 0; i < strings_amount; i++) {
        printf("%s", strings[i]);
    }
}

void free_memory(char **strings, int strings_amount) {
    if (strings_amount == 0) {
        for (int i = 0; i < STRINGS_AMOUNT_BEFORE_RELOCATION; i++)
            free(strings[i]);
        free(strings);
    } else {
        for (int i = 0; i <= strings_amount; i++)
            free(strings[i]);
        free(strings);
    }
}

int main() {
    char **strings = (char **) malloc(STRINGS_AMOUNT_BEFORE_RELOCATION * sizeof(*strings));
    if (strings == NULL) {
        printf("Malloc error: %s\n", strerror(errno));
        return 0;
    }
    for (int i = 0; i < STRINGS_AMOUNT_BEFORE_RELOCATION; i++) {
        strings[i] = malloc(MAX_LEN * sizeof(char));
        if (strings[i] == NULL) {
            printf("Malloc error: %s\n", strerror(errno));
            return 0;
        }
    }

    int strings_amount = 0;
    while (1) {
        if (strings_amount < STRINGS_AMOUNT_BEFORE_RELOCATION) {
//            char *fgets_result = fgets(strings[strings_amount], MAX_LEN, stdin);
//            if (fgets_result == NULL) {
//                break;
//            }
        } else {
            strings = realloc(strings, (strings_amount + 1) * sizeof(*strings));
            strings[strings_amount] = malloc(MAX_LEN * sizeof(char *));
            if (strings[strings_amount] == NULL) {
                printf("Malloc error: %s\n", strerror(errno));
                return 0;
            }
        }
            char *fgets_result = fgets(strings[strings_amount], MAX_LEN, stdin);
            if (fgets_result == NULL) {
                break;
            }
        //}
        strings_amount++;
    }
    process_strings(strings, strings_amount);
    free_memory(strings, strings_amount);

    return 0;
}