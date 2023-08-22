#include "libyam/source.h"
#include "libyam/config.h"
#include "libyam/expr.h"
#include "libyam/macros.h"
#include "libyam/error.h"
#include "libyam/log.h"
#include <string.h>

struct yam_source yam_source_init(enum yam_sources type, int from, int read) {
  struct yam_source self;

  memset(&self, 0, sizeof(self));

  self.from = from;
  self.read = read;
  self.type = type;

  return self;
}

struct yam_source yam_source_from(struct yam_config *cfg, const char *expr) {
  // find first token
  size_t len = 0;
  const char *cmd = yam_tok_next(expr, YAM_TOK_STD_TERM, &len);

  if (strncmp(YAM_PREFIX_STRING, cmd, len) == 0) {
    const char *val = yam_tok_next(cmd + len, YAM_TOK_STD_TERM, &len);
    return yam_source_string(val, 0, YAM_READ_TO_END);
  } else if (strncmp(YAM_PREFIX_FILE, cmd, len) == 0) {
    const char *val = yam_tok_next(cmd + len, YAM_TOK_STD_TERM, &len);
    return yam_source_file(yam_fopen(val, "re", stdin), 0, YAM_READ_TO_END);
  }

  // default case just assumes file path input
  return yam_source_file(yam_fopen(expr, "re", stdin), 0, YAM_READ_TO_END);
}

struct yam_source yam_source_string(const char *sval, int from, int read) {
  struct yam_source self = yam_source_init(YAM_STRING, from, read);
  self.sval = sval;

  if (read == YAM_READ_TO_END) {
    self.read = (int)strlen(sval);
  }
  return self;
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

size_t yam_source_read_string(const struct yam_source *self, char *buffer,
                              size_t buffer_len) {
  if (self->total_written > self->read) {
    return 0;
  }

  size_t read_amount = MIN(buffer_len, self->read - self->total_written);

  strncpy(buffer, self->sval + self->total_written, read_amount);

  return read_amount;
}

size_t yam_source_read(struct yam_source *self, char *buffer,
                       size_t buffer_len) {
  size_t written = 0;
  switch (self->type) {
  case YAM_FILE:
    written = yam_source_read_from_file(self, buffer, buffer_len);
    break;
  case YAM_STRING:
    written = yam_source_read_string(self, buffer, buffer_len);
    break;
  case YAM_HEX_STRING:
  case YAM_PADDING:
    break;
  }

  self->total_written += written;
  return written;
}

void yam_source_free(struct yam_source *self) {}
