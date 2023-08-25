#ifndef MAP_H_
#define MAP_H_

#include "libyam/data.h"
#include "libyam/drain.h"
#include <stddef.h>
#include <stdio.h>

// TODO: make this system way more dynamic
#define YAM_SINK_C_CHAR_ARRAY_STR "cchar"
#define YAM_SINK_ECHO_STR "echo"

#define YAM_SINK_ENDIANESS "endianess="
#define YAM_SINK_SIGN_INT "sign="
#define YAM_SINK_FMT_INT "fmt="
#define YAM_SINK_VAR_NAME "name="

#define YAM_SINK_PRE "pre="
#define YAM_SINK_POST "post="

// allow specifying endianess with f32:le or f32:be
#define YAM_SINK_FLOAT_STR "float"
#define YAM_SINK_DOUBLE_STR "double"

// allow specifying sign with i8:s or i8:u
// allow specifying format with i8:s:x, i8:s:b, i8:s:o or i8:s:d
// allow specifying endianess with i8:u:d:le or i8:u:d:be
#define YAM_SINK_BYTE_STR "byte"
#define YAM_SINK_SHORT_STR "short"
#define YAM_SINK_INT_STR "int"
#define YAM_SINK_LONG_STR "long"

#define YAM_SINK_STD_VAR_NAME "var"

#define YAM_HEX_PREFIX "0x"
#define YAM_BIN_PREFIX "0b"
#define YAM_OCT_PREFIX "0"

struct yam_config;

enum yam_sinks {
  YAM_SINK_C_CHAR_ARRAY,
  YAM_SINK_ECHO,

  YAM_SINK_BYTE,
  YAM_SINK_SHORT,
  YAM_SINK_INT,
  YAM_SINK_LONG,

  YAM_SINK_FLOAT,
  YAM_SINK_DOUBLE,
};

enum yam_sink_flags {
  YAM_SINK_FLAG_FREE_PRE = 1,
  YAM_SINK_FLAG_FREE_POST = 2,
};

struct yam_sink {
  enum yam_sinks type;
  size_t stride;
  const char *pre;
  const char *post;
  enum yam_sink_flags flags;
  union {
    const char *var_name;
    struct {
      enum yam_endianess int_endianess;
      const char *int_fmt_str;
      enum yam_int_sign int_sign;
      enum yam_int_fmt int_fmt;
    };
    enum yam_endianess float_endianess;
  };
};

struct yam_sink yam_sink_from(struct yam_config *cfg, const char *expr);
struct yam_sink yam_sink_init(enum yam_sinks type, size_t stride);

struct yam_sink yam_sink_byte(size_t stride, enum yam_int_fmt fmt,
                              enum yam_int_sign sign);

struct yam_sink yam_sink_short(size_t stride, enum yam_int_fmt fmt,
                               enum yam_int_sign sign,
                               enum yam_endianess endianess);

struct yam_sink yam_sink_int(size_t stride, enum yam_int_fmt fmt,
                             enum yam_int_sign sign,
                             enum yam_endianess endianess);

struct yam_sink yam_sink_long(size_t stride, enum yam_int_fmt fmt,
                              enum yam_int_sign sign,
                              enum yam_endianess endianess);

struct yam_sink yam_sink_float(size_t stride, enum yam_endianess endianess);
struct yam_sink yam_sink_double(size_t stride, enum yam_endianess endianess);

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
