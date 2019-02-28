#include "../src/array.h"
#include "snow.h"

describe(arrays) {
  array_t* sut;

  subdesc("Array creation") {
    before_each() {
      sut = Array.empty();
    }

    after_each() {
      Array.destroy(&sut);  
    }

    subdesc("Array::empty") {
      it("Creates an empty array, with capacity of 32") {
        asserteq(sut->count, 0);
        asserteq(sut->capacity, 32);
      }
    }
  }

  subdesc("Array destruction") {
    it("Array::destroy deallocates memory and nullifies the pointer") {
      sut = Array.empty();
      Array.destroy(&sut);

      asserteq(sut, null);
    }
  }
}

snow_main();
