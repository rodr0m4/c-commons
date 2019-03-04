#ifndef __ARRAY_H
#define __ARRAY_H

#include <stdint.h>

#include "kernel.h"

typedef struct array_adt {
  // Should arrays bigger than an int be allowed? maybe using a uint64 or shomething? 
  uint32_t capacity;     // Number of elements that the array_t can contain before enlarging
  uint32_t count;        // Number of elements present in the array
  
  void** elements;  // Backing buffer 
} array_t;

#define ARRAY_INITIAL_CAPACITY 32

array_t* array_empty();
array_t* array_of(uint32_t count, void* values[]);
array_t* array_from(void* (*copier)(void*), uint32_t count, void* values[]);
array_t* array_with_capacity(uint32_t capacity);

void array_destroy(array_t** ptr_to_array);
void array_destroy_with_free(array_t** ptr_to_array);
void array_destroy_with_destructor(array_t** ptr_to_array, void(*destructor)(void*));

bool_t array_is_empty(array_t* self);
int array_add(array_t* self, void* element);

// index is signed so we can have access from the back of the array
void* array_raw_get(array_t* self, int32_t index);

#define array_get(self, type, index) (type*) array_raw_get(self, index)

// @Improvement 
// - Find a way to not pass the block, so it can be called like foreach(...) { it }
// - Find a way to dereference it automatically 
//
// The if(true) looks dumb, but it is for generating it inside a new scope :)
#define foreach(type, self, block) if (!array_is_empty(self)) \
if (true) { \
  type* it = array_get(self, type, 0); \
  for (int i = 0; i < self->count; i += 1, it = array_get(self, type, i)) block \
}

#endif
