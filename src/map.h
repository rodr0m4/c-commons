#ifndef __MAP_H__
#define __MAP_H__

#include <stdint.h>

#define MAP_INITIAL_CAPACITY 32 // This must be a multiple of 8, because occupation mask
#define MAP_LOAD_FACTOR 0.75

typedef struct bucket_adt {
  char* key;                /// null-terminated unique identifier
  void* value;

  struct bucket_adt* next;
} bucket_t;

typedef struct map_adt {
  bucket_t** entries;

  uint32_t capacity;
  uint32_t count;
  char** occupancy_mask;  /// bitmask of ocuppied slots

  uint32_t (*hash_function)(char*); // Key -> Position
} map_t;


map_t* map_empty();

int map_put(map_t* self, char* key, void* value);
void map_foreach(map_t* self, void(*consumer)(bucket_t*));

void map_destroy(map_t** ptr_to_map);

#endif
