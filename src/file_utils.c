#include <err.h>
#include <stdio.h>
#include <stdlib.h>

char *readFile(const char *path) {
  FILE *file = fopen(path, "rb");

  if (file == NULL)
    errx(EXIT_FAILURE, "Could not open file \"%s\".\n", path);

  fseek(file, 0, SEEK_END);
  size_t fileSize = ftell(file);
  rewind(file);

  char *buffer = (char *)malloc(fileSize + 1);

  if (buffer == NULL)
    errx(EXIT_FAILURE, "Not enough memory to read \"%s\".\n", path);

  size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);

  if (bytesRead < fileSize)
    errx(EXIT_FAILURE, "Could not read file \"%s\".\n", path);

  buffer[bytesRead] = '\0';

  fclose(file);
  return buffer;
}
