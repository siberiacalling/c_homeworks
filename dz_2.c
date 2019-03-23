#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <zconf.h>
#include <string.h>

/*
 * Смирнова Анита АПО-12
 * RUN ID
 * Разработайте программу-калькулятор, умеющую вычислять арифметические выражения над множествами натуральных чисел.
 * Входные данные (выражение с описанием множеств и производимых над ними операций) подаются на стандартный поток ввода программы,
 * результат вычислений должен подаваться на стандартный поток вывода.
 * Поддерживаемые операции: 'U' - объединение, '^' - пересечение, '\' - разность множеств, '()' - задание приоритета вычислений.
 */
struct Stack {
  int top;
  unsigned capacity;
  int *array;
};

struct Stack *createStack(unsigned capacity) {
  struct Stack *stack = (struct Stack *) malloc(sizeof(struct Stack));
  stack->capacity = capacity;
  stack->top = -1;
  stack->array = (int *) malloc(stack->capacity * sizeof(int));
  return stack;
}

int isFull(struct Stack *stack) { return stack->top == stack->capacity - 1; }

int isEmpty(struct Stack *stack) { return stack->top == -1; }

void push(struct Stack *stack, int item) {
  if (isFull(stack))
    return;
  stack->array[++stack->top] = item;
  printf("%d pushed to stack\n", item);
}

int pop(struct Stack *stack) {
  if (isEmpty(stack))
    return INT_MIN;
  return stack->array[stack->top--];
}

char **allocateTokens(int line_length) {
  char **tokens = (char **) malloc(line_length * sizeof(*tokens));
  if (tokens == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  return tokens;
//  for (int i = 0; i < line_length; i++) {
//    tokens[i] = malloc(line_length * sizeof(char));
//    if (tokens[i] == NULL) {
//      perror("malloc");
//      exit(EXIT_FAILURE);
//    }
//  }
}

void free_memory(char **strings, int strings_capacity) {
  for (int i = 0; i < strings_capacity; i++)
    free(strings[i]);
  free(strings);
}

void createToken(char ** tokens, int token_number, int token_first_pos, int token_last_pos, char *line) {
  int token_length = token_last_pos - token_first_pos;
  tokens[token_number] = malloc(token_length * sizeof(char));
  if (tokens[token_number] == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  strncpy(tokens[token_number], line + token_first_pos, token_length);
}
void findTokens(char *line, int line_length) {
  int digit_token_flag = -1;
  char **tokens = allocateTokens(line_length);

  int tokens_amount = 0;
  for (int i = 0; i < line_length; i++) {
    if (!isspace(line[i])) {
      if (line[i] == '[') {
        digit_token_flag = 1;
      } else if (line[i] == ']') {
        //printf("\n");
        digit_token_flag = 0;
      } else {
        if (digit_token_flag == -1) {
          digit_token_flag = 0;
        }
        if (digit_token_flag == 0) {
          createToken(tokens, tokens_amount, i, i+1, line);
          tokens_amount++;
          //printf("%c\n", line[i]);
        } else {
          //printf("%c", line[i]);
        }
      }
    }
  }
  for (int i = 0; i < tokens_amount; i++) {
    printf("%s\n", tokens[i]);
  }
  printf("\n");
  free_memory(tokens, tokens_amount);
}

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
   // printf("Retrieved line of length %zu:\n", line_length);
    fwrite(line, line_length, 1, stdout);
    findTokens(line, line_length);
  }

  free(line);
  fclose(stream);
  exit(EXIT_SUCCESS);
  return 0;
}
