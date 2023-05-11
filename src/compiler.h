#pragma once
#include "vm.h"

bool compile(const char source[], Chunk *chunk, const char filename[],
             bool repl_mode);
