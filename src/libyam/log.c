#include "libyam/log.h"

#include <stdint.h>

enum yam_loglevel YAM_LOG_LEVEL;
FILE *YAM_LOG_TARGET;

void yam_log_init(enum yam_loglevel level) {
  YAM_LOG_LEVEL = level;
  YAM_LOG_TARGET = stderr;
}
