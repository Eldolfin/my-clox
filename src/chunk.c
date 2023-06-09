#include <stdlib.h>

#include "chunk.h"
#include "memory.h"
#include "scanner.h"
#include "value.h"
#include "vm.h"

void initChunk(Chunk *chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->positions = NULL;
  initValueArray(&chunk->constants);
}

void freeChunk(Chunk *chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->positions, chunk->capacity);
  freeValueArray(&chunk->constants);
  initChunk(chunk);
}
void writeChunk(Chunk *chunk, uint8_t byte, Position position) {
  if (chunk->capacity < chunk->count + 1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code =
        GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    chunk->positions =
        GROW_ARRAY(Position, chunk->positions, oldCapacity, chunk->capacity);
  }

  chunk->code[chunk->count] = byte;
  chunk->positions[chunk->count] = position;
  chunk->count++;
}

int addConstant(Chunk *chunk, Value value) {
  push(value);
  writeValueArray(&chunk->constants, value);
  pop();
  return chunk->constants.count - 1;
}
