#ifndef EXPR_H_
#define EXPR_H_

#include <stddef.h>

#define YAM_TOK_STD_TERM ':'

// finds the next unescaped : and returns a ptr to it
const char *yam_tok_next(const char *input, char term, size_t *parsed_len);

// trims a token
const char *yam_tok_trim(const char *tok, size_t *len);

// converts a token to int
int yam_tok_to_int(const char *tok, size_t len);

#endif