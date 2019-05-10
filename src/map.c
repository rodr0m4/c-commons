#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "kernel.h"
#include "map.h"
#include "array.h"

uint32_t _sdbm(char* key) {
  uint32_t address = 0;
  
  for (int i = 0; key[i] != '\0'; i += 1) {
    address = key[i] + (address << 6);
  }
  
  return address;
}

map_t* map_empty() {
  map_t* self = (map_t*) malloc(sizeof(map_t));  

  if (!self) return null;

  self->capacity = MAP_INITIAL_CAPACITY;
  self->count = 0;
  self->entries = (bucket_t**) calloc(MAP_INITIAL_CAPACITY, sizeof(bucket_t*));
  self->occupancy_mask = (char*) calloc(MAP_INITIAL_CAPACITY / 8, sizeof(char));
  self->hash_function = _sdbm;

  return self;
}

int map_put(map_t* self, char* key, void* value) {
  uint32_t index = self->hash_function(key) & (self->capacity - 1); // Fast modulo
  bucket_t* current_entry = self->entries[index];

  if ((self->count + 1) / self->capacity >= MAP_LOAD_FACTOR) {
    // Rehash! TODO: Handle them
    return -3;
  }

  if (!current_entry) {
    // No collision

    current_entry = (bucket_t*) malloc(sizeof(bucket_t));

    if (!current_entry) return -1;

    current_entry->key = key;
    current_entry->value = value;
    current_entry->next = null;

    self->entries[index] = current_entry;

    return 0;
  }

  if (!strcmp(key, current_entry->key)) { // TODO: Handle collisions
    // Same key, we replace the value
    current_entry->value = value;
  }

  return -2; // Collision! TODO: handle them
}

void map_foreach(map_t* self, void(*consumer)(bucket_t*)) {
  // TODO: Use occupancy
  for(int i = 0; i < self->capacity; i += 1) {
    for(bucket_t* current = self->entries[i]; current; current = current->next) {
      consumer(current);
    }
  }
}

void map_destroy(map_t** ptr_to_map) {
  if (!ptr_to_map) return;
  map_t* map = *ptr_to_map;
  if (!map) return;

  // TODO: Free entries

  free(map->entries);
  free(map->occupancy_mask);
  free(map);

  *ptr_to_map = null;
}
