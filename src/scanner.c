#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"
#include "tokens_regexes.h"

typedef struct {
  const char *start;
  const char *current;
  Position position;
} Scanner;

Scanner scanner;

void initScanner(const char source[]) {
  scanner.start = source;
  scanner.current = source;
  scanner.position.line = 1;
  scanner.position.column = 1;
  initRegexes();
}

static bool isAtEnd() { return *scanner.current == '\0'; }

static Token makeToken(TokenType type) {
  Token token;
  token.type = type;
  token.start = scanner.start;
  token.lenght = (int)(scanner.current - scanner.start);
  token.position = scanner.position;
  return token;
}

static void advance() {
  if (*scanner.current == '\n') {
    scanner.position.line++;
    scanner.position.column = 0;
  }
  scanner.current++;
}

static void advance_n(int n) {
  for (int i = 0; i < n; i++) {
    advance();
  }
}

static Token errorToken(const char message[]) {
  Token token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.lenght = (int)strlen(message);
  token.position = scanner.position;
  advance();
  return token;
}

static void skipWhitespace() {
  while (isspace(*scanner.current)) {
    advance();
  }
}

Token scanToken() {
  skipWhitespace();
  scanner.start = scanner.current;
  if (isAtEnd())
    return makeToken(TOKEN_EOF);

  Token matched = matchRegexes(scanner.current);

  if (matched.type == TOKEN_ERROR) {
    return errorToken("Unexpected character.");
  } else {
    advance_n(matched.lenght);
    return makeToken(matched.type);
  }
}
