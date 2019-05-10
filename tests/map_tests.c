#include "snow.h"
#include "../src/map.h"

describe(maps) {
  map_t* sut;

  before_each() {
    sut = map_empty();
  }

  after_each() {
    map_destroy(&sut);
  }

  subdesc("constructors") {
    it("should be constructed empty, with initial capacity and hashing function") {
      asserteq(sut->capacity, MAP_INITIAL_CAPACITY);
      asserteq(sut->count, 0);
      assert(sut->hash_function);
      assert(sut->occupancy_mask);
      asserteq(*(sut->occupancy_mask), 0);
      assert(sut->entries);
    }
  }
}