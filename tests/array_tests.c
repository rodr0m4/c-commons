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

    subdesc("Array::of") {
      it("Creates the array with the given elements (does not copy them)") {
        // @Improvement
        // There's a lot of ceremony in declaring the count + primitive array
        // Maybe have some value type { .count, .raw } for defining arrays? 
        // (AKA fat pointer :P)
        int one = 1;
        int* primitive[] = { &one };

        array_t* sut = Array.of(1, primitive);

        asserteq(sut->count, 1);
        asserteq(sut->elements[0], &one);

        Array.destroy(&sut);
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
