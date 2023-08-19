#include "libyam/handle.h"
#include "libyam/error.h"
#include "libyam/sink.h"
#include <string.h>

struct yam_handle yam_handle_init(struct yam_source *sources,
                                  size_t sources_len, struct yam_sink *sinks,
                                  size_t sinks_len) {
  struct yam_handle self;
  memset(&self, 0, sizeof(self));
  return self;
}

void yam_handle_run(struct yam_handle *self) {
  size_t buffer_len = 128;
  char buffer[buffer_len];

  yam_sink_start(self->sink);
  if (yam_err()) {
    return;
  }

  for (size_t i = 0; i < self->sources_len; i++) {
    size_t read = 0;
    while ((read = yam_source_read(&self->sources[i], buffer, buffer_len)) !=
           0) {
      yam_sink_convert(self->sink, buffer, read);
      if (yam_err()) {
        return;
      }
    }
  }

  yam_sink_end(self->sink);
  if (yam_err()) {
    return;
  }
}

void yam_handle_free(struct yam_handle *self) {}
