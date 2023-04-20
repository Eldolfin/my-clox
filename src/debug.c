#include <stdio.h>

#include "chunk.h"
#include "debug.h"
#include "value.h"

void disassembleChunk(Chunk *chunk, const char *name) {
  printf("== %s ==\n", name);

  for (int offset = 0; offset < chunk->count;) {
    offset = disassembleInstruction(chunk, offset);
  }
}

static int constantInstruction(const char *name, Chunk *chunk, int offset) {
  uint8_t constant = chunk->code[offset + 1];
  printf("%-16s %4d '", name, constant);
  printValue(chunk->constants.values[constant], stdout);
  printf("'\n");
  return offset + 2;
}

static int simpleInstruction(const char *name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

int disassembleInstruction(Chunk *chunk, int offset) {
#define CASE_SIMPLE_INTRUCTION(instruction)                                    \
  case instruction:                                                            \
    return simpleInstruction(#instruction, offset)

  printf("%04d ", offset);
  if (offset > 0 &&
      chunk->positions[offset].line == chunk->positions[offset - 1].line) {
    printf("   | ");
  } else {
    printf("%4d ", chunk->positions[offset].line);
  }

  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
  case OP_CONSTANT:
    return constantInstruction("OP_CONSTANT", chunk, offset);
    CASE_SIMPLE_INTRUCTION(OP_NIL);
    CASE_SIMPLE_INTRUCTION(OP_TRUE);
    CASE_SIMPLE_INTRUCTION(OP_FALSE);
    CASE_SIMPLE_INTRUCTION(OP_EQUAL);
    CASE_SIMPLE_INTRUCTION(OP_GREATER);
    CASE_SIMPLE_INTRUCTION(OP_LESS);
    CASE_SIMPLE_INTRUCTION(OP_ADD);
    CASE_SIMPLE_INTRUCTION(OP_SUBTRACT);
    CASE_SIMPLE_INTRUCTION(OP_MULTIPLY);
    CASE_SIMPLE_INTRUCTION(OP_DIVIDE);
    CASE_SIMPLE_INTRUCTION(OP_NOT);
    CASE_SIMPLE_INTRUCTION(OP_NEGATE);
    CASE_SIMPLE_INTRUCTION(OP_RETURN);
  default:
    printf("Unknown opcode %d\n", instruction);
    return offset + 1;
  }

#undef CASE_SIMPLE_INTRUCTION
}
