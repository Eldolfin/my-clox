#pragma once
#include "object.h"
#include "vm.h"

ObjFunction *compile(const char source[], const char filename[],
                     bool repl_mode);
