#ifndef DRAIN_H_
#define DRAIN_H_

#include <stdio.h>
enum yam_drains { YAM_DRAIN_FILE };

struct yam_drain {
  enum yam_drains type;
  union {
    FILE *f;
  };
};

struct yam_drain yam_drain_from(const char *expr);
struct yam_drain yam_drain_init(enum yam_drains type);
struct yam_drain yam_drain_file(FILE *f);

size_t yam_drain_fprintf(struct yam_drain *self, const char *restrict format,
                         ...);

void yam_drain_free(struct yam_drain *self);

#endif
