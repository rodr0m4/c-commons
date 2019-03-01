#ifndef __ARRAY_H
#define __ARRAY_H

#include "kernel.h"
#include "boolean.h"
// #include "iterator.h"

typedef struct array_adt {
  // Should arrays bigger than an int be allowed? maybe using a uint64 or shomething? 
  int capacity;     // Number of elements that the array_t can contain before enlarging
  int count;        // Number of elements present in the array
  
  void** elements;  // Backing buffer 
} array_t;

defm(Array) {
  /// Module-level members 
  const int initial_capacity;

  ///
  /// Factories
  ///

  /// Creates an empty array (note: it's capacity is Array.initial_capacity)
  array_t* (*empty)();

  /// Creates an array given a number of elements (note: elements are not copied)
  array_t* (*of)(int, void* values[]);

  /// Creates an array given a number of elements, copying them with the given copy function
  array_t* (*from)(void*(*copy_function)(void*), int, void* values[]); 

  /// Creates an empty array with the given capacity
  array_t* (*with_capacity)(int);

  ///
  /// Destructors
  ///

  /// Destroys the array, does not do anything with its members
  void (*destroy)(array_t** ptr_to_array);

  /// Destroys the array, and calls free on each of its members
  void (*destroy_with_free)(array_t** ptr_to_array);

  /// Destroys the array, and calls the given destructor for each of its members.
  void (*destroy_with_destructor)(array_t** ptr_to_array, void(*destructor)(void*));

  ///
  /// "Methods"
  ///

  /// Checks if list is empty
  boolean (*is_empty)(array_t* self);

  /// Adds an element to the array
  /// 1) If it coult not allocate memory it will return -1
  int (*add)(array_t* self, void* element);

  /// Returns the iterator view for the array
  // iterator_t (*iterator)(array_t* self);
};

array_t* ___array_empty();
array_t* ___array_of(int count, void* values[]);
array_t* ___array_from(void*(*copy_function)(void*), int count, void* values[]);
array_t* ___array_with_capacity(int capacity);

void ___array_destroy(array_t** ptr_to_array);
void ___array_destroy_with_free(array_t** ptr_to_array);
void ___array_destroy_with_destructor(array_t** ptr_to_array, void(*destructor)(void*));

boolean ___array_is_empty(array_t* self);
int ___array_add(array_t* self, void* element);
// iterator_t ___array_iterator(array_t* self);

impl(Array) {
  .initial_capacity = 32, 
  
  .empty = ___array_empty,
  .of = ___array_of,
  .from = ___array_from,
  .with_capacity = ___array_with_capacity,

  .is_empty = ___array_is_empty,
  .add = ___array_add,
  // .iterator = ___array_iterator,

  .destroy = ___array_destroy,
  .destroy_with_free = ___array_destroy_with_free,
  .destroy_with_destructor = ___array_destroy_with_destructor
};

#endif
