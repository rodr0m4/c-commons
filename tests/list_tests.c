#include "snow.h"
#include "../src/list.h"

describe(list) {
  subdesc(constructors) {
    list_t* list;
    
    before_each() {
      list = new_list();
    }

    after_each() {
      destroy_list(&list);      
    }

    it("can be created with new_list()") {
      asserteq(list->size, 32);
    }

    it("is created empty") {
      assert(list_is_empty(list));
    }

    it("is 0 initialized") {
      for (int i = 0; i < list->size; i += 1) {
        asserteq(list->members[i], NULL);
      }
    }

    subdesc("#add operations") {
      it("elements can be added at the tail") {
        int zero = 0; // This should be mallocd

        list_add(list, &zero);

        asserteq(list->current, 1);
        asserteq(list->members[0], &zero);
      }

      it("When there is no space left, it will allocate new memory") {
        int zero = 0;
        int one = 1;

        list_t* lil_list = new_list_with_size(1);
        list_add(lil_list, &zero);
        list_add(lil_list, &one);

        asserteq(*(int*)(lil_list->members[0]), zero);

        asserteq(lil_list->size, 2);

        asserteq(*(int*)(lil_list->members[0]), zero);
        asserteq(*(int*)(lil_list->members[1]), one);

        destroy_list(&lil_list);
      }
    }
  }
}

snow_main();
