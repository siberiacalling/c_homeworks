#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*
 * Смирнова Анита АПО-12
 * RUN ID
 * Разработайте программу-калькулятор, умеющую вычислять арифметические выражения над множествами натуральных чисел.
 * Входные данные (выражение с описанием множеств и производимых над ними операций) подаются на стандартный поток ввода программы,
 * результат вычислений должен подаваться на стандартный поток вывода.
 * Поддерживаемые операции: 'U' - объединение, '^' - пересечение, '\' - разность множеств, '()' - задание приоритета вычислений.
 */


int main() {
  FILE *stream = fopen("/home/anita/Desktop/c_homeworks/test.txt", "r");
  if (stream == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  char *line = NULL;
  size_t len = 0;
  ssize_t line_length;

  while ((line_length = getline(&line, &len, stream)) != -1) {
    printf("Retrieved line of length %zu:\n", line_length);
    fwrite(line, line_length, 1, stdout);
    int digit_token_flag = -1;
    for (int i = 0; i < line_length; i++) {
      if (!isspace(line[i])) {
        if (line[i] == '[') {
          digit_token_flag = 1;
        } else if (line[i] == ']') {

          printf("\n");
          digit_token_flag = 0;
        } else {
          if (digit_token_flag == -1) {
            digit_token_flag = 0;
          }
            if (digit_token_flag == 0) {
            printf("%c\n", line[i]);
          } else {
            printf("%c", line[i]);
          }
        }
      }
    }
  }

  free(line);
  fclose(stream);
  exit(EXIT_SUCCESS);
  return 0;
}
