#include "libyam/config.h"
#include "libyam/log.h"
#include <string.h>

const char *default_sources[] = {YAM_STD_FILE};
const size_t default_sources_len = 1;

struct yam_config yam_config_init(void) {
  struct yam_config c;
  memset(&c, 0, sizeof(c));

  c.verbose = YAM_LOG_LEVEL_ERROR;
  c.drain_str = "c-char-array";
  c.sink = YAM_STD_FILE;

  c.source_exprs = default_sources;
  c.source_exprs_len = default_sources_len;

  return c;
}

FILE *yam_fopen(const char *path, const char *mode, FILE *stdfile) {
  if (strncmp(YAM_STD_FILE, path, 1) == 0) {
    return stdfile;
  }
  return fopen(path, mode);
}

void yam_fclose(FILE *f) {
  if (f == stdin || f == stdout || f == stderr) {
    return;
  }
  fclose(f);
}

struct yam_handle yam_setup(struct yam_config *cfg) {}

void yam_run(struct yam_handle *handle) {}

void yam_exit(struct yam_config *cfg, struct yam_handle *handle) {}
