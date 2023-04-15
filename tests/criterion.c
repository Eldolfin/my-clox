#include <criterion/alloc.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>

char *cr_strdup(const char *str) {
  char *ptr = cr_malloc(strlen(str) + 1);
  if (ptr)
    strcpy(ptr, str);
  return ptr;
}

// TODO: uncomment this when ready
/* #include "end_to_end_tests.c" */
#include "scanner_tests.c"
