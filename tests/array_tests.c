#include "../src/array.h"
#include "snow.h"

describe(arrays) {
  array_t* sut;
  before_each() {
    sut = Array.empty();
  }

  after_each() {
    Array.destroy(&sut);  
  }

  subdesc("Array creation") {
    it("An array is created with initial capacity of 32") {
      asserteq(sut->capacity, 32);
    }
  }
}

snow_main();
