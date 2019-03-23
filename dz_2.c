#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Смирнова Анита АПО-12
 * RUN ID
 * Разработайте программу-калькулятор, умеющую вычислять арифметические выражения над множествами натуральных чисел.
 * Входные данные (выражение с описанием множеств и производимых над ними операций) подаются на стандартный поток ввода программы,
 * результат вычислений должен подаваться на стандартный поток вывода.
 * Поддерживаемые операции: 'U' - объединение, '^' - пересечение, '\' - разность множеств, '()' - задание приоритета вычислений.
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

void delete_spaces(char *string) {
    int space_amount = 0;
    int string_length = (int) strlen(string);
    for (int j = string_length - 2; j >= 0; j--) {
      if (string[j] == ' ') {
        space_amount++;
      } else {
        if (space_amount > 1) {
          delete_spaces_from_string(string, space_amount, j);
        }
        space_amount = 0;
      }
    }
    if (space_amount > 1) {
      delete_spaces_from_string(string, space_amount, -1);
    }
}

int main() {
  FILE * stream = fopen("/home/anita/Desktop/c_homeworks/test.txt", "r");
  if (stream == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  while ((nread = getline(&line, &len, stream)) != -1) {
    printf("Retrieved line of length %zu:\n", nread);
    fwrite(line, nread, 1, stdout);
  }

  free(line);
  fclose(stream);
  exit(EXIT_SUCCESS);
  return 0;
}
