#include "libyam/expr.h"
#include "libyam/error.h"
#include "libyam/log.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

const char *yam_tok_next(const char *input, char term, size_t *parsed_len) {
  if (*input == term) {
    input++;
  }

  size_t len = 0;
  const char *tok_start = input;
  while (*input != '\0' && *input != term) {
    input++;
    len++;
  }

  if (len == 0) {
    return NULL;
  }

  *parsed_len = len;
  return tok_start;
}

const char *yam_tok_kv(const char *input, size_t len, const char *key,
                       size_t *parsed_len) {
  size_t key_len = strlen(key);
  if (strncmp(key, input, key_len) != 0) {
    return NULL;
  }

  const char *value = input + key_len; 
  *parsed_len = len - key_len;
  return value;
}

const char *yam_tok_trim(const char *tok, size_t *len) {
  while (*len > 0 && *tok != '\0' && isspace(*tok)) {
    *len = *len - 1;
    tok++;
  }

  if (*len == 0) {
    return NULL;
  }

  return tok;
}

int yam_tok_to_int(const char *tok, size_t len) {
  char *end = NULL;
  int result = (int)strtol(tok, &end, 0);

  if (end != tok + len) {
    yam_err_fset(YAM_ERR_INT_CONVERSION, "Unable to convert '%.*s' to int!\n",
                 (int)len, tok);
  }

  return result;
}
