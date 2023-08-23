#include "libyam/data.h"
#include "libyam/error.h"
#include "libyam/log.h"
#include <string.h>

enum yam_endianess yam_endianess_from(const char *expr, size_t len) {
  if (strncmp("little", expr, MAX(strlen("little"), len)) == 0) {
    return YAM_ENDIANESS_LITTLE;
  } else if (strncmp("big", expr, MAX(strlen("big"), len)) == 0) {
    return YAM_ENDIANESS_BIG;
  }

  yam_err_fset(YAM_ERR_EXPR_SYNTAX, "Unable to convert '%.*s' to endianess\n",
               (int)len, expr);
  return -1;
}

enum yam_int_fmt yam_int_fmt_from(const char *expr, size_t len) {
  if (strncmp("x", expr, MAX(strlen("x"), len)) == 0) {
    return YAM_FMT_HEX;
  } else if (strncmp("o", expr, MAX(strlen("o"), len)) == 0) {
    return YAM_FMT_OCT;
  } else if (strncmp("d", expr, MAX(strlen("d"), len)) == 0) {
    return YAM_FMT_DEC;
  } else if (strncmp("b", expr, MAX(strlen("b"), len)) == 0) {
    return YAM_FMT_BIN;
  }

  yam_err_fset(YAM_ERR_EXPR_SYNTAX, "Unable to convert '%.*s' to int fmt\n",
               (int)len, expr);
  return -1;
}

enum yam_int_sign yam_int_sign_from(const char *expr, size_t len) {
  if (strncmp("u", expr, MAX(strlen("u"), len)) == 0) {
    return YAM_FMT_UNSIGNED;
  } else if (strncmp("s", expr, MAX(strlen("s"), len)) == 0) {
    return YAM_FMT_SIGNED;
  }

  yam_err_fset(YAM_ERR_EXPR_SYNTAX, "Unable to convert '%.*s' to int fmt\n",
               (int)len, expr);
  return -1;
}
