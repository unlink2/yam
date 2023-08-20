#include "libyam/config.h"
#include "libyam/drain.h"
#include "libyam/error.h"
#include "libyam/log.h"
#include "libyam/sink.h"
#include <string.h>

const char *default_sources[] = {YAM_STD_FILE};
const size_t default_sources_len = 1;

int yam_main(struct yam_config *cfg) {
  struct yam_handle handle = yam_init(cfg);
  if (!yam_err()) {
    yam_run(&handle);
  }
  yam_free(&handle);
  return yam_err();
}

struct yam_config yam_config_init(void) {
  struct yam_config c;
  memset(&c, 0, sizeof(c));

  c.verbose = YAM_LOG_LEVEL_ERROR;
  c.drain_expr = YAM_STD_FILE;
  c.sink_expr = YAM_SINK_C_CHAR_ARRAY_STR;

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

struct yam_handle yam_init(struct yam_config *cfg) {
  struct yam_handle handle;
  memset(&handle, 0, sizeof(handle));

  yam_log_init(cfg->verbose);

  size_t sources_len = cfg->source_exprs_len;
  struct yam_source *sources = malloc(sources_len * sizeof(struct yam_source));
  for (size_t i = 0; i < sources_len; i++) {
    sources[i] = yam_source_from(cfg->source_exprs[i]);
    if (yam_err()) {
      return handle;
    }
  }

  struct yam_sink sink = yam_sink_from(cfg->sink_expr);
  struct yam_drain drain = yam_drain_from(cfg->drain_expr);

  handle = yam_handle_init(sources, sources_len, sink, drain);

  return handle;
}

void yam_run(struct yam_handle *handle) { yam_handle_run(handle); }

void yam_free(struct yam_handle *handle) {
  for (size_t i = 0; i < handle->sources_len; i++) {
    yam_source_free(&handle->sources[i]);
  }
  yam_drain_free(&handle->drain);
  yam_sink_free(&handle->sink);
  yam_handle_free(handle);
  free(handle->sources);
}
