#include "libyam/sink.h"
#include "libyam/log.h"
#include "libyam/error.h"
#include <string.h>

struct yam_sink yam_sink_init(const char *path, enum yam_sinks type,
                              size_t stride) {
  struct yam_sink self;
  memset(&self, 0, sizeof(self));

  self.type = type;
  self.stride = stride;
  self.f = fopen(path, "we");
  if (!self.f) {
    yam_errno();
  }

  return self;
}

struct yam_sink yam_sink_init_c_char_array(const char *path,
                                           enum yam_sinks type, size_t stride,
                                           const char *var_name) {
  struct yam_sink self = yam_sink_init(path, type, stride);
  self.var_name = var_name;

  return self;
}

size_t yam_sink_start(struct yam_sink *self) {
  switch (self->type) {
  case YAM_MAP_C_CHAR_ARRAY:
    return fprintf(self->f, "const char %s[] = { ", self->var_name);
    break;
  }

  return 0;
}

size_t yam_sink_convert_c_char_array(struct yam_sink *self, const char *data,
                                     size_t data_len) {
  FILE *f = self->f;
  size_t written = 0;

  for (size_t i = 0; i < data_len; i++) {
    written += fprintf(f, "%0x, ", data[i]);
  }

  return written;
}

size_t yam_sink_convert(struct yam_sink *self, const char *data,
                        size_t data_len) {

  switch (self->type) {
  case YAM_MAP_C_CHAR_ARRAY:
    return yam_sink_convert_c_char_array(self, data, data_len);
    break;
  }

  return 0;
}

size_t yam_sink_end(struct yam_sink *self) {
  switch (self->type) {
  case YAM_MAP_C_CHAR_ARRAY:
    return fprintf(self->f, "};\n");
    break;
  }

  return 0;
}

void yam_sink_free(struct yam_sink *self) { fclose(self->f); }
