#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "commons.h"

typedef struct array_adt {
  int capacity;     // Number of elements that the array can contain before enlarging
  int count;        // Number of elements present in the array
  
  void** elements;  // Backing real array
} array;

struct ArrayModule {
  /// Module-level members 
  const int initialCapacity;

  ///
  /// Factories
  ///

  /// Creates an empty array (note: it's capacity is Array.initialCapacity)
  array* (*empty)();

  /// Creates an array given a number of elements (note: elements are not copied)
  array* (*of)(int, ...);

  /// Creates an array given a number of elements, copying them with the given copy function
  array* (*from)(void*(*copyFunction)(void*), int, ...);

  /// Creates an empty array with the given capacity
  array* (*withCapacity)(int);
};

array* ___arrayEmpty();
array* ___arrayOf(int count, ...);
array* ___arrayFrom(void*(*copyFunction)(void*), int count, ...);
array* ___arrayWithCapacity(int capacity);

// "Class"
const struct ArrayModule Array = {
  .initialCapacity = 32,
  .empty = ___arrayEmpty,
  .of = ___arrayOf,
  .from = ___arrayFrom,
  .withCapacity = ___arrayWithCapacity
};

#endif
