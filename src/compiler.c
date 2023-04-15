#include <stdio.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"

void compile(const char source[]) {
  initScanner(source);

  int line = -1;
  while (true) {
    Token token = scanToken();
    if (token.position.line != line) {
      printf("%4d ", token.position.line);
      line = token.position.line;
    } else {
      printf("   | ");
    }
    printf("%2d '%.*s'\n", token.type, token.lenght, token.start);
    if (token.type == TOKEN_EOF)
      break;
  }
}
