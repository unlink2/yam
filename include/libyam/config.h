#ifndef CONFIG_H_
#define CONFIG_H_

#include <stddef.h>
#include "libyam/handle.h"
#include "libyam/drain.h"
#include "libyam/sink.h"

#define YAM_STD_FILE "-"

struct yam_config {
  int verbose;

  const char **source_exprs;
  size_t source_exprs_len;

  const char *sink;
  const char *drain_str;
};

struct yam_config yam_config_init(void);

struct yam_handle yam_setup(struct yam_config *cfg);

#endif
