#include "libyam/source.h"
#include "libyam/config.h"
#include "libyam/expr.h"
#include "libyam/macros.h"
#include "libyam/error.h"
#include "libyam/log.h"
#include "libyam/sink.h"
#include "libyam/data.h"
#include <endian.h>
#include <string.h>

struct yam_source yam_source_init(enum yam_sources type, int from, int read) {
  struct yam_source self;

  memset(&self, 0, sizeof(self));

  // TODO: make these asserts yam_errs and test them
  yam_dbg_assert(from >= 0);
  self.from = from;
  self.read = read;
  self.type = type;

  return self;
}

struct yam_source yam_source_from(struct yam_config *cfg, const char *expr) {
  if (expr[0] == '\0') {
    expr = YAM_STD_FILE;
  }

  // find first token
  size_t len = 0;
  const char *cmd = yam_tok_next(expr, YAM_TOK_STD_TERM, &len);

  int from = 0;
  int read = YAM_READ_TO_END;
  enum yam_endianess endianess = YAM_ENDIANESS_LITTLE;

  const char *subcmd_val = NULL;
  do {
    size_t subcmd_val_len = 0;
    subcmd_val = yam_tok_kv_adv(&cmd, &len, YAM_CMD_FROM, &subcmd_val_len);
    if (subcmd_val) {
      from = (int)yam_tok_to_int(subcmd_val, subcmd_val_len);
      continue;
    }

    subcmd_val = yam_tok_kv_adv(&cmd, &len, YAM_CMD_READ, &subcmd_val_len);
    if (subcmd_val) {
      read = (int)yam_tok_to_int(subcmd_val, subcmd_val_len);
      continue;
    }

    subcmd_val = yam_tok_kv_adv(&cmd, &len, YAM_CMD_ENDIANESS, &subcmd_val_len);
    if (subcmd_val) {
      endianess = yam_endianess_from(subcmd_val, subcmd_val_len);
      continue;
    }

    subcmd_val = yam_tok_kv_adv(&cmd, &len, YAM_PREFIX_STRING, &subcmd_val_len);
    if (subcmd_val) {
      return yam_source_string(subcmd_val, from, read);
    }

    subcmd_val = yam_tok_kv_adv(&cmd, &len, YAM_PREFIX_FILE, &subcmd_val_len);
    if (subcmd_val) {
      return yam_source_file(yam_fopen(subcmd_val, "re", stdin), from, read);
    }

    // ints

    subcmd_val = yam_tok_kv_adv(&cmd, &len, YAM_PREFIX_BYTE, &subcmd_val_len);
    if (subcmd_val) {
      return yam_source_byte(
          (int8_t)yam_tok_to_int(subcmd_val, subcmd_val_len));
    }

    subcmd_val = yam_tok_kv_adv(&cmd, &len, YAM_PREFIX_SHORT, &subcmd_val_len);
    if (subcmd_val) {
      return yam_source_short(
          (int16_t)yam_tok_to_int(subcmd_val, subcmd_val_len), endianess);
    }

    subcmd_val = yam_tok_kv_adv(&cmd, &len, YAM_PREFIX_INT, &subcmd_val_len);
    if (subcmd_val) {
      return yam_source_int((int32_t)yam_tok_to_int(subcmd_val, subcmd_val_len),
                            endianess);
    }

    subcmd_val = yam_tok_kv_adv(&cmd, &len, YAM_PREFIX_LONG, &subcmd_val_len);
    if (subcmd_val) {
      return yam_source_long(
          (int64_t)yam_tok_to_int(subcmd_val, subcmd_val_len), endianess);
    }

    // floats

    subcmd_val = yam_tok_kv_adv(&cmd, &len, YAM_PREFIX_FLOAT, &subcmd_val_len);
    if (subcmd_val) {
      return yam_source_float(yam_tok_to_float(subcmd_val, subcmd_val_len),
                              endianess);
    }

    subcmd_val = yam_tok_kv_adv(&cmd, &len, YAM_PREFIX_DOUBLE, &subcmd_val_len);
    if (subcmd_val) {
      return yam_source_double(yam_tok_to_float(subcmd_val, subcmd_val_len),
                               endianess);
    }

    if (strncmp(YAM_STD_FILE, expr, strlen(YAM_STD_FILE)) != 0) {
      yam_err_fset(YAM_ERR_EXPR_SYNTAX, "Invalid assignment: '%.*s'\n",
                   (int)len, cmd);
      return yam_source_init(0, 0, 0);
    }
  } while (subcmd_val != NULL);

