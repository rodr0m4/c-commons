#ifndef __LIST_H__
#define __LIST_H__

#include "bool.h"

#define __LIST_INITIAL_SIZE 32

typedef struct {
  int     size;
  int     current;
  void**  members;
} list_t;

/// Constructors
list_t* new_list();
list_t* new_list_with_size(int size);

/// Operations
boolean list_is_empty(list_t* list);
void list_add(list_t* list, void* element);

/// Destructor$a

// Destroys the list, does not destroy elementss
void destroy_list(list_t** ptr_to_list);

// Destroy the list, calls destructor in each element, for example:
// destroy_list_with_destructor(&list, free);
void destroy_list_with_destructor(list_t** ptr_to_list, void(*destructor)(void*));

#endif
