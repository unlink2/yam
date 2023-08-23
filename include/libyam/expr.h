#ifndef EXPR_H_
#define EXPR_H_

#include <stddef.h>
#include <stdint.h>

#define YAM_TOK_STD_TERM ':'

// finds the next unescaped : and returns a ptr to it
const char *yam_tok_next(const char *input, char term, size_t *parsed_len);

// checks if a sub token starts with a key, and if so reutrns the value after
// the key
const char *yam_tok_kv(const char *input, size_t len, const char *key,
                       size_t *parsed_len);

const char *yam_tok_kv_adv(const char **input, size_t *len, const char *key,
                           size_t *parsed_len);

// trims a token
const char *yam_tok_trim(const char *tok, size_t *len);

// converts a token to int
int64_t yam_tok_to_int(const char *tok, size_t len);
float yam_tok_to_float(const char *tok, size_t len);

#endif
