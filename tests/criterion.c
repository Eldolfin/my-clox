#include <criterion/alloc.h>
#include <criterion/internal/assert.h>
#include <criterion/parameterized.h>
#include <dirent.h>
#include <err.h>
#include <ftw.h>
#include <glob.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *read_file(char filename[]);
char **list_files(size_t *n);
char *cr_strdup(const char *str);
char *extract_expected(char *source);

ParameterizedTestParameters(end_to_end_suite, book_test_suite) {
  size_t nb_params;
  char **params = list_files(&nb_params);

  return cr_make_param_array(char *, params, nb_params, NULL);
}

ParameterizedTest(char *program[], end_to_end_suite, book_test_suite) {
  char *source = read_file(*program);
  char *expected = extract_expected(source);
  // TODO: implement run() function
  /* cr_assert_str_eq(run(source), expected); */
  free(source);
  free(expected);
}

char *extract_expected(char *source) {
  char *output_strings = malloc(strlen(source));
  char *src_ptr = source;
  char *dest_ptr = output_strings;

  while (1) {
    char *expect_ptr = strstr(src_ptr, "// expect: ");
    if (expect_ptr == NULL) {
      break;
    }

    src_ptr = expect_ptr + strlen("// expect: ");

    char *line_end_ptr = strstr(src_ptr, "\n");

    if (line_end_ptr == NULL) {
      line_end_ptr = source + strlen(source);
    }

    size_t string_length = line_end_ptr - src_ptr;
    strncpy(dest_ptr, src_ptr, string_length);
    dest_ptr[string_length] = '\n';
    dest_ptr += string_length + 1;

    src_ptr = line_end_ptr;
  }
  *dest_ptr = '\0';

  return output_strings;
}

char *read_file(char filename[]) {
  FILE *fp;
  long file_size;
  char *buffer;
  fp = fopen(filename, "rb");
  if (fp == NULL) {
    printf("Failed to open file %s\n", filename);
    exit(1);
  }
  fseek(fp, 0, SEEK_END);
  file_size = ftell(fp);
  rewind(fp);
  buffer = (char *)malloc(file_size + 1);
  fread(buffer, file_size, 1, fp);
  buffer[file_size] = '\0';
  fclose(fp);
  return buffer;
}

// searches for .lox files
char **list_files(size_t *n) {
  glob_t results;
  char *pattern = "*.txt";
  int ret = glob("./*/*.lox", GLOB_TILDE | GLOB_BRACE, NULL, &results);
  if (ret != 0) {
    errx(EXIT_FAILURE, "Error: glob() failed with return code %d\n", ret);
  }
  *n = results.gl_pathc;
  char **files = cr_malloc(sizeof(char *) * (*n));
  for (size_t i = 0; i < *n; i++) {
    files[i] = cr_strdup(results.gl_pathv[i]);
  }
  return files;
}

char *cr_strdup(const char *str) {
  char *ptr = cr_malloc(strlen(str) + 1);
  if (ptr)
    strcpy(ptr, str);
  return ptr;
}