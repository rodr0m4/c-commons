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

    subdesc("adding/removing operations") {
      it("elements can be added at the tail") {
        int zero = 0; // This should be mallocd

        list_add(list, &zero);

        asserteq(list->current, 1);
        asserteq(list->members[0], &zero);
      }
    }
  }
}

snow_main();
