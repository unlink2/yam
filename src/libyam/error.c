#include <libyam/macros.h>
#include <libyam/error.h>
#include <stdlib.h>
#include <string.h>

_Thread_local enum yam_error YAM_ERR;

void yam_err_set(enum yam_error err) { YAM_ERR = err; }

enum yam_error yam_err(void) {
  enum yam_error latest = YAM_ERR;
  return latest;
}
