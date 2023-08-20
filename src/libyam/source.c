#include "libyam/source.h"
#include "libyam/config.h"
#include "libyam/macros.h"
#include "libyam/error.h"
#include "libyam/log.h"
#include <string.h>

struct yam_source yam_source_init(enum yam_sources type, int from, int read) {
  struct yam_source self;

  memset(&self, 0, sizeof(self));

  self.from = from;
  self.read = read;

  return self;
}

struct yam_source yam_source_from(struct yam_config *cfg, const char *expr) {
  /*  TODO:
      Implement input source syntax that allows like this:
      No specific syntax -> file path
      hex:01 02 02 -> hex string
      file:/path/to/file -> file path
      str:Hello world -> regular string
      pad:0:100 -> pad with 100 * '0'
  */
  return yam_source_file(yam_fopen(expr, "re", stdin), 0, YAM_READ_TO_END);
}

struct yam_source yam_source_file(FILE *f, int from, int read) {
  struct yam_source self = yam_source_init(YAM_FILE, from, read);

  self.f = f;
  if (!self.f) {
    return self;
  }

  if (self.from) {
    if (fseek(self.f, self.from, SEEK_SET) == -1) {
      yam_errno();
    }
  }

  return self;
}

size_t yam_source_read_from_file(const struct yam_source *self, char *buffer,
                                 size_t buffer_len) {
  if (self->total_written > self->read) {
    return 0;
  }

  size_t read_amount = MIN(buffer_len, self->read - self->total_written);

  size_t written = fread(buffer, 1, read_amount, self->f);

  if (written == -1) {
    yam_errno();
  }

  return written;
}

size_t yam_source_read(const struct yam_source *self, char *buffer,
                       size_t buffer_len) {
  switch (self->type) {
  case YAM_FILE:
    return yam_source_read_from_file(self, buffer, buffer_len);
  case YAM_STRING:
  case YAM_HEX_STRING:
  case YAM_PADDING:
    break;
  }

  return 0;
}

void yam_source_free(struct yam_source *self) {}
