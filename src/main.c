#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "file_utils.h"
#include "vm.h"

static void repl() {
  char line[1024];
  while (true) {
    printf("> ");

    if (!fgets(line, sizeof(line), stdin)) {
      printf("\nByeBye 👋\n");
      break;
    }

    interpret(line, "repl", stdout);
  }
}

static void runFile(const char path[]) {
  char *source = readFile(path);
  InterpretResult result = interpret(source, path, stdout);
  free(source);

  switch (result) {
  case INTERPRET_COMPILE_ERROR:
    exit(65);
    break;
  case INTERPRET_RUNTIME_ERROR:
    exit(70);
    break;
  default:
    break;
  }
}

int main(int argc, char *argv[]) {
  initVM();

  if (argc == 1) {
    repl();
  } else if (argc == 2) {
    runFile(argv[1]);
  } else {
    errx(EXIT_FAILURE, "Usage: %s [path]\n", argv[0]);
  }

  freeVM();
  return EXIT_SUCCESS;
}
