#ifndef SOURCES_H_
#define SOURCES_H_

#include <stdio.h>

#define YAM_READ_TO_END (-1)

enum yam_sources { YAM_FILE, YAM_STRING, YAM_HEX_STRING, YAM_PADDING };

struct yam_source {
  enum yam_sources type;
  int from;
  int read;
  union {
    FILE *f;
    const char *sval;
    struct {
      char pad_char;
      size_t pad_amount;
    };
  };
  size_t total_written;
};

struct yam_source yam_source_init(enum yam_sources type, int from, int read);
struct yam_source yam_source_from(const char *expr);
struct yam_source yam_source_file(FILE *f, int from, int read);

// read from a source into buffer
// returns amount of bytes written or 0 if no more writes need to be done
size_t yam_source_read(const struct yam_source *self, char *buffer,
                       size_t buffer_len);

void yam_source_free(struct yam_source *self);

#endif
