#include "libyam/sink.h"
#include "libyam/drain.h"
#include "libyam/log.h"
#include "libyam/error.h"
#include "libyam/config.h"
#include "libyam/expr.h"
#include "libyam/data.h"
#include <endian.h>
#include <string.h>

struct yam_sink yam_sink_from(struct yam_config *cfg, const char *expr) {
  if (expr[0] == '\0') {
    expr = YAM_SINK_C_CHAR_ARRAY_STR;
  }

  struct yam_sink sink;
  memset(&sink, 0, sizeof(sink));

  size_t len = 0;
  const char *cmd = yam_tok_next(expr, YAM_TOK_STD_TERM, &len);
  const char *subcmd_val = NULL;

  // possible variables that can be set
  // are defined here
  const char *var_name = YAM_SINK_STD_VAR_NAME;
  size_t var_name_len = strlen(var_name);

  enum yam_endianess endianess = YAM_ENDIANESS_LITTLE;
  enum yam_int_sign int_sign = YAM_FMT_SIGNED;
  enum yam_int_fmt int_fmt = YAM_FMT_HEX;

  do {
    size_t subcmd_val_len = 0;
    subcmd_val = yam_tok_kv_adv(&cmd, &len, YAM_SINK_VAR_NAME, &subcmd_val_len);
    if (subcmd_val) {
      var_name = subcmd_val;
      var_name_len = subcmd_val_len;
      continue;
    }

    subcmd_val =
        yam_tok_kv_adv(&cmd, &len, YAM_SINK_ENDIANESS, &subcmd_val_len);
    if (subcmd_val) {
      endianess = yam_endianess_from(subcmd_val, subcmd_val_len);
      continue;
    }

    subcmd_val = yam_tok_kv_adv(&cmd, &len, YAM_SINK_SIGN_INT, &subcmd_val_len);
    if (subcmd_val) {
      int_sign = yam_int_sign_from(subcmd_val, subcmd_val_len);
      continue;
    }

    subcmd_val = yam_tok_kv_adv(&cmd, &len, YAM_SINK_FMT_INT, &subcmd_val_len);
    if (subcmd_val) {
      int_fmt = yam_int_fmt_from(subcmd_val, subcmd_val_len);
      continue;
    }

    subcmd_val = yam_tok_kv_adv(&cmd, &len, YAM_SINK_ECHO_STR, &subcmd_val_len);
    if (subcmd_val) {
      sink = yam_sink_init(YAM_SINK_ECHO, 1);
      break;
    }

    subcmd_val =
        yam_tok_kv_adv(&cmd, &len, YAM_SINK_C_CHAR_ARRAY_STR, &subcmd_val_len);
    if (subcmd_val) {
      sink = yam_sink_c_char_array(1, strndup(var_name, var_name_len));
      break;
    }

    subcmd_val = yam_tok_kv_adv(&cmd, &len, YAM_SINK_BYTE_STR, &subcmd_val_len);
    if (subcmd_val) {
      sink = yam_sink_byte(1, int_fmt, int_sign);
      break;
    }

    subcmd_val =
        yam_tok_kv_adv(&cmd, &len, YAM_SINK_SHORT_STR, &subcmd_val_len);
    if (subcmd_val) {
      sink = yam_sink_short(1, int_fmt, int_sign, endianess);
      break;
    }

    subcmd_val = yam_tok_kv_adv(&cmd, &len, YAM_SINK_INT_STR, &subcmd_val_len);
    if (subcmd_val) {
      sink = yam_sink_int(1, int_fmt, int_sign, endianess);
      break;
    }

    subcmd_val = yam_tok_kv_adv(&cmd, &len, YAM_SINK_LONG_STR, &subcmd_val_len);
    if (subcmd_val) {
      sink = yam_sink_long(1, int_fmt, int_sign, endianess);
      break;
    }

    subcmd_val =
        yam_tok_kv_adv(&cmd, &len, YAM_SINK_FLOAT_STR, &subcmd_val_len);
    if (subcmd_val) {
      sink = yam_sink_float(1, endianess);
      break;
    }

    subcmd_val =
        yam_tok_kv_adv(&cmd, &len, YAM_SINK_DOUBLE_STR, &subcmd_val_len);
    if (subcmd_val) {
      sink = yam_sink_double(1, endianess);
      break;
    }

    if (strncmp(YAM_STD_FILE, expr, strlen(YAM_STD_FILE)) != 0) {
      yam_err_fset(YAM_ERR_EXPR_SYNTAX, "Invalid assignment: '%.*s'\n",
                   (int)len, cmd);
      break;
    }
  } while (subcmd_val != NULL);

