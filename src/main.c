#include <stdio.h>

#include "map.h"

void __print_int(bucket_t* entry) {
  printf("%s => %d\n", entry->key, *((int*) entry->value));
}

int main(int argc, char** argv) {
  map_t* map = map_create();

  char* key = "one";
  int value = 1;

  map_put(map, key, &value);

  map_foreach(map, __print_int);

  int other_value = 2;

  map_put(map, key, &other_value);

  map_foreach(map, __print_int);

  return 0;
}