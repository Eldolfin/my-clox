#include <time.h>

#include "value.h"
#include "vm.h"

Value clockNative(int argCount, Value *args) {
  return NUMBER_VAL((double)clock() / CLOCKS_PER_SEC);
}

void defineNatives() { defineNative("clock", clockNative); }
