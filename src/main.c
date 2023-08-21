#include <stdio.h>
#include <stdlib.h>
#include "libyam/config.h"
#include "libyam/error.h"
#include "libyam/log.h"
#include "arg.h"

int main(int argc, char **argv) {
  yam_log_init(YAM_LOG_LEVEL_ERROR);
  struct yam_config cfg = yam_args_to_config(argc, argv);
  int ret_val = yam_main(&cfg);

  yam_args_free();
  return ret_val;
}
