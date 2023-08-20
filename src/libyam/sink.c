#include "libyam/sink.h"
#include "libyam/drain.h"
#include "libyam/log.h"
#include "libyam/error.h"
#include <string.h>

struct yam_sink yam_sink_init(enum yam_sinks type, size_t stride) {
  struct yam_sink self;
  memset(&self, 0, sizeof(self));

  self.type = type;
  self.stride = stride;

  return self;
}

struct yam_sink yam_sink_c_char_array(size_t stride, const char *var_name) {
  struct yam_sink self = yam_sink_init(YAM_SINK_C_CHAR_ARRAY, stride);
  self.var_name = var_name;

  return self;
}

size_t yam_sink_start(struct yam_sink *self, struct yam_drain *drain) {
  switch (self->type) {
  case YAM_SINK_C_CHAR_ARRAY:
    return yam_drain_fprintf(drain, "const char %s[] = { ", self->var_name);
    break;
  }

  return 0;
}

size_t yam_sink_convert_c_char_array(struct yam_sink *self,
                                     struct yam_drain *drain, const char *data,
                                     size_t data_len) {
  size_t written = 0;
  for (size_t i = 0; i < data_len; i += self->stride) {
    written += yam_drain_fprintf(drain, "0x%0x, ", data[i]);
  }

  return written;
}

size_t yam_sink_convert(struct yam_sink *self, struct yam_drain *drain,
                        const char *data, size_t data_len) {

  switch (self->type) {
  case YAM_SINK_C_CHAR_ARRAY:
    return yam_sink_convert_c_char_array(self, drain, data, data_len);
    break;
  }

  return 0;
}

size_t yam_sink_end(struct yam_sink *self, struct yam_drain *drain) {
  switch (self->type) {
  case YAM_SINK_C_CHAR_ARRAY:
    return yam_drain_fprintf(drain, "};");
    break;
  }

  return 0;
}

void yam_sink_free(struct yam_sink *self) {}
