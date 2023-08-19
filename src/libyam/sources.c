#include "libyam/sources.h"
#include <string.h>

struct yam_source yam_source_init(enum yam_sources type, int from, int to) {
  struct yam_source self;

  memset(&self, 0, sizeof(self));

  self.from = from;
  self.to = to;

  return self;
}

struct yam_source yam_source_file(const char *path, int from, int to) {
  struct yam_source self = yam_source_init(YAM_FILE, from, to);

  self.from = from;
  self.to = to;

  return self;
}

size_t yam_source_read(const struct yam_source *self, char *buffer,
                       size_t buffer_len, size_t total_written) {

  size_t written = 0;

  switch (self->type) {
  case YAM_FILE:

  case YAM_STRING:
  case YAM_HEX_STRING:
  case YAM_PADDING:
    break;
  }

  return written;
}

void yam_source_free(struct yam_source *self) {}
