#ifndef HANDLE_H_
#define HANDLE_H_

#include "libyam/macros.h"
#include "libyam/sink.h"
#include "libyam/source.h"
#include "libyam/drain.h"

struct yam_handle {
  struct yam_source *sources;
  size_t sources_len;
  struct yam_sink sink;
  struct yam_drain drain;

  bool print_total_read;
  bool no_lf;
};

struct yam_handle yam_handle_init(struct yam_source *sources,
                                  size_t sources_len, struct yam_sink sink,
                                  struct yam_drain drain);

void yam_handle_run(struct yam_handle *self);

void yam_handle_free(struct yam_handle *self);

#endif
