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
  bool repl_mode;
} Scanner;

Scanner scanner;

void initScanner(const char source[], const char filename[], bool repl_mode) {
  scanner.start = source;
  scanner.current = source;
  scanner.position.line = 1;
  scanner.position.column = 1;
  scanner.position.filename = filename;
  scanner.repl_mode = repl_mode;
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

static char peek() { return *scanner.current; }

static char peekNext() {
  if (isAtEnd())
    return '\0';

  return scanner.current[1];
}

static void skipWhitespace() {
  while (true) {
    if (isspace(*scanner.current)) {
      advance();
    } else if (peek() == '/' && peekNext() == '/') {
      while (peek() != '\n' && !isAtEnd())
        advance();
    } else {
      break;
    }
  }
}

Token scanToken() {
  skipWhitespace();
  scanner.start = scanner.current;
  if (isAtEnd()) {
    if (scanner.repl_mode) {
      scanner.repl_mode = false;
      return makeToken(TOKEN_SEMICOLON);
    } else {
      return makeToken(TOKEN_EOF);
    }
  }

  Token matched = matchRegexes(scanner.current);

  if (matched.type == TOKEN_ERROR) {
    return errorToken("Unexpected character.");
  } else {
    advance_n(matched.lenght);
    return makeToken(matched.type);
  }
}
