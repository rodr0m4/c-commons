#include <stdlib.h>
#include <stdarg.h>

#include "kernel.h"
#include "array.h"

array_t* ___array_with_capacity(int capacity) {
  array_t* this = (array_t*) malloc(sizeof(array_t));

  if (!this) return null;

  this->capacity = capacity;
  this->count = 0;
  this->elements = calloc(capacity, sizeof(void*));

  return this;
}

array_t* ___array_empty() {
  return ___array_with_capacity(Array.initial_capacity);
}

array_t* ___array_from(void*(*copy_function)(void*), int count, void* values[]){
  array_t* this = ___array_empty();
  this->count = count;

  if (!this) return null;

  void* element;
  for (int i=0; i < count; i += 1) {
    element = values[i]; 

    this->elements[i] = copy_function(element);
  }
  
  return this;
}

array_t* ___array_of(int count, void* values[]) {
  return ___array_from(id, count, values);
}

void ___array_destroy_with_destructor(array_t** ptr_to_array, void(*destructor)(void*)) {
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

void ___array_destroy(array_t** ptr_to_array) {
  ___array_destroy_with_destructor(ptr_to_array, noop);
}

void ___array_destroy_with_free(array_t** ptr_to_array) {
  ___array_destroy_with_destructor(ptr_to_array, free);
}


boolean ___array_is_empty(array_t* self) {
  return self->count == 0;
}

/// This allocates more memory if needed
int ensure_capacity(array_t* self, int capacity) {
  if (self->capacity >= capacity) return 0;
  int new_capacity = self->capacity * 2 * sizeof(void*);

  void** new_elements = malloc(new_capacity);
  if (!new_elements) return -1;

  for (int new_array_index = 0; new_array_index < new_capacity; new_array_index += 1) {
    new_elements[new_array_index] = new_array_index > self->count ? 0 : self->elements[new_array_index];
  }

  return 0;
}

int ___array_add(array_t* self, void* element) {
  if (ensure_capacity(self, self->count + 1) != 0) {
    // We failed allocating more memory :(
    return -1;
  }

  self->elements[self->count] = element;

  return 0;
}
