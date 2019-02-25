#include <stdlib.h>

#include "list.h"
#include "bool.h"
#include "commons.h"

list_t* new_list() {
  return new_list_with_size(__LIST_INITIAL_SIZE);
}

list_t* new_list_with_size(int size) {
  list_t* this = (list_t*) malloc(sizeof(list_t));

  if (!this) return NULL;

  this->size = size;
  this->current = 0; 
  this->members = calloc(size, sizeof(list_t));

  return this;
}

boolean list_is_empty(list_t* list) {
  return list->current == 0; 
}

void list_add(list_t* list, void* element) {
  list->members[list->current] = element;
  list->current += 1;
}

void destroy_list(list_t** ptr_to_list) {
  destroy_list_with_destructor(ptr_to_list, noop);  // Calling noop a lot of times O(n). Should we optimize it?
}

void destroy_list_with_destructor(list_t** ptr_to_list, void(*destructor)(void*)) {
  if (!ptr_to_list) return;

  list_t* list = *ptr_to_list;

  if (!list) return;

  for (int i=0; i < list->size; i += 1) {
    destructor(list->members[i]);
  }

  //free(list->members); // ????
  free(list);

  list = NULL;
}
