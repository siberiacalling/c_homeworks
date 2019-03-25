#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

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
  char **array;
};

struct Stack *createStack(unsigned capacity) {
  struct Stack *stack = (struct Stack *) malloc(sizeof(struct Stack));
  stack->capacity = capacity;
  stack->top = -1;
  stack->array = (char **) malloc(128 * sizeof(char *));
  return stack;
}

int isFull(struct Stack *stack) { return stack->top == stack->capacity - 1; }

int isEmpty(struct Stack *stack) { return stack->top == -1; }

void push(struct Stack *stack, char *item) {
  if (isFull(stack))
    return;
  stack->array[++stack->top] = item;
}

char *pop(struct Stack *stack) {
  if (isEmpty(stack))
    return NULL;
  return stack->array[stack->top--];
}

void freeStack(struct Stack *stack) {
  if (isEmpty(stack)) {
//    for (int i = 0; i < 128; i++)
//      free(stack->array[i]);
    free(stack->array);
    free(stack);
  }
}

char *top(struct Stack *stack) {
  if (isEmpty(stack))
    return NULL;
  return stack->array[stack->top];
}
char **allocateTokens(int line_length) {
  char **tokens = (char **) malloc(line_length * sizeof(*tokens));
  if (tokens == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  return tokens;
}

int compare(const void *a, const void *b) {
  int int_a = *((int *) a);
  int int_b = *((int *) b);

  if (int_a == int_b) return 0;
  if (int_a < int_b) return -1;
  return 1;
}

int convertStrToArr(int *arr, const char *str) {
  // 128 max length
  int flag_digit = 0;
  int j = -1;
  if (str == NULL) {
    return 0;
  }
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
  // array length
  return j + 1;
//  for (int i = 0; i <= j; i++) {
//    printf("%d ", arr[i]);
//  }
//  printf("\n");

}

/* The symmetric difference of two sorted array.
   m is the number of elements in arr1[]
   n is the number of elements in arr2[] */

int getDiff(const int arr1[], const int arr2[], int m, int n, int *my_diff) {
//  int arr1[] = {1, 3, 3, 5, 6, 6, 8, 9, 70};
//  int arr2[] = {-29, 1, 3, 5, 6, 7, 9, 13, 15};
  int diff_elements_amount = 0;

  int i = 0, j = 0;
  while (i < n && j < m) {
    if (arr1[i] < arr2[j]) {
      my_diff[diff_elements_amount] = arr1[i];
      diff_elements_amount++;
      i++;
    } else if (arr2[j] < arr1[i]) {
      my_diff[diff_elements_amount] = arr2[j];
      diff_elements_amount++;
      j++;
    } else {
      i++;
      j++;
    }
  }
  if (n < m) {
    for (int k = i; k < m; k++) {
      my_diff[diff_elements_amount - 1] = arr1[k];
      diff_elements_amount++;

    }
  } else if (m < n) {
    for (int k = j; k < n; k++) {
      my_diff[diff_elements_amount - 1] = arr2[k];
      diff_elements_amount++;
    }
  }
  return diff_elements_amount - 1;
}

/* Union of arr1[] and arr2[]
   m is the number of elements in arr1[]
   n is the number of elements in arr2[] */

int getUnion(const int *arr1, const int *arr2, int m, int n, int *my_union) {
  int union_elements_amount = 0;

  if (m == 0) {
    for (int i = 0; i < n; i++) {
      my_union[i] = arr2[i];
    }
    return n;
  }

  if (n == 0) {
    for (int i = 0; i < m; i++) {
      my_union[i] = arr1[i];
    }
    return m;
  }

  int i = 0, j = 0;
  while (i < m && j < n) {
    if (arr1[i] < arr2[j]) {
      my_union[union_elements_amount] = arr1[i++];
      union_elements_amount++;
    } else if (arr2[j] < arr1[i]) {
      my_union[union_elements_amount] = arr2[j++];
      union_elements_amount++;
    } else {
      my_union[union_elements_amount] = arr2[j++];
      union_elements_amount++;
      i++;
    }
  }
  while (i < m) {
    my_union[union_elements_amount] = arr1[i++];
    union_elements_amount++;
  }
  while (j < n) {
    my_union[union_elements_amount] = arr2[j++];
    union_elements_amount++;
  }
  return union_elements_amount;
}

/* Intersection of arr1[] and arr2[]
   m is the number of elements in arr1[]
   n is the number of elements in arr2[] */
int getIntersection(const int *arr1, const int *arr2, int m, int n, int *my_intersection) {

  if (n == 0 || m == 0) {
    my_intersection[0] = '\0';
    return 0;
  }
  int i = 0, j = 0;

  int intersections_elements_amount = 0;
  while (i < m && j < n) {
    if (arr1[i] < arr2[j])
      i++;
    else if (arr2[j] < arr1[i])
      j++;
    else /* if arr1[i] == arr2[j] */
    {
      my_intersection[intersections_elements_amount] = arr2[j++];
      intersections_elements_amount++;
      i++;
    }
  }
  return intersections_elements_amount;
}

void createToken(char **tokens, int token_number, int token_first_pos, int token_last_pos, char *line) {
  int token_length = token_last_pos - token_first_pos;
  if (token_length == 0) {
    tokens[token_number] = NULL;
    return;
  }
  tokens[token_number] = malloc((token_length +1)* sizeof(char));
  if (tokens[token_number] == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  strncpy(tokens[token_number], line + token_first_pos, (size_t) token_length);
  tokens[token_number][token_length] = '\0';
}

int findTokens(char **tokens, char *line, int line_length) {
  int digit_token_flag = 0;

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
  return tokens_amount;
}
bool isSet(const char * token) {
  return token == NULL || (token[0] != 'U' && token[0] != '^' && token[0] != '(' && token[0] != ')'
&& token[0] != '\\');
} 
int shuntingYard(char **queue, char **tokens, int tokens_amount) {
  int queue_elements = 0;

  struct Stack *my_stack = createStack(128);

  for (int i = 0; i < tokens_amount; i++) {

    // if token = digits
    if (isSet(tokens[i])) {
      queue[queue_elements] = tokens[i];
      queue_elements++;
    } else if (tokens[i][0] == '(') {
      push(my_stack, tokens[i]);
    } else if (tokens[i][0] == ')') {
      char *current_top;
      if (isEmpty(my_stack)) {
        perror("missed ( 1");
        exit(EXIT_FAILURE);
      }
      current_top = top(my_stack);
      while (current_top[0] != '(') {
        queue[queue_elements] = pop(my_stack);
        queue_elements++;
        if (isEmpty(my_stack)) {
          perror("missed ( 2");
          exit(EXIT_FAILURE);
        }
        current_top = top(my_stack);
      }
      pop(my_stack);

    } else if (tokens[i][0] == '^' || tokens[i][0] == 'U') {
      while (!isEmpty(my_stack)) {
        char *current_top = top(my_stack);
        if (current_top[0] == '(') {
          push(my_stack, tokens[i]);
          break;
        }
        queue[queue_elements] = pop(my_stack);
        queue_elements++;
      }
      if (isEmpty(my_stack)) {
        push(my_stack, tokens[i]);
      }
    }
  }
  while (!isEmpty(my_stack)) {
    char *current_top = top(my_stack);
    if (current_top[0] == '(' || current_top[0] == ')') {
      perror("there are not closed bracket");
      exit(EXIT_FAILURE);
    }
    queue[queue_elements] = pop(my_stack);
    queue_elements++;
  }
  freeStack(my_stack);
  return queue_elements;
}
//void printTokens(char **tokens, int tokens_amount) {
//  for (int i = 0; i < tokens_amount; i++) {
//    int arr[128] = {0};
//    if (tokens[i][0] != 'U' && tokens[i][0] != '^' && tokens[i][0] != '(' && tokens[i][0] != ')'
//        && tokens[i][0] != '\\')
//      convertStrToArr(arr, tokens[i]);
//    else
//      printf("%s\n", tokens[i]);
//  }
//}

void calculateReversePolishNotation(char **queue, int queue_length) {
  struct Stack *my_stack = createStack((unsigned int) queue_length);
  for (int i = 0; i < queue_length; i++) {

    // if queue[i] = digits
    if (isSet(queue[i])) {
      push(my_stack, queue[i]);
    } else {
      if (!isEmpty(my_stack)) {
        char *set = pop(my_stack);
        int arr[128] = {0};
        int length_set1 = convertStrToArr(arr, set);

        char *set2 = pop(my_stack);
        int arr2[128] = {0};
        int length_set2 = convertStrToArr(arr2, set2);

        if (queue[i][0] == 'U') {
          int my_union[128] = {0};
          int union_elements_amount = getUnion(arr, arr2, length_set1, length_set2, my_union);

          char string[128], *pos = string;
          for (int j = 0; j < union_elements_amount; j++) {
            pos += sprintf(pos, "%d ", my_union[j]);
          }
          char *result = string;
          //printf("OB %s\n", result);
          push(my_stack, result);
        }

        if (queue[i][0] == '^') {
          int my_inters[128] = {0};
          int intersections_elements_amount = getIntersection(arr, arr2, length_set1, length_set2, my_inters);

          char string[128], *pos = string;
          if (intersections_elements_amount == 0) {
            string[0] = '\0';
          } else {
            for (int j = 0; j < intersections_elements_amount; j++) {
              pos += sprintf(pos, "%d ", my_inters[j]);
            }
          }
          char *result = string;
          //printf("IN %s\n", result);
          push(my_stack, result);

        }
      }
    }
  }
  char *result = pop(my_stack);
  int arr[128] = {0};
  int result_length = 0;
  if (result != NULL) {
    result_length = convertStrToArr(arr, result);
  }

  printf("[");
  for (int i = 0; i < result_length; i++) {
    if (i != result_length - 1) {
      printf("%i,", arr[i]);
    } else {
      printf("%i", arr[i]);
    }
  }
  printf("]\n");
  freeStack(my_stack);
}

//void free_char_array(char **array, int array_length) {
//  for (int i = 0; i < array_length; i++)
//    free(array[i]);
//  free(array);
//}

int main() {
//  FILE *stream = fopen("/home/anita/Desktop/c_homeworks/test.txt", "r");
//  if (stream == NULL) {
//    perror("fopen");
//    exit(EXIT_FAILURE);
//  }

  char *line = NULL;
  size_t len = 0;
  ssize_t line_length;

  while ((line_length = getline(&line, &len, stdin)) != -1) {
    //char **tokens = allocateTokens(128);
    char **tokens = (char **) malloc(line_length * sizeof(char *));
    if (tokens == NULL) {
      perror("malloc");
      exit(EXIT_FAILURE);
    }
   // fwrite(line, (size_t) line_length, 1, stdout);
    int tokens_amount = findTokens(tokens, line, (int) line_length);
    //printTokens(tokens, tokens_amount);

    //char **queue = allocateTokens(tokens_amount);
    char **queue = (char **) malloc(tokens_amount * sizeof(*tokens));
//    for (int i = 0; i < tokens_amount; i++) {
//      queue[i] = malloc(128 * sizeof(char));
//    }
    int queue_length = shuntingYard(queue, tokens, tokens_amount);
    calculateReversePolishNotation(queue, queue_length);

//    for (int i = 0; i < tokens_amount; i++)
//      free(queue[i]);
    free(queue);

    for (int i = 0; i < tokens_amount; i++)
      free(tokens[i]);
    free(tokens);
  }

//  int arr1[] = {1, 3, 3, 5, 6, 6, 8, 9, 70};
//  int arr2[] = {-29, 1, 3, 5, 6, 7, 9, 13, 15};

//  int arr1[] =  {3, 5, 6, 7};
//  int arr2[] = {1, 3,  8, 9, 70};
//  int m = sizeof(arr1) / sizeof(arr1[0]);
//  int n = sizeof(arr2) / sizeof(arr2[0]);
//
//  int my_union[128] = {0};
//  int union_elements_amount = getUnion(arr1, arr2, m, n, my_union);
//
//  int my_intersections[128] = {0};
//  int intersections_elements_amount = getIntersection(arr1, arr2, m, n, my_intersections);

//  int my_diff[128] = {0};
//  int diff_elements_amount = getDiff(arr1, arr2, m, n, my_diff);
//
//  printf("DIFF\n");
//  for(int i = 0; i < diff_elements_amount; i++) {
//    printf("%d ", my_diff[i]);
//  }

  free(line);
//  fclose(stream);
  return 0;
}
