#ifndef MAP_H_
#define MAP_H_

#include "libyam/drain.h"
#include <stddef.h>
#include <stdio.h>

// TODO: make this system way more dynamic
#define YAM_SINK_C_CHAR_ARRAY_STR "cchar"
#define YAM_SINK_ECHO_STR "echo"

#define YAM_SINK_ENDIANESS "endianess="
#define YAM_SINK_SIGNED_INT "sing="
#define YAM_SINK_INT_FMT "fmt="
#define YAM_SINK_VAR_NAME "name="

// allow specifying endianess with f32:le or f32:be
#define YAM_SINK_FLOAT32_STR "f32"
#define YAM_SINK_FLOAT64_STR "f64"

// allow specifying sign with i8:s or i8:u
// allow specifying format with i8:s:x, i8:s:b, i8:s:o or i8:s:d
// allow specifying endianess with i8:u:d:le or i8:u:d:be
#define YAM_SINK_INT8_STR "i8"
#define YAM_SINK_INT16_STR "i16"
#define YAM_SINK_INT32_STR "i32"
#define YAM_SINK_INT64_STR "i64"

#define YAM_SINK_STD_VAR_NAME "var"

struct yam_config;

enum yam_endianess { YAM_ENDIANESS_LITTLE, YAM_ENDIANESS_BIG };

enum yam_int_fmt { YAM_FMT_HEX, YAM_FMT_BIN, YAM_FMT_OCT, YAM_FMT_DEC };

enum yam_sinks { YAM_SINK_C_CHAR_ARRAY, YAM_SINK_ECHO };

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
