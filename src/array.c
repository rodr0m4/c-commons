#include <stdlib.h>
#include <stdarg.h>

#include "commons.h"
#include "array.h"

array* ___arrayWithCapacity(int capacity) {
  array* this = (array*) malloc(sizeof(array));

  if (!this) return null;

  this->capacity = capacity;
  this->count = 0;
  this->elements = calloc(capacity, sizeof(void*));

  return this;
}

array* ___arrayEmpty() {
  return ___arrayWithCapacity(Array.initialCapacity);
}

array* ___arrayFrom(void*(*copyFunction)(void*), int count, ...) {
  array* this = ___arrayEmpty();

  if (!this) return null;

  va_list args;
  va_start(args, count);

  void* element;
  for (int i=0; i < count; i += 1) {
    element = va_arg(args, void*);

    this->elements[i] = copyFunction(element);
  }
  
  va_end(args);

  return this;
}

array* ___arrayOf(int count, ...) {
  
  // C is dumb.
  // Vararg passing is not supported by the standard, so there are only two alternatives:
  // 1. make va_list receiving versions of both Array::of and Array::from
  // 2. copy code from one to another
  //
  // Because I do not wish to pollute the global namespace with more shit, I went for 2). Possibly will migrate to 1)
  //
  // return ___arrayFrom(id, count, ...);

  array* this = ___arrayEmpty();

  if (!this) return null;

  va_list args;
  va_start(args, count);

  void* element;
  for (int i=0; i < count; i += 1) {
    element = va_arg(args, void*);

    this->elements[i] = id(element);
  }
  
  va_end(args);

  return this;
}

