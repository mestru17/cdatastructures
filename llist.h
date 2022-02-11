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

#ifndef LLIST_H
#define LLIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct llist llist;

llist *llist_create();
void llist_destroy(llist *list);
size_t llist_length(llist *list);
bool llist_insert(llist *list, size_t index, int value);
int llist_remove(llist *list, size_t index);
bool llist_push(llist *list, int value);
int llist_peek(llist *list);
int llist_pop(llist *list);
void llist_set(llist *list, size_t index, int value);
int llist_get(llist *list, size_t index);
void llist_clear(llist *list);
void llist_print(llist *list);

#endif