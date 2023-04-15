#pragma once

#include "scanner.h"

void initRegexes();
Token matchRegexes(const char source[]);
