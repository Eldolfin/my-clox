#include "scanner.h"
#include <criterion/logging.h>
#include <stddef.h>
#include <stdio.h>

// TODO: negative tests to make sure things like
// `andor!printreturnforclassifelsethistrue<<=>>=` returns errors

struct source_to_token_types_params {
  char *source;
  TokenType *types;
  size_t type_count;
};

char *token_name(TokenType type);

ParameterizedTestParameters(scanner_tests, test_token_types) {
  const size_t nb_params = 5;
  struct source_to_token_types_params *params =
      cr_calloc(sizeof(struct source_to_token_types_params), nb_params);

  params[0].source = cr_strdup(";;");
  params[0].type_count = 3;
  params[0].types = cr_calloc(sizeof(TokenType), params[0].type_count);
  params[0].types[0] = TOKEN_SEMICOLON;
  params[0].types[1] = TOKEN_SEMICOLON;
  params[0].types[2] = TOKEN_EOF;

  params[1].source = cr_strdup(
      "and or ! print return for class if else this "
      "true < <= > >= , . - + * / != == =; while var this super nil fun");
  params[1].type_count = 32;
  params[1].types = cr_calloc(sizeof(TokenType), params[1].type_count);
  params[1].types[0] = TOKEN_AND;
  params[1].types[1] = TOKEN_OR;
  params[1].types[2] = TOKEN_BANG;
  params[1].types[3] = TOKEN_PRINT;
  params[1].types[4] = TOKEN_RETURN;
  params[1].types[5] = TOKEN_FOR;
  params[1].types[6] = TOKEN_CLASS;
  params[1].types[7] = TOKEN_IF;
  params[1].types[8] = TOKEN_ELSE;
  params[1].types[9] = TOKEN_THIS;
  params[1].types[10] = TOKEN_TRUE;
  params[1].types[11] = TOKEN_LESS;
  params[1].types[12] = TOKEN_LESS_EQUAL;
  params[1].types[13] = TOKEN_GREATER;
  params[1].types[14] = TOKEN_GREATER_EQUAL;
  params[1].types[15] = TOKEN_COMMA;
  params[1].types[16] = TOKEN_DOT;
  params[1].types[17] = TOKEN_MINUS;
  params[1].types[18] = TOKEN_PLUS;
  params[1].types[19] = TOKEN_STAR;
  params[1].types[20] = TOKEN_SLASH;
  params[1].types[21] = TOKEN_BANG_EQUAL;
  params[1].types[22] = TOKEN_EQUAL_EQUAL;
  params[1].types[23] = TOKEN_EQUAL;
  params[1].types[24] = TOKEN_SEMICOLON;
  params[1].types[25] = TOKEN_WHILE;
  params[1].types[26] = TOKEN_VAR;
  params[1].types[27] = TOKEN_THIS;
  params[1].types[28] = TOKEN_SUPER;
  params[1].types[29] = TOKEN_NIL;
  params[1].types[30] = TOKEN_FUN;
  params[1].types[31] = TOKEN_EOF;

  params[2].source = cr_strdup(
      "\"This is a string :)\" ThisIs_aN_Ident1fier() {} 123.654 123");
  params[2].type_count = 7;
  params[2].types = cr_calloc(sizeof(TokenType), params[2].type_count);
  params[2].types[0] = TOKEN_STRING;
  params[2].types[1] = TOKEN_IDENTIFIER;
  params[2].types[2] = TOKEN_LEFT_PAREN;
  params[2].types[3] = TOKEN_RIGHT_PAREN;
  params[2].types[4] = TOKEN_LEFT_BRACE;
  params[2].types[5] = TOKEN_RIGHT_BRACE;
  params[2].types[6] = TOKEN_EOF;

  params[3].source = cr_strdup("\"\"\n"
                               "\"string\"\n");
  params[3].type_count = 3;
  params[3].types = cr_calloc(sizeof(TokenType), params[3].type_count);
  params[3].types[0] = TOKEN_STRING;
  params[3].types[1] = TOKEN_STRING;
  params[3].types[2] = TOKEN_EOF;

  params[4].source = cr_strdup("123\n"
                               "123.456 \n  "
                               ".456     \n"
                               "123. \n    ");
  params[4].type_count = 7;
  params[4].types = cr_calloc(sizeof(TokenType), params[4].type_count);
  params[4].types[0] = TOKEN_NUMBER;
  params[4].types[1] = TOKEN_NUMBER;
  params[4].types[2] = TOKEN_DOT;
  params[4].types[3] = TOKEN_NUMBER;
  params[4].types[4] = TOKEN_NUMBER;
  params[4].types[5] = TOKEN_DOT;
  params[4].types[6] = TOKEN_EOF;

  return cr_make_param_array(struct source_to_token_types_params, params,
                             nb_params, NULL);
}

