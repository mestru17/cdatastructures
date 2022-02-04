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

#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stddef.h>

// A dynamic array.
typedef struct vector vector;

// Checks that a given vector capacity is not 0 and will not cause an unsigned
// integer wrap.
bool vector_capacity_ok(size_t capacity);

// Creates a new vector with given capacity. If there is any allocation errors,
// then NULL is returned. `capacity` must not be 0 and must not be large enough
// that it would cause an unsigned integer wrap to allocate space for this
// vector. If unsure, call the vector_capacity_ok function first. When a vector
// created using this function is no longer needed, it should be freed by
// calling the vector_destroy function to avoid memory leaking.
vector *vector_create(size_t capacity);

// Destroys a given vector, freeing the allocated memory. Does nothing if
// `vec` is NULL.
void vector_destroy(vector *vec);

// Gets the length of a given vector. `vec` must not be NULL.
size_t vector_length(vector *vec);

// Checks that a given vector is empty (holds no values). `vec` must not be
// NULL.
bool vector_empty(vector *vec);

// Checks that a given vector is full (has no more capacity). Note that a
// vector being full does not mean that new values cannot be added -
// it only means that adding any more values will cause the vector to grow.
// `vec` must not be NULL.
bool vector_full(vector *vec);

// Inserts a given value into a given vector at a given index. Returns true if
// the insertion succeeds and false if `vec` is full and needs to grow but the
// growing operation fails. The growing operation can fail if either `vec` is
// too large to grow or an error occurs during re-allocation. `vec` must not
// be NULL or empty and `index` must be within bounds.
bool vector_insert(vector *vec, size_t index, int value);

// Gets a value at a given index in a given vector. `vec` must not be NULL or
// empty and `index` must be within bounds.
int vector_get(vector *vec, size_t index);

// Pushes a value onto the end of a given vector. Is functionally equivalent to
// vector_insert(vec, vector_length(vec), value). See vector_insert for failure
// conditions. `vec` must not be NULL.
bool vector_push(vector *vec, int value);

// Gets the last value in a given vector. Is functionally equivalent to
// vector_get(vec, vector_length(vec) - 1). `vec` must not be NULL.
int vector_peek(vector *vec);

// Pops a value off the end of a given vector. The value is popped into
// `value`. Returns false if the operation tried to shrink the vector and
// failed because of an error during re-allocation. Otherwise returns true.
// Regardless of the return value, the popped value will be popped into
// `value`. `vec` and `value` must not be NULL.
bool vector_pop(vector *vec, int *value);

// Prints a string representation of a given vector. `vec` must not be NULL.
void vector_print(vector *vec);

#endif
