#ifndef __ARRAY_H
#define __ARRAY_H

#include "kernel.h"
#include "boolean.h"

typedef struct array_adt {
  int capacity;     // Number of elements that the array_t can contain before enlarging
  int count;        // Number of elements present in the array_t
  
  void** elements;  // Backing real array_t
} array_t;

DEF(Array) {
  /// Module-level members 
  const int initial_capacity;

  ///
  /// Factories
  ///

  /// Creates an empty array_t (note: it's capacity is Array.initial_capacity)
  array_t* (*empty)();

  /// Creates an array_t given a number of elements (note: elements are not copied)
  array_t* (*of)(int, void* values[]);

  /// Creates an array_t given a number of elements, copying them with the given copy function
  array_t* (*from)(void*(*copy_function)(void*), int, void* values[]); 

  /// Creates an empty array_t with the given capacity
  array_t* (*with_capacity)(int);

  ///
  /// Destructors
  ///

  /// Destroys the array_t, does not do anything with its members
  void (*destroy)(array_t** ptr_to_array);

  /// Destroys the array_t, and calls free on each of its members
  void (*destroy_with_free)(array_t** ptr_to_array);

  /// Destroys the array_t, and calls the given destructor for each of its members.
  void (*destroy_with_destructor)(array_t** ptr_to_array, void(*destructor)(void*));

  ///
  /// "Methods"
  ///

  /// Checks if list is empty
  boolean (*is_empty)(array_t* self);

  /// Adds an element to the array_t
  /// 1) If it coult not allocate memory it will return -1
  int (*add)(array_t* self, void* element);
};

array_t* ___array_empty();
array_t* ___array_of(int count, void* values[]);
array_t* ___array_from(void*(*copy_function)(void*), int count, void* values[]);
array_t* ___array_with_capacity(int capacity);

boolean ___array_is_empty(array_t* self);
int ___array_add(array_t* self, void* element);

void ___array_destroy(array_t** ptr_to_array);
void ___array_destroy_with_free(array_t** ptr_to_array);
void ___array_destroy_with_destructor(array_t** ptr_to_array, void(*destructor)(void*));

IMPL(Array) {
  .initial_capacity = 32, 
  
  .empty = ___array_empty,
  .of = ___array_of,
  .from = ___array_from,
  .with_capacity = ___array_with_capacity,

  .is_empty = ___array_is_empty,
  .add = ___array_add,

  .destroy = ___array_destroy,
  .destroy_with_free = ___array_destroy_with_free,
  .destroy_with_destructor = ___array_destroy_with_destructor
};

#endif