  // default case just assumes file path input
  return yam_source_file(yam_fopen(expr, "re", stdin), from, read);
}

struct yam_source yam_source_byte(int8_t ival) {
  struct yam_source self = yam_source_init(YAM_INT, 0, sizeof(ival));
  self.byteval = ival;
  return self;
}

struct yam_source yam_source_short(int16_t ival, enum yam_endianess endianess) {
  struct yam_source self = yam_source_init(YAM_INT, 0, sizeof(ival));

  if (endianess == YAM_ENDIANESS_LITTLE) {
    self.shortval = htole16(ival);
  } else {
    self.shortval = htobe16(ival);
  }

  return self;
}

struct yam_source yam_source_int(int32_t ival, enum yam_endianess endianess) {
  struct yam_source self = yam_source_init(YAM_INT, 0, sizeof(ival));

  if (endianess == YAM_ENDIANESS_LITTLE) {
    self.intval = htole32(ival);
  } else {
    self.intval = htobe32(ival);
  }

  return self;
}

struct yam_source yam_source_long(int64_t ival, enum yam_endianess endianess) {
  struct yam_source self = yam_source_init(YAM_INT, 0, sizeof(ival));

  if (endianess == YAM_ENDIANESS_LITTLE) {
    self.longval = htole64(ival);
  } else {
    self.longval = htobe64(ival);
  }

  return self;
}

struct yam_source yam_source_float(float fval, enum yam_endianess endianess) {
  struct yam_source self = yam_source_init(YAM_FLOAT, 0, sizeof(fval));
  self.fval = fval;

  int32_t *pfval = (void *)&self.fval;
  if (endianess == YAM_ENDIANESS_LITTLE) {
    *pfval = htole32(*pfval);
  } else {
    *pfval = htobe32(*pfval);
  }

  return self;
}

struct yam_source yam_source_double(double fval, enum yam_endianess endianess) {
  struct yam_source self = yam_source_init(YAM_FLOAT, 0, sizeof(fval));
  self.dval = fval;

  int64_t *pfval = (void *)&self.dval;
  if (endianess == YAM_ENDIANESS_LITTLE) {
    *pfval = htole64(*pfval);
  } else {
    *pfval = htobe64(*pfval);
  }

  return self;
}

struct yam_source yam_source_string(const char *sval, int from, int read) {
  struct yam_source self = yam_source_init(YAM_STRING, from, read);
  size_t sval_len = strlen(sval);
  yam_dbg_assert(from < sval_len);

  self.sval = sval + from;
  sval_len = strlen(self.sval);

  if (read == YAM_READ_TO_END) {
    self.read = (int)sval_len;
  }

  yam_dbg_assert(self.read <= sval_len);

  return self;
}

struct yam_source yam_source_file(FILE *f, int from, int read) {
  struct yam_source self = yam_source_init(YAM_FILE, from, read);

  self.f = f;
  if (!self.f) {
    return self;
  }

  if (self.from && f != stdin) {
    if (fseek(self.f, self.from, SEEK_SET) == -1) {
      yam_errno();
    }
  } else if (self.from) {
    for (size_t i = 0; i < self.from; i++) {
      fgetc(self.f);
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

size_t yam_source_read_ptr(const struct yam_source *self, char *buffer,
                           size_t buffer_len, void *data) {
  if (self->total_written > self->read) {
    return 0;
  }

  size_t read_amount = MIN(buffer_len, self->read - self->total_written);
  memcpy(buffer, (char *)data + self->total_written, read_amount);
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
  case YAM_INT:
    written = yam_source_read_ptr(self, buffer, buffer_len, &self->intval);
    break;
  case YAM_FLOAT:
    written = yam_source_read_ptr(self, buffer, buffer_len, &self->fval);
    break;
  case YAM_DOUBLE:
    written = yam_source_read_ptr(self, buffer, buffer_len, &self->dval);
    break;
  case YAM_HEX_STRING:
  case YAM_PADDING:
    break;
  }

  self->total_written += written;
  return written;
}

void yam_source_free(struct yam_source *self) {}
