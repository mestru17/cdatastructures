// MIT License
//
// Copyright (c) 2022 Mathias Estrup
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "llist.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int value;
  struct node *next;
  struct node *previous;
} node;

typedef struct llist {
  node *head;
  node *tail;
  size_t length;
} llist;

static void link(node *previous, node *next) {
  if (previous != NULL) {
    previous->next = next;
  }
  if (next != NULL) {
    next->previous = previous;
  }
}

node *node_create(int value) {
  node *n = malloc(sizeof(node));
  if (n == NULL) {
    return NULL;
  }

  n->value = value;
  n->next = NULL;
  n->previous = NULL;
  return n;
}

static node *get_node(llist *list, size_t index) {
  node *n;
  if (index <= list->length / 2) {
    // Index closer to head than tail
    n = list->head;
    for (size_t i = 0; i < index; i++) {
      n = n->next;
    }
  } else {
    // Index closer to tail than head
    n = list->tail;
    for (size_t i = list->length - 1; i > index; i--) {
      n = n->previous;
    }
  }

  return n;
}

llist *llist_create() {
  llist *list = malloc(sizeof(llist));
  if (list == NULL) {
    return NULL;
  }

  list->head = NULL;
  list->tail = NULL;
  list->length = 0;
  return list;
}

llist *llist_create_from_values(int *values, size_t length) {
  assert(values != NULL &&
         "Failed to create linked list from values because value array pointer "
         "was NULL");

  llist *list = llist_create();
  for (int i = 0; i < length; i++) {
    int value = *(values + i);
    llist_push(list, value);
  }
  return list;
}

void llist_destroy(llist *list) {
  if (list != NULL) {
    llist_clear(list);
    free(list);
  }
}

size_t llist_length(llist *list) {
  assert(list != NULL &&
         "Failed to get length of linked list because pointer was NULL");
  return list->length;
}

bool llist_insert(llist *list, size_t index, int value) {
  assert(list != NULL &&
         "Failed to insert value into linked list because pointer was NULL");
  assert(index <= list->length &&
         "Failed to insert value into linked list because index was out of "
         "bounds");

  node *n = node_create(value);
  if (n == NULL) {
    return false;
  }

  if (list->head == NULL) {
    // No elements in list
    list->head = n;
    list->tail = n;
  } else if (index == 0) {
    // Push front
    link(n, list->head);
    list->head = n;
  } else if (index == list->length) {
    // Push back
    link(list->tail, n);
    list->tail = n;
  } else {
    // Insert in the middle
    node *ni = get_node(list, index);
    link(ni->previous, n);
    link(n, ni);
  }
  list->length++;
  return true;
}

int llist_remove(llist *list, size_t index) {
  assert(list != NULL &&
         "Failed to remove value from linked list because pointer was NULL");
  assert(index < list->length &&
         "Failed to remove value from linked list because index was out of "
         "bounds");

  node *n = get_node(list, index);
  if (index == 0) {
    list->head = n->next;
  } else if (index == list->length - 1) {
    link(n->previous, NULL);
    list->tail = n->previous;
  } else {
    link(n->previous, n->next);
  }
  list->length--;
  int value = n->value;
  free(n);
  return value;
}

bool llist_push(llist *list, int value) {
  assert(list != NULL &&
         "Failed to push value into linked list because pointer was NULL");
  return llist_insert(list, list->length, value);
}

int llist_peek(llist *list) {
  assert(list != NULL &&
         "Failed to peek value from linked list because pointer was NULL");
  return llist_get(list, list->length - 1);
}

int llist_pop(llist *list) {
  assert(list != NULL &&
         "Failed to peek value from linked list because pointer was NULL");
  return llist_remove(list, list->length - 1);
}

void llist_set(llist *list, size_t index, int value) {
  assert(list != NULL &&
         "Failed to set value in linked list because pointer was NULL");
  assert(index < list->length &&
         "Failed to set value in linked list because index was out of bounds");

  node *n = get_node(list, index);
  n->value = value;
}

int llist_get(llist *list, size_t index) {
  assert(list != NULL &&
         "Failed to get value from linked list because pointer was NULL");
  assert(
      index < list->length &&
      "Failed to get value from linked list because index was out of bounds");

  node *n = get_node(list, index);
  return n->value;
}

void llist_clear(llist *list) {
  assert(list != NULL &&
         "Failed to get value from linked list because pointer was NULL");

  node *n = list->head;
  node *tmp;
  while (n != NULL) {
    tmp = n;
    n = n->next;
    free(tmp);
  }
  list->head = NULL;
  list->tail = NULL;
  list->length = 0;
}

bool llist_contains(llist *list, int value) {
  assert(
      list != NULL &&
      "Failed to check if linked list contains value because pointer was NULL");

  for (node *ni = list->head; ni != NULL; ni = ni->next) {
    if (ni->value == value) {
      return true;
    }
  }
  return false;
}

bool llist_equals(llist *list1, llist *list2) {
  assert((list1 != NULL || list2 != NULL) &&
         "Failed to compare linked lists because at least one of the pointers "
         "was NULL");

  if (list1 == list2) {
    // The pointers refer to the same object
    return true;
  }

  if (list1->length != list2->length) {
    return false;
  }

  node *n1 = list1->head;
  node *n2 = list2->head;
  while (n1 != NULL) {
    if (n1->value != n2->value) {
      return false;
    }
    n1 = n1->next;
    n2 = n2->next;
  }
  return true;
}

bool llist_position(llist *list, int value, size_t *index) {
  assert(list != NULL &&
         "Failed to find position of element in linked list because list "
         "pointer was NULL");
  assert(index != NULL &&
         "Failed to return position of element in linked list because index "
         "pointer was NULL");

  size_t position = 0;
  for (node *ni = list->head; ni != NULL; ni = ni->next) {
    if (ni->value == value) {
      *index = position;
      return true;
    }
    position++;
  }
  return false;
}

bool llist_empty(llist *list) {
  assert(list != NULL &&
         "Failed to check if linked list is empty because pointer was NULL");
  return list->length == 0;
}

void llist_print(llist *list) {
  assert(list != NULL &&
         "Failed to print linked list because pointer was NULL");

  printf("[ ");
  node *n = list->head;
  if (n != NULL) {
    printf("%d", n->value);
    n = n->next;
  }
  while (n != NULL) {
    printf(" <-> %d", n->value);
    n = n->next;
  }
  printf(" ]\n");
}