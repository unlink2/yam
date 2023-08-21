#ifndef ERROR_H_
#define ERROR_H_

#include "libyam/macros.h"
#include <stddef.h>
#include <errno.h>
#include <stdio.h>

// Possible error types.
enum yam_error {
  YAM_OK = 0,
  YAM_ERR_INVAL_SINK,
  YAM_ERR_INT_CONVERSION,
  YAM_ERR_TOK_READ,
  YAM_ERR_EXPR_SYNTAX,
  YAM_ERRNO
};

#define yam_errno()                                                            \
  {                                                                            \
    yam_error("%s\n", strerror(errno));                                        \
    yam_err_set(YAM_ERRNO);                                                    \
  }

#define yam_err_fset(err, ...)                                                 \
  {                                                                            \
    yam_err_set((err));                                                        \
    yam_error(__VA_ARGS__);                                                    \
  }

// sets err to a value
// to indicate a failure in a call
void yam_err_set(enum yam_error err);

void yam_err_print(FILE *f);

// return error code
enum yam_error yam_err(void);

#endif
