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

  const char *sink_expr;
  const char *drain_expr;
};

int yam_main(struct yam_config *cfg);
struct yam_config yam_config_init(void);

FILE *yam_fopen(const char *path, const char *mode, FILE *stdfile);
void yam_fclose(FILE *f);

struct yam_handle yam_init(struct yam_config *cfg);

void yam_run(struct yam_handle *handle);

void yam_free(struct yam_handle *handle);

#endif
