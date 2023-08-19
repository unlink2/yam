#ifndef HANDLE_H_
#define HANDLE_H_

#include "libyam/sink.h"
#include "libyam/source.h"

struct yam_handle {
  struct yam_source *sources;
  size_t sources_len;
  struct yam_sink *sink;
};

struct yam_handle yam_handle_init(struct yam_source *sources,
                                  size_t sources_len, struct yam_sink *sink,
                                  size_t sinks_len);

void yam_handle_run(struct yam_handle *self);

void yam_handle_free(struct yam_handle *self);

#endif
