#ifndef MAP_H_
#define MAP_H_

#include "libyam/drain.h"
#include <stddef.h>
#include <stdio.h>

// TODO: make this system way more dynamic
#define YAM_SINK_C_CHAR_ARRAY_STR "c-char-array"

struct yam_config;

enum yam_sinks { YAM_SINK_C_CHAR_ARRAY };

struct yam_sink {
  enum yam_sinks type;
  size_t stride;
  union {
    const char *var_name;
  };
};

struct yam_sink yam_sink_from(struct yam_config *cfg, const char *expr);
struct yam_sink yam_sink_init(enum yam_sinks type, size_t stride);

struct yam_sink yam_sink_c_char_array(size_t stride, const char *var_name);

// always called before conversion starts
size_t yam_sink_start(struct yam_sink *self, struct yam_drain *drain);

// converts data read from a source into a new format
// Note: a sink cannot realy on the data being complete at this stage.
// This may be called multiple times until all data is read
size_t yam_sink_convert(struct yam_sink *self, struct yam_drain *drain,
                        const char *data, size_t data_len);

// always called
size_t yam_sink_end(struct yam_sink *self, struct yam_drain *drain);

void yam_sink_free(struct yam_sink *self);

#endif