  return sink;
}

struct yam_sink yam_sink_init(enum yam_sinks type, size_t stride) {
  struct yam_sink self;
  memset(&self, 0, sizeof(self));

  self.type = type;
  self.stride = stride;

  return self;
}

struct yam_sink yam_sink_byte(size_t stride, enum yam_int_fmt fmt,
                              enum yam_int_sign sign) {
  struct yam_sink self = yam_sink_init(YAM_SINK_BYTE, stride);
  self.int_fmt = fmt;
  self.int_sign = sign;

  return self;
}

struct yam_sink yam_sink_short(size_t stride, enum yam_int_fmt fmt,
                               enum yam_int_sign sign,
                               enum yam_endianess endianess) {
  struct yam_sink self = yam_sink_init(YAM_SINK_SHORT, stride);
  self.int_fmt = fmt;
  self.int_sign = sign;
  self.int_endianess = endianess;

  return self;
}

struct yam_sink yam_sink_int(size_t stride, enum yam_int_fmt fmt,
                             enum yam_int_sign sign,
                             enum yam_endianess endianess) {
  struct yam_sink self = yam_sink_init(YAM_SINK_INT, stride);
  self.int_fmt = fmt;
  self.int_sign = sign;
  self.int_endianess = endianess;

  return self;
}

struct yam_sink yam_sink_long(size_t stride, enum yam_int_fmt fmt,
                              enum yam_int_sign sign,
                              enum yam_endianess endianess) {
  struct yam_sink self = yam_sink_init(YAM_SINK_LONG, stride);
  self.int_fmt = fmt;
  self.int_sign = sign;
  self.int_endianess = endianess;

  return self;
}

struct yam_sink yam_sink_float(size_t stride, enum yam_endianess endianess) {
  struct yam_sink self = yam_sink_init(YAM_SINK_FLOAT, stride);
  self.int_endianess = endianess;

  return self;
}

struct yam_sink yam_sink_double(size_t stride, enum yam_endianess endianess) {
  struct yam_sink self = yam_sink_init(YAM_SINK_DOUBLE, stride);
  self.int_endianess = endianess;

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
  default:
    break;
  }

  return 0;
}

size_t yam_sink_convert_c_char_array(struct yam_sink *self,
                                     struct yam_drain *drain, const char *data,
                                     size_t data_len) {
  size_t written = 0;
  for (size_t i = 0; i < data_len; i += self->stride) {
    written += yam_drain_fprintf(drain, "0x%02x, ", (uint8_t)data[i]);
  }

  return written;
}

size_t yam_sink_convert_fallback(struct yam_sink *self, struct yam_drain *drain,
                                 const char *data, size_t data_len,
                                 size_t actual_written) {
  data_len = data_len - actual_written;
  if (data_len >= actual_written) {
    return 0;
  }
  yam_warn("%ld bytes were not converted! Using fallback output!\n", data_len);
  size_t written = 0;
  for (size_t i = 0; i < data_len; i += self->stride) {
    written += yam_drain_fprintf(drain, "0x%x ", (uint8_t)data[i]);
  }

  return written;
}

