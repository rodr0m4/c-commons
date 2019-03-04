#ifndef __ARRAY_H
#define __ARRAY_H

#include <stdint.h>

#include "kernel.h"
#include "boolean.h"

typedef struct array_adt {
  // Should arrays bigger than an int be allowed? maybe using a uint64 or shomething? 
  size_t capacity;     // Number of elements that the array_t can contain before enlarging
  size_t count;        // Number of elements present in the array
  
  void** elements;  // Backing buffer 
} array_t;

#define ARRAY_INITIAL_CAPACITY 32

array_t* array_empty();
array_t* array_of(size_t count, void* values[]);
array_t* array_from(void* (*copier)(void*), size_t count, void* values[]);
array_t* array_with_capacity(size_t capacity);

void array_destroy(array_t** ptr_to_array);
void array_destroy_with_free(array_t** ptr_to_array);
void array_destroy_with_destructor(array_t** ptr_to_array, void(*destructor)(void*));

bool_t array_is_empty(array_t* self);
int array_add(array_t* self, void* element);

// index is signed so we can have access from the back of the array
void* array_raw_get(array_t* self, int32_t index);

#define array_get(self, type, index) (type*) array_raw_get(self, index)

#endif
