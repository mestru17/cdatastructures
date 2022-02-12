
#include <stdio.h>

#include "llist.h"

int main() {
  llist *list = llist_create();
  llist_print(list);

  llist_insert(list, 0, 42);
  llist_print(list);

  llist_insert(list, 0, 84);
  llist_print(list);

  llist_insert(list, 2, 168);
  llist_print(list);

  llist_insert(list, 1, 336);
  llist_print(list);

  printf("\n");

  int value = llist_remove(list, 2);
  printf("Removed %d\n", value);
  llist_print(list);

  printf("\n");

  for (int i = 0; i < 10; i++) {
    llist_push(list, i * 2);
    llist_print(list);
  }

  printf("\n");

  value = llist_pop(list);
  printf("Popped %d\n", value);
  llist_print(list);

  printf("\n");

  value = llist_peek(list);
  printf("Peeked %d\n", value);
  llist_print(list);

  printf("\n");

  llist_set(list, 9, 12345);

  for (size_t i = 0; i < llist_length(list); i++) {
    printf("%d ", llist_get(list, i));
  }
  printf("\n");

  printf("\n");

  int values[12] = {84, 336, 168, 0, 2, 4, 6, 8, 10, 12345, 14, 16};
  llist *list2 = llist_create_from_values(values, 12);
  llist_print(list2);

  char *equals = llist_equals(list, list) ? "true" : "false";
  printf("list == list? %s\n", equals);
  equals = llist_equals(list, list2) ? "true" : "false";
  printf("list == list2? %s\n", equals);
  equals = llist_equals(list, list2) ? "true" : "false";
  llist_destroy(list2);

  list2 = llist_create();
  llist_print(list2);
  llist_push(list2, 42);
  equals = llist_equals(list, list2) ? "true" : "false";
  printf("list == list2? %s\n", equals);
  llist_destroy(list2);

  printf("\n");

  char *contains;
  for (int i = llist_length(list) - 1; i >= 0; i--) {
    value = llist_get(list, i);
    contains = llist_contains(list, value) ? "true" : "false";
    printf("list contains %d? %s\n", value, contains);
  }
  value = 54321;
  contains = llist_contains(list, value) ? "true" : "false";
  printf("list contains %d? %s\n", value, contains);

  printf("\n");

  llist_clear(list);
  llist_print(list);

  llist_destroy(list);
}
