#include <stdlib.h>

#include "../src/array.h"
#include "../src/kernel.h"
#include "snow.h"

// Helper functions (no nested functions nor closures in ANSI C)
int *copy_int(int *payload) {
  if (!payload) return null;

  int *new_int = (int *)malloc(sizeof(int));

  if (!new_int) return null;

  *new_int = *payload;

  return new_int;
}

// Find a better way to mock?
bool_t was_called = false;
void effectful_destructor(void *x) { was_called = true; }

int one = 1;
int two = 2;

describe(arrays) {
  array_t *sut;

  subdesc("Array creation") {
    before_each() { sut = array_empty(); }

    after_each() { array_destroy(&sut); }

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
        int *primitive[] = {&one};

        array_t *sut = array_of(1, primitive);

        asserteq(sut->count, 1);
        asserteq(sut->elements[0], &one);

        array_destroy(&sut);
      }
    }

    subdesc("Array::from") {
      it("Creates the array copying the given elements") {
        int *primitive[] = {&one, &two};

        array_t *sut = array_from(copy_int, 2, primitive);

        asserteq(sut->count, 2);
        assertneq(sut->elements[0], &one, "Elements should have been copied!");
        asserteq(*((int *)sut->elements[0]), one,
                 "Elements should have been copied!");
        assertneq(sut->elements[1], &two, "Elements should have been copied!");
        asserteq(*((int *)sut->elements[1]), two,
                 "Elements should have been copied!");

        // Cleanup because we do not want to leak
        array_destroy_with_free(&sut);
      }
    }

    subdesc("Array::with_capacity") {
      it("Creates the array with the given capacity (should it be fixed?)") {
        array_t *sut = array_with_capacity(10);

        asserteq(sut->capacity, 10);

        array_destroy(&sut);
      }
    }
  }

  subdesc("'Instance methods'") {
    array_t *sut;
    before_each() { sut = array_empty(); }

    after_each() { array_destroy(&sut); }

    subdesc("Array::shallow_copy") {
      it("should copy the whole array, including nulls at the end of the "
         "buffer") {
        array_add(sut, &one);

        array_t *shallow_copy = array_shallow_copy(sut);

        asserteq(shallow_copy->capacity, sut->capacity);
        asserteq(shallow_copy->count, sut->count);

        for (int i = 0; i < shallow_copy->count; i += 1) {
          asserteq(shallow_copy->elements[i], sut->elements[i]);
        }

        array_destroy(&shallow_copy);
      }
    }

    subdesc("Array::deep_copy") {
      it("should copy the whole array, copying the values with the given copy "
         "function") {
        array_add(sut, &one);

        array_t *deep_copy = array_deep_copy(sut, copy_int);

        asserteq(deep_copy->capacity, sut->capacity);
        asserteq(deep_copy->count, sut->count);

        for (int i = 0; i < deep_copy->count; i += 1) {
          if (!sut->elements[i]) {
            // Element should not be in the deep copy either
            assert(!deep_copy->elements[i]);
          } else {
            // The pointers are different, but the values are the same (because
            // our copier function mallocs new memory)
            assertneq(deep_copy->elements[i], sut->elements[i]);
            asserteq(*((int *)deep_copy->elements[i]),
                     *((int *)sut->elements[i]));
          }
        }

        array_destroy_with_free(&deep_copy);
      }
    }

    subdesc("Array::is_empty") {
      it("Returns true when the array does not have elements") {
        assert(array_is_empty(sut));
      }

      it("Returns false otherwise") {
        sut->elements[0] = &one;
        sut->count += 1;

        assert(!array_is_empty(sut));
      }

      it("Of course, as it can only check count, you can lie to it and not add "
         "anything") {
        sut->count += 34;

        assert(!array_is_empty(sut));
      }
    }

    subdesc("Array:insert") {
      it("should add the element to the given index") {
        array_t *sut = array_empty();

        array_insert(sut, &one, 0);

        asserteq(sut->elements[0], &one);

        array_destroy(&sut);
      }

      it("should add the element to the desired index in the middle of the "
         "array, and shifts right the tail from there") {
        array_t *sut = array_empty();

        int three = 3;

        array_add(sut, &one);
        array_add(sut, &two);

        array_insert(sut, &three, 0);

        asserteq(sut->elements[0], &three);
        asserteq(*((int *)sut->elements[1]), one);
        asserteq(sut->elements[2], &two);

        array_destroy(&sut);
      }
    }

    subdesc("Array::add") {
      it("Adds the element to the tail") {
        array_t *sut = array_empty();
        array_add(sut, &one);

        asserteq(sut->count, 1);
        asserteq(sut->elements[0], &one);

        array_destroy(&sut);
      }

      it("ensures enlarging the capacity of the buffer to hold every element, "
         "cleaning the old one each time") {
        array_t *sut = array_with_capacity(1);
        array_add(sut, &one);

        // void* old_buffer = *(sut->elements); // See below

        asserteq(sut->count, 1);
        asserteq(sut->capacity, 1);

        // When an element is added, capacity doubles.
        array_add(sut, &two);

        // assertneq(*(sut->elements), old_buffer); // This leaks, maybe a snow
        // bug?
        asserteq(sut->count, 2);

        array_destroy(&sut);
      }

      it("In the special case that the capacity is 0, it will set it to the "
         "default initial capacity (perhaps the array should be lazy "
         "initialized, like in jvm?)") {
        array_t *sut = array_with_capacity(0);

        // int one = 1;

        array_add(sut, &one);

        asserteq(sut->count, 1);
        asserteq(sut->capacity, 32);

        array_destroy(&sut);
      }
    }

    subdesc("Array::get") {
      it("should fetch the element from the array and cast it, without "
         "dereferencing") {
        array_add(sut, &one);
        asserteq(*array_get(sut, int, 0), one);
      }

      it("should fetch the element from the array from the tail on negative "
         "indexes, a-la-ruby") {
        array_add(sut, &one);
        array_add(sut, &two);

        asserteq(*array_get(sut, int, -1), two);  // -1 is last
        asserteq(*array_get(sut, int, -2), one);
      }

      subdesc("Bounds checking") {
        it("should fail on empty arrays") {
          asserteq(array_get(sut, int, -1), null);
          asserteq(array_get(sut, int, 0), null);
        }

        it("should bound check positive indexes") {
          array_add(sut, &one);
          asserteq(array_get(sut, int, 15), null);
        }

        it("should bound check negative indexes") {
          array_add(sut, &one);
          asserteq(array_get(sut, int, -2), null);
        }
      }
    }

    subdesc("Array::foreach") {
      it("should execute the given piece of code") {
        array_add(sut, &one);
        array_add(sut, &two);

        int expected = 1;

        foreach (int, sut, {
          asserteq(*it, expected);
          expected += 1;
        })
      }

      it("should not execute the code on empty arrays") {
        bool_t should_be_true = true;

        foreach (int, sut, { should_be_true = false; })

          assert(
              should_be_true,
              "The block passed to the foreach was executed on empty array!");

        array_add(sut, &one);

        foreach (int, sut, { should_be_true = false; })

          assert(!should_be_true,
                 "The block passed to the foreach was not executed on non "
                 "empty array!");
      }
    }
  }

  subdesc("Array destruction") {
    it("Array::destroy deallocates memory and nullifies the pointer") {
      array_t *sut = array_empty();
      array_destroy(&sut);

      asserteq(sut, null);
    }

    it("Array::destroy_with_free destroys and calls free on each of the "
       "elements") {
      int *something = (int *)malloc(sizeof(int));
      int *primitive[] = {something};

      array_t *sut = array_of(1, primitive);

      array_destroy_with_free(&sut);

      asserteq(sut, null);  // This does not assert nothing really. You should
                            // check valgrind output (0 bytes lost 😊)
    }

    it("Array::destroy_with_destructor calls the custom destructor") {
      int *primitive[] = {&one};

      array_t *sut = array_of(1, primitive);

      array_destroy_with_destructor(&sut, effectful_destructor);

      assert(was_called);
    }
  }
}

snow_main();
