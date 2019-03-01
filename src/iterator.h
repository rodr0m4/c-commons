#ifndef __ITERATOR_H_
#define __ITERATOR_H_

#include "kernel.h"
#include "array.h"

typedef union {
  // @Improvement
  // An array and a slice are the same, maybe two different data structures?
  // Or generate another array with a pointer to the start of the slice? (this last one is dangerous, who owns that memory?)
  array_t* array;
  int range;
} iterator_source;

typedef struct iterator_adt {
  int index;
  int count;

  iterator_source source;
} iterator_t;

defm(Iterator) {
  /// Factories
 
  /// Iterates over all the array
  iterator_t (*array)(array_t* source);

  /// Iterates over a slice of the array
  /// Note: -1 in `to` means until the end of the array
  iterator_t (*slice)(array_t* source, int from, int to);

  /// Iterates over a range
  /// @Improvement: This could be infinite
  iterator_t (*range)(int from, int to);


  /// "Instance methods"
  boolean (*has_next)(iterator_t self);
  
  /// Returns the next index (in a range iterator this is the value that you are looking for,
  /// in the array/slice this returns the index of the next element)
  int (*next_index)(iterator_t self);

  /// Returns the next element in the case of an array/slice iterator, otherwise null
  void* (*next_element)(iterator_t self);
};

// iterator_t ___iterator_array(array_t* source);
// iterator_t ___iterator_slice(array_t* source, int from, int to);
// iterator_t ___iterator_range(int from, int to);
//
// int ___iterator_next_index(iterator_t self);
// void* ___iterator_next_element(iterator_t self);
//
// impl(Iterator) {
  // .array = ___iterator_array,
  // .slice = ___iterator_slice,
  // .range = ___iterator_range,
//
  // .next_index = ___iterator_next_index,
  // .next_element = ___iterator_next_element
// };

#endif