size_t yam_sink_convert_byte(struct yam_sink *self, struct yam_drain *drain,
                             const char *data, size_t data_len) {
  size_t written = 0;
  const char *fmt = "%d ";

  switch (self->int_fmt) {
  case YAM_FMT_HEX:
    fmt = "0x%x ";
    break;
  case YAM_FMT_BIN:
    fmt = "0b%b ";
    break;
  case YAM_FMT_OCT:
    fmt = "0%o ";
    break;
  case YAM_FMT_DEC:
    fmt = "%d ";
    break;
  }

  if (self->int_sign == YAM_FMT_SIGNED) {
    for (size_t i = 0; i < data_len; i += self->stride) {
      written += yam_drain_fprintf(drain, fmt, (int8_t)data[i]);
    }
  } else {
    for (size_t i = 0; i < data_len; i += self->stride) {
      written += yam_drain_fprintf(drain, fmt, (uint8_t)data[i]);
    }
  }

  return written;
}

size_t yam_sink_convert_short(struct yam_sink *self, struct yam_drain *drain,
                              const char *data, size_t data_len) {
  size_t written = 0;
  const char *fmt = "%d ";

  switch (self->int_fmt) {
  case YAM_FMT_HEX:
    fmt = "0x%x ";
    break;
  case YAM_FMT_BIN:
    fmt = "0b%b ";
    break;
  case YAM_FMT_OCT:
    fmt = "0%o ";
    break;
  case YAM_FMT_DEC:
    fmt = "%d ";
    break;
  }

  size_t stride = self->stride * sizeof(int16_t);

  size_t full_len = data_len;
  data_len = data_len - data_len % sizeof(int16_t);

  for (size_t i = 0; i < data_len; i += stride) {
    int16_t res = 0;
    int16_t *d = (int16_t *)(data + i);

    if (self->int_endianess == YAM_ENDIANESS_LITTLE) {
      res = le16toh(*d);
    } else {
      res = be16toh(*d);
    }

    if (self->int_sign == YAM_FMT_SIGNED) {
      written += yam_drain_fprintf(drain, fmt, (int16_t)res);
    } else {
      written += yam_drain_fprintf(drain, fmt, (uint16_t)res);
    }
  }

  size_t actual_write = data_len;
  written +=
      yam_sink_convert_fallback(self, drain, data, full_len, actual_write);

  return written;
}

size_t yam_sink_convert_int(struct yam_sink *self, struct yam_drain *drain,
                            const char *data, size_t data_len) {
  size_t written = 0;
  const char *fmt = "%d ";

  switch (self->int_fmt) {
  case YAM_FMT_HEX:
    fmt = "0x%x ";
    break;
  case YAM_FMT_BIN:
    fmt = "0b%b ";
    break;
  case YAM_FMT_OCT:
    fmt = "0%o ";
    break;
  case YAM_FMT_DEC:
    fmt = "%d ";
    break;
  }

  size_t full_len = data_len;
  size_t stride = self->stride * sizeof(int32_t);

  data_len = data_len - data_len % sizeof(int32_t);

  for (size_t i = 0; i < data_len; i += stride) {
    int32_t res = 0;
    int32_t *d = (int32_t *)(data + i);

    if (self->int_endianess == YAM_ENDIANESS_LITTLE) {
      res = le32toh(*d);
    } else {
      res = be32toh(*d);
    }

    if (self->int_sign == YAM_FMT_SIGNED) {
      written += yam_drain_fprintf(drain, fmt, (int32_t)res);
    } else {
      written += yam_drain_fprintf(drain, fmt, (uint32_t)res);
    }
  }

  size_t actual_write = data_len;
  written +=
      yam_sink_convert_fallback(self, drain, data, full_len, actual_write);

  return written;
}

