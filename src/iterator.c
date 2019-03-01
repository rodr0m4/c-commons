#include "iterator.h"

iterator_t ___iterator_array(array_t* source) {
  iterator_t iterator = {
    .index = 0,
    .count = source->count,

    .source = source
  };

  return iterator;
}
