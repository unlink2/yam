#ifndef LOG_H_
#define LOG_H_

#include "assert.h"
#include <stdio.h>
#include <stdlib.h>

enum yam_loglevel {
  YAM_LOG_LEVEL_OFF,
  YAM_LOG_LEVEL_ERROR,
  YAM_LOG_LEVEL_WARN,
  YAM_LOG_LEVEL_INFO,
  YAM_LOG_LEVEL_DBG,
};

void yam_log_init(enum yam_loglevel level);

extern enum yam_loglevel YAM_LOG_LEVEL;
extern FILE *YAM_LOG_TARGET;

#define yam_dbg_assert(a) assert((a))
#define yam_log_fprintf(f, level, ...)                                        \
  if ((level) <= YAM_LOG_LEVEL) {                                             \
    /* fprintf((f), "[%d] ", level); */                                        \
    fprintf((f), __VA_ARGS__);                                                 \
  }

#define yam_dbg(...) yam_log_fprintf(stderr, YAM_LOG_LEVEL_DBG, __VA_ARGS__)
#define yam_warn(...)                                                         \
  yam_log_fprintf(stderr, YAM_LOG_LEVEL_WARN, __VA_ARGS__)
#define yam_info(...)                                                         \
  yam_log_fprintf(stderr, YAM_LOG_LEVEL_INFO, __VA_ARGS__)
#define yam_error(...)                                                        \
  yam_log_fprintf(stderr, YAM_LOG_LEVEL_ERROR, __VA_ARGS__)

#define yam_panic(...)                                                        \
  {                                                                            \
    fprintf(stderr, __VA_ARGS__);                                              \
    exit(-1);                                                                  \
  }
#define yam_todo(...)                                                         \
  { yam_panic(__VA_ARGS__); }

#endif
