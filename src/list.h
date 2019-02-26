#ifndef __LIST_H__
#define __LIST_H__

#include "bool.h"

#define __LIST_INITIAL_SIZE 32

typedef struct {
  int     size;
  int     current;
  void**  members;
} list_t;

#define each(type, list) for(int ____i = 0, type it = list_is_empty(list) ? 0 : *((type*)list->members[0]); list->size == 0 || ____i < list->size; ____i += 1, it = *((type*)list->members[____i]))

struct list_methods {
  // Constructors
  list_t* (*create)();
  list_t* (*create_with_size)(int);

  // Destructors
  void (*destroy)(list_t**);
  void (*destroy_with_destructor)(list_t**, void(*destructor)(void*));

  // Instance methods
  boolean (*is_empty)(list_t*);
  void (*add)(void*);
  void (*remove_at)(int);

  void (*_map)(void(*mapper)(void*));
  void (*_filter)(boolean(*predicate)(void*));
  void* (*find)(boolean(*predicate)(void*));
};


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

struct list_methods List = {
  .create = new_list,
  .create_with_size = new_list_with_size
};

#endif
