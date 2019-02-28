#include <stdlib.h>

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

    subdesc("Array::from") {
      it("Creates the array copying the given elements") {
        int* copy_int(int* payload) {
          if (!payload) return null;

          int* new_int = (int*) malloc(sizeof(int));

          if (!new_int) return null;

          *new_int = *payload;

          return new_int;
        }

        int one = 1;
        int two = 2;

        int* primitive[] = { &one, &two };

        array_t* sut = Array.from(copy_int, 2, primitive);

        asserteq(sut->count, 2);
        assertneq(sut->elements[0], &one, "Elements should have been copied!");
        asserteq(*((int*) sut->elements[0]), one, "Elements should have been copied!");
        assertneq(sut->elements[1], &two, "Elements should have been copied!");
        asserteq(*((int*) sut->elements[1]), two, "Elements should have been copied!");

        // Cleanup because we do not want to leak
        Array.destroy_with_free(&sut);
      }
    }

    subdesc("Array::with_capacity") {
      it("Creates the array with the given capacity (should it be fixed?)") {
        array_t* sut = Array.with_capacity(10);

        asserteq(sut->capacity, 10);
        
        Array.destroy(&sut);
      } 
    }
  }

  subdesc("'Instance methods'") {
    array_t* sut;
    before_each() {
      sut = Array.empty();
    }

    after_each() {
      Array.destroy(&sut);
    }

    subdesc("Array::is_empty") {
      it("Returns true when the array does not have elements") {
        assert(Array.is_empty(sut));
      }

      it("Returns false otherwise") {
        int one = 1;
        sut->elements[0] = &one;
        sut->count += 1;

        assert(!Array.is_empty(sut));
      }

      it("Of course, as it can only check count, you can lie to it and not add anything") {
        sut->count += 34;

        assert(!Array.is_empty(sut));
      }
    }

    subdesc("Array::add") {
      it("Adds the element to the tail") {
        array_t* sut = Array.empty();

        int one = 1;

        asserteq(sut->count, 0);

        Array.add(sut, &one);

        asserteq(sut->count, 1);
        asserteq(sut->elements[0], &one);

        Array.destroy(&sut);
      }

      it("It ensures enlarging the capacity of the buffer to hold every element, cleaning the old one each time") {
        array_t* sut = Array.with_capacity(1);

        int one = 1;
        int two = 2;

        Array.add(sut, &one);

        void* old_buffer = *(sut->elements);

        asserteq(sut->count, 1);
        asserteq(sut->capacity, 1);

        // When an element is added, capacity doubles.
        Array.add(sut, &two);
//
        // assertneq(*(sut->elements), old_buffer);
        asserteq(sut->count, 2);


        Array.destroy(&sut);
      }

      it("In the special case that the capacity is 0, it will set it to the default initial capacity (perhaps the array should be lazy initialized, like in jvm?)") {
      
      }
    }
  }

  subdesc("Array destruction") {
    it("Array::destroy deallocates memory and nullifies the pointer") {
      array_t* sut = Array.empty();
      Array.destroy(&sut);

      asserteq(sut, null);
    }

    it("Array::destroy_with_free destroys and calls free on each of the elements") {
      int* something = (int*) malloc(sizeof(int));
      int* primitive[] = { something };

      array_t* sut = Array.of(1, primitive);
      
      Array.destroy_with_free(&sut);

      asserteq(sut, null); // This does not assert nothing really. You should check valgrind output (0 bytes lost 😊)
    }

    it("Array::destroy_with_destructor calls the custom destructor") {
      // Find a better way to mock?
      boolean was_called = false;
      void effectful_destructor(void* x) {
        was_called = true;
      }

      int one = 1;
      int* primitive[] = { &one };

      array_t* sut = Array.of(1, primitive);

      Array.destroy_with_destructor(&sut, effectful_destructor);

      assert(was_called);
    }
  }
}

snow_main();