ParameterizedTest(struct source_to_token_types_params *params, scanner_tests,
                  test_token_types) {
  initScanner(params->source, "");
  TokenType type = scanToken().type;
  size_t i = 0;
  do {
    cr_assert_eq(type, params->types[i],
                 "Expected %s, found %s (at position %i)",
                 token_name(params->types[i]), token_name(type), i);
    type = scanToken().type;
    i++;
  } while (type != TOKEN_EOF && i < params->type_count - 1);
  cr_assert_eq(i, params->type_count - 1, "Expected %i tokens, found %i",
               params->type_count, i + 1);
}

char *token_name(TokenType type) {
  switch (type) {
    // clang-format off
    case TOKEN_LEFT_PAREN: return "TOKEN_LEFT_PAREN";
    case TOKEN_RIGHT_PAREN: return "TOKEN_RIGHT_PAREN";
    case TOKEN_LEFT_BRACE: return "TOKEN_LEFT_BRACE";
    case TOKEN_RIGHT_BRACE: return "TOKEN_RIGHT_BRACE";
    case TOKEN_COMMA: return "TOKEN_COMMA";
    case TOKEN_DOT: return "TOKEN_DOT";
    case TOKEN_MINUS: return "TOKEN_MINUS";
    case TOKEN_PLUS: return "TOKEN_PLUS";
    case TOKEN_SEMICOLON: return "TOKEN_SEMICOLON";
    case TOKEN_SLASH: return "TOKEN_SLASH";
    case TOKEN_STAR: return "TOKEN_STAR";
    case TOKEN_BANG: return "TOKEN_BANG";
    case TOKEN_BANG_EQUAL: return "TOKEN_BANG_EQUAL";
    case TOKEN_EQUAL: return "TOKEN_EQUAL";
    case TOKEN_EQUAL_EQUAL: return "TOKEN_EQUAL_EQUAL";
    case TOKEN_GREATER: return "TOKEN_GREATER";
    case TOKEN_GREATER_EQUAL: return "TOKEN_GREATER_EQUAL";
    case TOKEN_LESS: return "TOKEN_LESS";
    case TOKEN_LESS_EQUAL: return "TOKEN_LESS_EQUAL";
    case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
    case TOKEN_STRING: return "TOKEN_STRING";
    case TOKEN_NUMBER: return "TOKEN_NUMBER";
    case TOKEN_AND: return "TOKEN_AND";
    case TOKEN_CLASS: return "TOKEN_CLASS";
    case TOKEN_ELSE: return "TOKEN_ELSE";
    case TOKEN_FALSE: return "TOKEN_FALSE";
    case TOKEN_FOR: return "TOKEN_FOR";
    case TOKEN_FUN: return "TOKEN_FUN";
    case TOKEN_IF: return "TOKEN_IF";
    case TOKEN_NIL: return "TOKEN_NIL";
    case TOKEN_OR: return "TOKEN_OR";
    case TOKEN_PRINT: return "TOKEN_PRINT";
    case TOKEN_RETURN: return "TOKEN_RETURN";
    case TOKEN_SUPER: return "TOKEN_SUPER";
    case TOKEN_THIS: return "TOKEN_THIS";
    case TOKEN_TRUE: return "TOKEN_TRUE";
    case TOKEN_VAR: return "TOKEN_VAR";
    case TOKEN_WHILE: return "TOKEN_WHILE";
    case TOKEN_EOF: return "TOKEN_EOF";
    case TOKEN_ERROR: return "TOKEN_ERROR";
    // clang-format on
  }
  return "DIDNT MATCH WTF??";
}
