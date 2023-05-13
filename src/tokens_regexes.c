#include <assert.h>
#include <err.h>
#include <regex.h>
#include <stdlib.h>

#include "scanner.h"
#include "tokens_regexes.h"

#define REGEX_COUNT SIZEOF_TOKEN - 2
// TOKEN_EOF and TOKEN_ERROR are respectively the
// before to last and last defined tokens in the enum
// and both have no regexes, thus, there is
// TOKEN_EOF different regexes == TOKEN_ERROR-1
regex_t regexes[REGEX_COUNT];

Token matchRegexes(const char source[]) {
  Token token;
  regmatch_t pmatch[1];
  token.lenght = 0;
  for (TokenType type = 0; type < REGEX_COUNT; type++) {
    regex_t *reg = &regexes[type];
    if (!regexec(reg, source, 1, pmatch, 0)) {
      if (pmatch->rm_so != 0) {
        continue;
      }
      int lenght = pmatch->rm_eo - pmatch->rm_so;
      if (lenght > token.lenght || token.type == TOKEN_IDENTIFIER) {
        token.lenght = lenght;
        token.type = type;
      }
    }
  }

  if (token.lenght == 0) {
    token.type = TOKEN_ERROR;
  }

  return token;
}

static void build_regex(TokenType tokentype, const char regex[]) {
  const int regex_flags = REG_NEWLINE | REG_EXTENDED;
  if (regcomp(&regexes[tokentype], regex, regex_flags)) {
    errx(EXIT_FAILURE, "Failed to compile regex \"%s\" for TokenType(%i)",
         regex, tokentype);
  }
}

void initRegexes() {
  // TODO: find a way to add ^ before each regex to not match
  // future tokens
#define R(r)                                                                   \
  build_regex(type, r);                                                        \
  break;

  for (TokenType type = 0; type < REGEX_COUNT; type++) {
    switch (type) {
    // clang-format off
    case TOKEN_LEFT_PAREN: R("\\(")
    case TOKEN_RIGHT_PAREN: R("\\)")
    case TOKEN_LEFT_BRACE: R("\\{")
    case TOKEN_RIGHT_BRACE: R("\\}")
    case TOKEN_COMMA: R(",")
    case TOKEN_DOT: R("\\.")
    case TOKEN_MINUS: R("-")
    case TOKEN_PLUS: R("\\+")
    case TOKEN_SEMICOLON: R(";")
    case TOKEN_SLASH: R("/")
    case TOKEN_STAR: R("\\*")
    case TOKEN_BANG: R("!")
    case TOKEN_BANG_EQUAL: R("!=")
    case TOKEN_EQUAL: R("=")
    case TOKEN_EQUAL_EQUAL: R("==")
    case TOKEN_GREATER: R(">")
    case TOKEN_GREATER_EQUAL: R(">=")
    case TOKEN_LESS: R("<")
    case TOKEN_LESS_EQUAL: R("<=")
    case TOKEN_IDENTIFIER: R("[_a-zA-Z][_a-zA-Z0-9]*")
    case TOKEN_STRING: R("\"[^\"]*\"")
    case TOKEN_NUMBER: R("[0-9]+(\\.[0-9]+)?")
    case TOKEN_AND: R("and")
    case TOKEN_CLASS: R("class")
    case TOKEN_ELSE: R("else")
    case TOKEN_FALSE: R("false")
    case TOKEN_FOR: R("for")
    case TOKEN_FUN: R("fun")
    case TOKEN_IF: R("if")
    case TOKEN_NIL: R("nil")
    case TOKEN_OR: R("or")
    case TOKEN_PRINT: R("print")
    case TOKEN_RETURN: R("return([; ])")
    case TOKEN_SUPER: R("super")
    case TOKEN_THIS: R("this")
    case TOKEN_TRUE: R("true")
    case TOKEN_VAR: R("var")
    case TOKEN_WHILE: R("while")
    // clang-format on
    // TOKEN_ERROR and TOKEN_EOF have no regex
    case TOKEN_EOF:
    case TOKEN_ERROR:
      break;
    }
  }

#undef R
}
