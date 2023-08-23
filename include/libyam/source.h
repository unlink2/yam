#ifndef SOURCES_H_
#define SOURCES_H_

#include "libyam/data.h"
#include <stdint.h>
#include <stdio.h>

#define YAM_READ_TO_END (-1)

#define YAM_PREFIX_FILE "file="
#define YAM_PREFIX_STRING "str="
#define YAM_PREFIX_HEX_STRING "hex="
#define YAM_PREFIX_PADDING "pad="

#define YAM_PREFIX_BYTE "byte="
#define YAM_PREFIX_SHORT "short="
#define YAM_PREFIX_INT "int="
#define YAM_PREFIX_LONG "long="

#define YAM_PREFIX_FLOAT "float="
#define YAM_PREFIX_DOUBLE "double="

#define YAM_CMD_FROM "from="
#define YAM_CMD_READ "read="

struct yam_config;

enum yam_sources {
  YAM_FILE,
  YAM_STRING,
  YAM_HEX_STRING,
  YAM_PADDING,

  YAM_BYTE,
  YAM_SHORT,
  YAM_INT,
  YAM_LONG,
  YAM_FLOAT,
  YAM_DOUBLE
};

struct yam_source {
  enum yam_sources type;
  int from;
  int read;
  union {
    FILE *f;
    const char *sval;
    struct {
      int pad_char;
      size_t pad_amount;
      int pad_stride;
    };
    int8_t byteval;
    int16_t shortval;
    int32_t intval;
    int64_t longval;
    float fval;
    double dval;
  };
  size_t total_written;
};

struct yam_source yam_source_init(enum yam_sources type, int from, int read);
struct yam_source yam_source_from(struct yam_config *cfg, const char *expr);
struct yam_source yam_source_file(FILE *f, int from, int read);
struct yam_source yam_source_string(const char *sval, int from, int read);

struct yam_source yam_source_byte(int8_t ival);
struct yam_source yam_source_short(int16_t ival, enum yam_endianess endianess);
struct yam_source yam_source_int(int32_t ival, enum yam_endianess endianess);
struct yam_source yam_source_long(int64_t ival, enum yam_endianess endianess);

struct yam_source yam_source_float(float fval, enum yam_endianess endianess);
struct yam_source yam_source_double(double fval, enum yam_endianess endianess);

// read from a source into buffer
// returns amount of bytes written or 0 if no more writes need to be done
size_t yam_source_read(struct yam_source *self, char *buffer,
                       size_t buffer_len);

void yam_source_free(struct yam_source *self);

#endif