size_t yam_sink_convert_long(struct yam_sink *self, struct yam_drain *drain,
                             const char *data, size_t data_len) {
  size_t written = 0;
  const char *fmt = "%ld ";

  switch (self->int_fmt) {
  case YAM_FMT_HEX:
    fmt = "0x%lx ";
    break;
  case YAM_FMT_BIN:
    fmt = "0b%lb ";
    break;
  case YAM_FMT_OCT:
    fmt = "0%lo ";
    break;
  case YAM_FMT_DEC:
    fmt = "%ld ";
    break;
  }

  size_t full_len = data_len;
  size_t stride = self->stride * sizeof(int64_t);

  data_len = data_len - data_len % sizeof(int64_t);
  for (size_t i = 0; i < data_len; i += stride) {
    int64_t res = 0;
    int64_t *d = (int64_t *)(data + i);

    if (self->int_endianess == YAM_ENDIANESS_LITTLE) {
      res = le64toh(*d);
    } else {
      res = be64toh(*d);
    }

    if (self->int_sign == YAM_FMT_SIGNED) {
      written += yam_drain_fprintf(drain, fmt, (int64_t)res);
    } else {
      written += yam_drain_fprintf(drain, fmt, (uint64_t)res);
    }
  }

  size_t actual_write = data_len;
  written +=
      yam_sink_convert_fallback(self, drain, data, full_len, actual_write);

  return written;
}

size_t yam_sink_convert_float(struct yam_sink *self, struct yam_drain *drain,
                              const char *data, size_t data_len) {
  size_t written = 0;

  size_t full_len = data_len;
  size_t stride = self->stride * sizeof(float);

  data_len = data_len - data_len % sizeof(int32_t);

  for (size_t i = 0; i < data_len; i += stride) {
    float *d = (float *)(data + i);
    int32_t *f = (void *)d;

    if (self->int_endianess == YAM_ENDIANESS_LITTLE) {
      *f = le32toh(*f);
    } else {
      *f = be32toh(*f);
    }

    written += yam_drain_fprintf(drain, "%f ", *d);
  }

  size_t actual_write = data_len;
  written +=
      yam_sink_convert_fallback(self, drain, data, full_len, actual_write);

  return written;
}

size_t yam_sink_convert_double(struct yam_sink *self, struct yam_drain *drain,
                               const char *data, size_t data_len) {
  size_t written = 0;

  size_t full_len = data_len;
  size_t stride = self->stride * sizeof(double);

  data_len = data_len - data_len % sizeof(int64_t);

  for (size_t i = 0; i < data_len; i += stride) {
    double *d = (double *)(data + i);
    int64_t *f = (void *)d;

    if (self->int_endianess == YAM_ENDIANESS_LITTLE) {
      *f = le64toh(*f);
    } else {
      *f = be64toh(*f);
    }

    written += yam_drain_fprintf(drain, "%f ", *d);
  }

  size_t actual_write = data_len;
  written +=
      yam_sink_convert_fallback(self, drain, data, full_len, actual_write);

  return written;
}

size_t yam_sink_convert(struct yam_sink *self, struct yam_drain *drain,
                        const char *data, size_t data_len) {

  size_t read = 0;
  switch (self->type) {
  case YAM_SINK_C_CHAR_ARRAY:
    read = yam_sink_convert_c_char_array(self, drain, data, data_len);
    break;
  case YAM_SINK_ECHO:
    read = yam_drain_fprintf(drain, "%.*s", data_len, data);
    break;
  case YAM_SINK_BYTE:
    read = yam_sink_convert_byte(self, drain, data, data_len);
    break;
  case YAM_SINK_SHORT:
    read = yam_sink_convert_short(self, drain, data, data_len);
    break;
  case YAM_SINK_INT:
    read = yam_sink_convert_int(self, drain, data, data_len);
    break;
  case YAM_SINK_LONG:
    read = yam_sink_convert_long(self, drain, data, data_len);
    break;
  case YAM_SINK_FLOAT:
    read = yam_sink_convert_float(self, drain, data, data_len);
    break;
  case YAM_SINK_DOUBLE:
    read = yam_sink_convert_double(self, drain, data, data_len);
    break;
  default:
    break;
  }

  return read;
}

size_t yam_sink_end(struct yam_sink *self, struct yam_drain *drain) {
  switch (self->type) {
  case YAM_SINK_C_CHAR_ARRAY:
    return yam_drain_fprintf(drain, "};");
  default:
    break;
  }

  return 0;
}

void yam_sink_free(struct yam_sink *self) {
  switch (self->type) {
  case YAM_SINK_C_CHAR_ARRAY:
    free((void *)self->var_name);
    break;
  default:
    break;
  }
}
