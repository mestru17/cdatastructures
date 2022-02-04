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
#include <stdio.h>

int main() {
  vector *vec = vector_create(10);

  for (int i = 0; i < 5; i++) {
    vector_insert(vec, 0, i * 2);
  }
  vector_print(vec);

  vector_insert(vec, 3, 42);
  for (int i = 0; i < vector_length(vec); i++) {
    printf("%d ", vector_get(vec, i));
  }
  printf("\n");

  printf("Peeked %d\n", vector_peek(vec));
  vector_push(vec, 84);
  vector_print(vec);
  printf("Peeked %d\n", vector_peek(vec));

  printf("\n");

  for (int i = 0; i < 5; i++) {
    int popped;
    if (!vector_pop(vec, &popped)) {
      fprintf(stderr, "Failed to pop value from vector");
      return 1;
    }
    printf("Popped: %d\n", popped);
    vector_print(vec);
  }

  vector_destroy(vec);
}
