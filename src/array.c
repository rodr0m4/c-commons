#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "kernel.h"

array_t* array_with_capacity(uint32_t capacity) {
  array_t* this = (array_t*)malloc(sizeof(array_t));

  if (!this) return null;

  this->capacity = capacity;
  this->count = 0;
  this->elements = calloc(capacity, sizeof(void*));

  if (!this->elements) {
    free(this);
    return null;
  }

  return this;
}

array_t* array_empty() { return array_with_capacity(ARRAY_INITIAL_CAPACITY); }

array_t* array_from(void* (*copier)(void*), uint32_t count, void* values[]) {
  array_t* this = array_empty();
  this->count = count;

  if (!this) return null;

  void* element;
  for (int i = 0; i < count; i += 1) {
    element = values[i];

    this->elements[i] = copier(element);
  }

  return this;
}

array_t* array_of(uint32_t count, void* values[]) {
  return array_from(id, count, values);
}

void array_destroy_with_destructor(array_t** ptr_to_array,
                                   void (*destructor)(void*)) {
  if (!ptr_to_array) return;

  array_t* self = *ptr_to_array;

  if (!self) return;

  for (int i = 0; i < self->count; i += 1) {
    destructor(self->elements[i]);
  }

  free(self->elements);
  free(self);

  *ptr_to_array = null;
}

void array_destroy(array_t** ptr_to_array) {
  array_destroy_with_destructor(ptr_to_array, noop);
}

void array_destroy_with_free(array_t** ptr_to_array) {
  array_destroy_with_destructor(ptr_to_array, free);
}

// @Improvement
// Find a way to factor out shared code between shallow and deep copies
// NOTE: Probably will need closures :(
array_t* array_shallow_copy(array_t* source) {
  array_t* copy = (array_t*)malloc(sizeof(array_t));

  if (!copy) return null;

  uint32_t capacity = source->capacity;
  copy->capacity = capacity;
  copy->count = source->count;

  // We don't calloc so we traverse the source only once.
  copy->elements = (void**)(malloc(capacity * sizeof(void*)));

  if (!copy->elements) {
    free(copy);
    return null;
  }

  memcpy(copy->elements, source->elements, capacity);

  return copy;
}

array_t* array_deep_copy(array_t* source, void* (*copier)(void*)) {
  array_t* copy = (array_t*)malloc(sizeof(array_t));

  if (!copy) return null;

  uint32_t capacity = source->capacity;
  copy->capacity = capacity;
  copy->count = source->count;

  // We don't calloc so we traverse the source only once.
  copy->elements = (void**)(malloc(capacity * sizeof(void*)));

  if (!copy->elements) {
    free(copy);
    return null;
  }

  for (int i = 0; i < capacity; i += 1) {
    copy->elements[i] =
        i < copy->count ? copier(source->elements[i]) :  // We copy it
            copy->elements[i];  // We are at the end of the array, so we just
                                // nullify the pointer
  }

  return copy;
}

bool_t array_is_empty(array_t* self) { return self->count == 0; }

/// This allocates more memory if needed
int ensure_capacity(array_t* self, int capacity) {
  if (self->capacity >= capacity) return 0;

  if (self->capacity == 0) {
    // We should set up the aray for the first time
    int new_capacity = ARRAY_INITIAL_CAPACITY;
    void** elements = malloc(new_capacity * sizeof(void*));

    if (!elements) return -1;

    // @Improvement
    // Is this dangerous?
    free(self->elements);

    self->capacity = new_capacity;
    self->elements = elements;
    self->count = 0;

    return 0;
  }

  int new_capacity = self->capacity * 2;

  void** new_elements = malloc(self->capacity * sizeof(void*));
  if (!new_elements) return -1;

  free(self->elements);

  for (int new_array_index = 0; new_array_index < new_capacity;
       new_array_index += 1) {
    new_elements[new_array_index] =
        new_array_index > self->count ? 0 : self->elements[new_array_index];
  }

  self->elements = new_elements;
  self->capacity = new_capacity;

  return 0;
}

int array_add(array_t* self, void* element) {
  if (ensure_capacity(self, self->count + 1) != 0)
    return -1;  // We failed allocating more memory :(

  self->elements[self->count] = element;
  self->count += 1;

  return 0;
}

void* array_raw_get(array_t* self, int32_t index) {
  uint32_t real_index = index < 0 ? self->count + index : index;

  // @Improvement
  // Optimize this (bounds checking will always be slow tho)
  if (self->count == 0 || index > 0 && (index > self->count) ||
      index < 0 && (index * -1 > self->count)) {
    // Out of bounds
    return null;
  }

  return self->elements[real_index];
}