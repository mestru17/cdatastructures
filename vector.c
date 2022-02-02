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

typedef struct vector {
	int *values;
	size_t length;
	size_t capacity;
} vector;

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

// Grows a given vector's capacity to twice what it was. Returns true if
// growing succeeds and false if `vec` is too large to grow or an error
// occurs during re-allocation. `vec` is too large to grow if the expanded
// capacity would cause an unsigned integer wrap.
static bool grow(vector *vec) {
	size_t new_capacity = vec->capacity * 2;
	if (!vector_capacity_ok(new_capacity)) {
		return false;
	}

	int *new_values = realloc(vec, new_capacity * sizeof(int));
	if (new_values == NULL) {
		return false;
	}

	vec->capacity = new_capacity;
	vec->values = new_values;
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

int vector_get(vector *vec, size_t index) {
	assert(vec != NULL && "Failed to get value from vector because pointer was NULL");
	assert(!vector_empty(vec) && "Failed to get element from vector because vector was empty");
	assert(index < vec->length && "Failed to get element from vector because index was out of bounds");
	return *(vec->values + index);
}

bool vector_push(vector *vec, int value) {
	assert(vec != NULL && "Failed to push value onto vector because pointer was NULL");
	return vector_insert(vec, vec->length, value);
}

int vector_peek(vector *vec) {
	assert(vec != NULL && "Failed to peek value from vector because pointer was NULL");
	return vector_get(vec, vec->length - 1);
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

