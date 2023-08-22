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
