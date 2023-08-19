#ifndef MAP_H_
#define MAP_H_

#include <stddef.h>
#include <stdio.h>

// TODO: make this system way more dynamic

enum yam_sinks { YAM_MAP_C_CHAR_ARRAY };

struct yam_sink {
  enum yam_sinks type;
  size_t stride;
  FILE *f;
  union {
    const char *var_name;
  };
};

struct yam_sink yam_sink_init(const char *path, enum yam_sinks type,
                              size_t stride);
struct yam_sink yam_sink_c_char_array(const char *path, enum yam_sinks type,
                                      size_t stride, const char *var_name);
size_t yam_sink_start(struct yam_sink *self);
size_t yam_sink_convert(struct yam_sink *self, const char *data,
                        size_t data_len);
size_t yam_sink_end(struct yam_sink *self);
void yam_sink_free(struct yam_sink *self);

#endif
