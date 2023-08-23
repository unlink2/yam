#include "libyam/sink.h"
#include "libyam/drain.h"
#include "libyam/log.h"
#include "libyam/error.h"
#include "libyam/config.h"
#include "libyam/expr.h"
#include "libyam/data.h"
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

  /*enum yam_endianess endianess = YAM_ENDIANESS_LITTLE;
  bool signed_int = false;
  enum yam_int_fmt int_fmt = YAM_FMT_HEX;*/

  do {
    size_t subcmd_val_len = 0;
    subcmd_val = yam_tok_kv_adv(&cmd, &len, YAM_SINK_VAR_NAME, &subcmd_val_len);
    if (subcmd_val) {
      var_name = subcmd_val;
      var_name_len = subcmd_val_len;
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
    written += yam_drain_fprintf(drain, "0x%0x, ", data[i]);
  }

  return written;
}

size_t yam_sink_convert(struct yam_sink *self, struct yam_drain *drain,
                        const char *data, size_t data_len) {

  switch (self->type) {
  case YAM_SINK_C_CHAR_ARRAY:
    return yam_sink_convert_c_char_array(self, drain, data, data_len);
  case YAM_SINK_ECHO:
    return yam_drain_fprintf(drain, "%.*s", data_len, data);
  default:
    break;
  }

  return 0;
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
