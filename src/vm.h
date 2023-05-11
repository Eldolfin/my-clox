#pragma once

#include "chunk.h"
#include "table.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
  Chunk *chunk;
  uint8_t *ip;
  Value stack[STACK_MAX];
  Value *stackTop;
  Table globals;
  Table strings;
  Obj *objects;
  bool repl_mode;
} VM;

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR,
} InterpretResult;

extern VM vm;

void initVM(bool repl_mode);
void freeVM();
InterpretResult interpret(const char source[], const char filename[],
                          FILE *outputStream);
void push(Value value);
Value pop();
