#pragma once

#include <stddef.h>

#include "common.h"
#include "compiler.h"
#include "object.h"

#define ALLOCATE(type, count) (type *)reallocate(NULL, 0, sizeof(type) * count)

#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : capacity * 2)

#define FREE(type, pointer) reallocate(pointer, sizeof(type), 0)

#define GROW_ARRAY(type, pointer, oldCount, newCount)                          \
  (type *)reallocate(pointer, oldCount, sizeof(type) * newCount)

#define FREE_ARRAY(type, pointer, oldCount) reallocate(pointer, oldCount, 0)

void *reallocate(void *pointer, size_t oldSize, size_t newSize);
void markObject(Obj *object);
void markValue(Value value);
void collectGarbage();
void freeObjects();
