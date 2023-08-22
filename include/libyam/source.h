#ifndef SOURCES_H_
#define SOURCES_H_

#include <stdint.h>
#include <stdio.h>

#define YAM_READ_TO_END (-1)

#define YAM_PREFIX_FILE "file="
#define YAM_PREFIX_STRING "str="
#define YAM_PREFIX_HEX_STRING "hex="
#define YAM_PREFIX_INT32 "int32="
#define YAM_PREFIX_FLOAT32 "float32="
#define YAM_PREFIX_PADDING "pad="

#define YAM_CMD_FROM "from="
#define YAM_CMD_READ "read="

struct yam_config;

enum yam_sources {
  YAM_FILE,
  YAM_STRING,
  YAM_HEX_STRING,
  YAM_PADDING,

  YAM_INT32,
  YAM_FLOAT32
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
    int32_t ival;
    float fval;
  };
  size_t total_written;
};

struct yam_source yam_source_init(enum yam_sources type, int from, int read);
struct yam_source yam_source_from(struct yam_config *cfg, const char *expr);
struct yam_source yam_source_file(FILE *f, int from, int read);
struct yam_source yam_source_string(const char *sval, int from, int read);
struct yam_source yam_source_int32(int32_t ival);
struct yam_source yam_source_float32(float fval);

// read from a source into buffer
// returns amount of bytes written or 0 if no more writes need to be done
size_t yam_source_read(struct yam_source *self, char *buffer,
                       size_t buffer_len);

void yam_source_free(struct yam_source *self);

#endif
