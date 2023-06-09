#include <stdint.h>
#include <stdio.h>

#include "chunk.h"
#include "debug.h"
#include "object.h"
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
  printValue(chunk->constants.values[constant]);
  printf("'\n");
  return offset + 2;
}

static int invokeInstruction(const char *name, Chunk *chunk, int offset) {
  uint8_t constant = chunk->code[offset + 1];
  uint8_t argCount = chunk->code[offset + 2];
  printf("%-16s (%d args) %4d '", name, argCount, constant);
  printValue(chunk->constants.values[constant]);
  printf("'\n");
  return offset + 3;
}

static int simpleInstruction(const char *name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

static int byteInstruction(const char *name, Chunk *chunk, int offset) {
  uint8_t slot = chunk->code[offset + 1];
  printf("%-16s %4d\n", name, slot);
  return offset + 2;
}

static int jumpInstruction(const char name[], int sign, Chunk *chunk,
                           int offset) {
  uint16_t jump =
      (uint16_t)(chunk->code[offset + 1] << 8) | chunk->code[offset + 2];

  printf("%16s %4d -> %d\n", name, offset, offset + 3 + sign * jump);
  return offset + 3;
}

int disassembleInstruction(Chunk *chunk, int offset) {
#define CASE_SIMPLE_INTRUCTION(instruction)                                    \
  case instruction:                                                            \
    return simpleInstruction(#instruction, offset)
#define CASE_CONSTANT_INSTRUCTION(instruction)                                 \
  case instruction:                                                            \
    return constantInstruction(#instruction, chunk, offset);
#define CASE_BYTE_INSTRUCTION(instruction)                                     \
  case instruction:                                                            \
    return byteInstruction(#instruction, chunk, offset);

  printf("%04d ", offset);
  if (offset > 0 &&
      chunk->positions[offset].line == chunk->positions[offset - 1].line) {
    printf("   | ");
  } else {
    printf("%4d ", chunk->positions[offset].line);
  }

  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
    CASE_CONSTANT_INSTRUCTION(OP_CONSTANT);
    CASE_CONSTANT_INSTRUCTION(OP_DEFINE_GLOBAL);
    CASE_CONSTANT_INSTRUCTION(OP_GET_GLOBAL);
    CASE_CONSTANT_INSTRUCTION(OP_SET_GLOBAL);
    CASE_CONSTANT_INSTRUCTION(OP_CLASS);
    CASE_CONSTANT_INSTRUCTION(OP_GET_PROPERTY);
    CASE_CONSTANT_INSTRUCTION(OP_SET_PROPERTY);
    CASE_CONSTANT_INSTRUCTION(OP_METHOD);
    CASE_CONSTANT_INSTRUCTION(OP_GET_SUPER);
    CASE_BYTE_INSTRUCTION(OP_GET_UPVALUE);
    CASE_BYTE_INSTRUCTION(OP_SET_UPVALUE);
    CASE_BYTE_INSTRUCTION(OP_GET_LOCAL);
    CASE_BYTE_INSTRUCTION(OP_SET_LOCAL);
    CASE_BYTE_INSTRUCTION(OP_CALL);
    CASE_SIMPLE_INTRUCTION(OP_NIL);
    CASE_SIMPLE_INTRUCTION(OP_TRUE);
    CASE_SIMPLE_INTRUCTION(OP_FALSE);
    CASE_SIMPLE_INTRUCTION(OP_POP);
    CASE_SIMPLE_INTRUCTION(OP_EQUAL);
    CASE_SIMPLE_INTRUCTION(OP_GREATER);
    CASE_SIMPLE_INTRUCTION(OP_LESS);
    CASE_SIMPLE_INTRUCTION(OP_ADD);
    CASE_SIMPLE_INTRUCTION(OP_SUBTRACT);
    CASE_SIMPLE_INTRUCTION(OP_MULTIPLY);
    CASE_SIMPLE_INTRUCTION(OP_DIVIDE);
    CASE_SIMPLE_INTRUCTION(OP_NOT);
    CASE_SIMPLE_INTRUCTION(OP_NEGATE);
    CASE_SIMPLE_INTRUCTION(OP_PRINT);
    CASE_SIMPLE_INTRUCTION(OP_RETURN);
    CASE_SIMPLE_INTRUCTION(OP_CLOSE_UPVALUE);
    CASE_SIMPLE_INTRUCTION(OP_INHERIT);
  case OP_LOOP:
    return jumpInstruction("OP_LOOP", -1, chunk, offset);
  case OP_JUMP:
    return jumpInstruction("OP_JUMP", 1, chunk, offset);
  case OP_JUMP_IF_FALSE:
    return jumpInstruction("OP_JUMP_IF_FALSE", 1, chunk, offset);
  case OP_INVOKE:
    return invokeInstruction("OP_INVOKE", chunk, offset);
  case OP_SUPER_INVOKE:
    return invokeInstruction("OP_SUPER_INVOKE", chunk, offset);
  case OP_CLOSURE: {
    offset++;
    uint8_t constant = chunk->code[offset++];
    printf("%-16s %4d ", "OP_CLOSURE", constant);
    printValue(chunk->constants.values[constant]);
    printf("\n");

    ObjFunction *function = AS_FUNCTION(chunk->constants.values[constant]);
    for (int j = 0; j < function->upvalueCount; j++) {
      int isLocal = chunk->code[offset++];
      int index = chunk->code[offset++];
      printf("%04d      |                     %s %d\n", offset - 2,
             isLocal ? "local" : "upvalue", index);
    }
    return offset;
  }

    /* default: */
    /*   printf("Unknown opcode %d\n", instruction); */
    /*   return offset + 1; */
  }

  return offset + 1;

#undef CASE_SIMPLE_INTRUCTION
#undef CASE_CONSTANT_INSTRUCTION
#undef CASE_BYTE_INSTRUCTION
}
