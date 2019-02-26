#include <stdio.h>
#include "list.h"

int main(int argc, char** argv) {
  list_t* list = new_list();
  
  int fst, snd, trd = 0;
  
  list_add(list, &fst);
  list_add(list, &snd);
  list_add(list, &trd);

  each(int, list) {
    printf("number is %d", it);
  }

  return 0;
}
