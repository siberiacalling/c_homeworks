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
}

void free_memory(char **strings, int strings_capacity) {
  for (int i = 0; i < strings_capacity; i++)
    free(strings[i]);
  free(strings);
}
int compare( const void* a, const void* b)
{
  int int_a = * ( (int*) a );
  int int_b = * ( (int*) b );

  if ( int_a == int_b ) return 0;
  if ( int_a < int_b ) return -1;
  return 1;
}

void convertStrtoArr(const char *str) {
  // 128 max length
  int arr[128] = {0};
  int flag_digit = 0;
  int j = -1;
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] == ',' || isspace(str[i])) {
      flag_digit = 0;
      // j++;
    } else {
      if (flag_digit == 0) {
        flag_digit = 1;
        j++;
      }
      arr[j] = arr[j] * 10 + (str[i] - 48);
    }
  }
  size_t arr_length = (size_t) j + 1;
  if (j != -1) {
    qsort(arr, arr_length, sizeof(int), compare);
  }
  for (int i = 0; i <= j; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");

}

/* Function prints union of arr1[] and arr2[]
   m is the number of elements in arr1[]
   n is the number of elements in arr2[] */

int getUnion(const int *arr1, const int *arr2, int m, int n, int *my_union)
{
  int union_elements  = 0;

  int i = 0, j = 0;
  while (i < m && j < n)
  {
    if (arr1[i] < arr2[j]) {
      my_union[union_elements] = arr1[i++];
      union_elements++;
    }
    else if (arr2[j] < arr1[i]) {
      my_union[union_elements] = arr2[j++];
      union_elements++;
    }
    else
    {
      my_union[union_elements] = arr2[j++];
      union_elements++;
      i++;
    }
  }
  /* Print remaining elements of the larger array */
  while(i < m) {
    my_union[union_elements] = arr1[i++];
    union_elements++;
  }
  while(j < n){
    my_union[union_elements] = arr2[j++];
    union_elements++;
  }
  return union_elements;
}

/* Function prints Intersection of arr1[] and arr2[]
   m is the number of elements in arr1[]
   n is the number of elements in arr2[] */
int getIntersection(const int *arr1, int *arr2, int m, int n, int *my_intersection)
{
  int i = 0, j = 0;
  int intersections_elements = 0;
  while (i < m && j < n)
  {
    if (arr1[i] < arr2[j])
      i++;
    else if (arr2[j] < arr1[i])
      j++;
    else /* if arr1[i] == arr2[j] */
    {
      my_intersection[intersections_elements] = arr2[j++];
      intersections_elements++;
      i++;
    }
  }
  return intersections_elements;
}

void createToken(char **tokens, int token_number, int token_first_pos, int token_last_pos, char *line) {
  int token_length = token_last_pos - token_first_pos;
  tokens[token_number] = malloc(token_length * sizeof(char));
  if (tokens[token_number] == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  strncpy(tokens[token_number], line + token_first_pos, (size_t) token_length);
}

void findTokens(char *line, int line_length) {
  int digit_token_flag = 0;
  char **tokens = allocateTokens(line_length);
  int digit_first_pos = 0;
  int digit_last_pos = 0;

  int tokens_amount = 0;
  for (int i = 0; i < line_length; i++) {
    if (!isspace(line[i])) {
      if (line[i] == '[') {
        digit_token_flag = 1;
        digit_first_pos = i + 1;
      } else if (line[i] == ']') {
        digit_token_flag = 0;
        digit_last_pos = i;
        createToken(tokens, tokens_amount, digit_first_pos, digit_last_pos, line);
        tokens_amount++;
      } else {
        if (digit_token_flag == 0) {
          createToken(tokens, tokens_amount, i, i + 1, line);
          tokens_amount++;
        }
      }
    }
  }
  for (int i = 0; i < tokens_amount; i++) {
    if (tokens[i][0] != 'U' && tokens[i][0] != '^' && tokens[i][0] != '(' && tokens[i][0] != ')')
      convertStrtoArr(tokens[i]);
    else
      printf("%s\n", tokens[i]);
  }
  printf("NEW\n");
  //free_memory(tokens, tokens_amount);
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

  int arr1[] = {1, 3, 3, 5, 6, 6, 8, 9, 70 };
  int arr2[] = {-29, 1, 3, 5, 6, 7, 9, 13, 15};

  int m = sizeof(arr1)/sizeof(arr1[0]);
  int n = sizeof(arr2)/sizeof(arr2[0]);

  printf("INTERSEC\n");

  int my_union[128] = {0};
  int union_elements = getUnion(arr1, arr2, m, n, my_union);

  int my_intersections[128] = {0};
  int intersections_elements = getIntersection(arr1, arr2, m, n, my_intersections);

  for(int i = 0; i < intersections_elements; i++) {
    printf("%d ", my_intersections[i]);
  }

  free(line);
  fclose(stream);
  exit(EXIT_SUCCESS);
  return 0;
}
