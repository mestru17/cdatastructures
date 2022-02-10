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
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "vector.h"

#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// How much to scale vector capacity by when growing.
static const double GROWTH_FACTOR = 2.0;

// How much to scale vector capacity by when shrinking.
static const double SHRINK_FACTOR = 1.0 / GROWTH_FACTOR;

// Threshold at which to shrink vector. Vector will be shrunk if its length
// is less than or equal to `SHRINK_THRESHOLD` * vector capacity AND the
// shrunken capacity is greater than or equal to `MIN_SHRINK_CAPACITY`; see the
// should_shrink function if necessary.
static const double SHRINK_THRESHOLD = 0.3;

// The minimum capacity that a vector is able to shrunk to. In other words, a
// vector will never be shrunk to have a capacity smaller than this; see
// `SHRINK_THRESHOLD` for more details.
static const size_t MIN_SHRINK_CAPACITY = 4;

typedef struct vector {
  int *values;
  size_t length;
  size_t capacity;
} vector;

// Scales a given vectors capacity by a given factor. Returns true if the
// resizing succeeds and false if `vec` is too large to grow or an error
// occurs during re-allocation. `vec` is too large to grow if the scaled
// capacity would cause an unsigned integer wrap.
static bool resize(vector *vec, double scale_factor) {
  size_t new_capacity = vec->capacity * scale_factor;
  if (!vector_capacity_ok(new_capacity)) {
    return false;
  }

  int *new_values = realloc(vec->values, new_capacity * sizeof(int));
  if (new_values == NULL) {
    return false;
  }

  vec->capacity = new_capacity;
  vec->values = new_values;
  return true;
}

// Grows a given vector by `GROWTH_FACTOR`.
static bool grow(vector *vec) {
  return resize(vec, GROWTH_FACTOR);
}

// Shrinks a given vector by `SHRINK_FACTOR`.
static bool shrink(vector *vec) {
  return resize(vec, SHRINK_FACTOR);
}

// Checks if a given vector has enough excess capacity that it should shrink.
static bool should_shrink(vector *vec) {
  size_t threshold = SHRINK_THRESHOLD * vec->capacity;
  size_t shrunken_capacity = SHRINK_FACTOR * vec->capacity;
  return vec->length <= threshold && shrunken_capacity >= MIN_SHRINK_CAPACITY;
}

bool vector_capacity_ok(size_t capacity) {
  return capacity > 0 && capacity <= SIZE_MAX / sizeof(int);
}

vector *vector_create(size_t capacity) {
  assert(vector_capacity_ok(capacity) && "Failed to create vector because capacity was 0 or would cause an unsigned integer wrap");

  vector *vec = malloc(sizeof(vector));
  if (vec == NULL) {
    return NULL;
  }

  vec->values = malloc(capacity * sizeof(int));
  if (vec->values == NULL) {
    free(vec);
    return NULL;
  }

  vec->capacity = capacity;
  vec->length = 0;
  return vec;
}

void vector_destroy(vector *vec) {
  if (vec != NULL) {
    free(vec->values);
    free(vec);
  }
}

size_t vector_length(vector *vec) {
  assert(vec != NULL && "Failed to get vector length because pointer was NULL");
  return vec->length;
}

bool vector_empty(vector *vec) {
  assert(vec != NULL && "Failed to check if vector was empty because pointer was NULL");
  return vec->length == 0;
}

bool vector_full(vector *vec) {
  assert(vec != NULL && "Failed to check if vector was full because pointer was NULL");
  return vec->length == vec->capacity;
}

int vector_get(vector *vec, size_t index) {
  assert(vec != NULL && "Failed to get value from vector because pointer was NULL");
  assert(!vector_empty(vec) && "Failed to get element from vector because vector was empty");
  assert(index < vec->length && "Failed to get element from vector because index was out of bounds");
  return *(vec->values + index);
}

void vector_set(vector *vec, size_t index, int value) {
  assert(vec != NULL && "Failed to set value in vector because pointer was NULL");
  assert(!vector_empty(vec) && "Failed to set value in vector because it was empty");
  assert(index < vec->length && "Failed to set element in vector because index was out of bounds");
  *(vec->values + index) = value;
}

bool vector_insert(vector *vec, size_t index, int value) {
  assert(vec != NULL && "Failed to insert value into vector because pointer was NULL");
  assert(index >= 0 && index <= vec->length && "Failed to insert value into vector because index was out of bounds");

  // Grow array if necessary
  if (vector_full(vec) && !grow(vec)) {
    return false;
  }

  vec->length++;
  // Move elements forward if necessary
  if (vec->length > 0 && index < vec->length) {
    int *src = vec->values + index;
    int *dst = src + 1;
    size_t num = vec->length - 1 - index;
    memmove(dst, src, num * sizeof(int));
  }
  *(vec->values + index) = value;
  return true;
}

bool vector_remove(vector *vec, size_t index, int *value) {
  assert(vec != NULL && "Failed to remove value from vector because pointer was NULL");

  *value = vector_get(vec, index);

  // Move elements backwards if necessary
  if (index < vec->length - 1) {
    int *dst = vec->values + index;
    int *src = dst + 1;
    size_t num = vec->length - 1 - index;
    memmove(dst, src, num * sizeof(int));
  }
  vec->length--;

  // Shrink if there is a excess capacity
  return !should_shrink(vec) || shrink(vec);
}

bool vector_push(vector *vec, int value) {
  assert(vec != NULL && "Failed to push value onto vector because pointer was NULL");
  return vector_insert(vec, vec->length, value);
}

int vector_peek(vector *vec) {
  assert(vec != NULL && "Failed to peek value from vector because pointer was NULL");
  return vector_get(vec, vec->length - 1);
}

bool vector_pop(vector *vec, int *value) {
  assert(vec != NULL && "Failed to pop value from vector because vector pointer was NULL");
  assert(value != NULL && "Failed to pop value from vector because value pointer was NULL");
  return vector_remove(vec, vec->length - 1, value);
}

void vector_print(vector *vec) {
  printf("[ ");
  for (int i = 0; i < vec->capacity; i++) {
    if (i < vec->length) {
      int value = *(vec->values + i);
      printf("%d, ", value);
    } else {
      printf("%s", "_, ");
    }
  }
  printf("]\n");
}

