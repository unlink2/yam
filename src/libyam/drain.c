#include "libyam/drain.h"
#include "libyam/error.h"
#include "libyam/log.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

struct yam_drain yam_drain_init(enum yam_drains type) {
  struct yam_drain self;
  memset(&self, 0, sizeof(self));
  self.type = type;

  return self;
}

struct yam_drain yam_drain_file(FILE *f) {
  struct yam_drain self = yam_drain_init(YAM_DRAIN_FILE);

  self.f = f;
  if (!self.f) {
    yam_errno();
  }

  return self;
}

size_t yam_drain_fprintf(struct yam_drain *self, const char *restrict format,
                         ...) {
  size_t written = 0;

  va_list args;
  va_start(args, format);

  switch (self->type) {
  case YAM_DRAIN_FILE:
    written = vfprintf(self->f, format, args);
    break;
  }

  va_end(args);
  return written;
}

void yam_drain_free(struct yam_drain *self) {}
