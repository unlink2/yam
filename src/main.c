#include <stdio.h>
#include <stdlib.h>
#include "libyam/config.h"
#include "libyam/error.h"
#include "libyam/log.h"
#include "arg.h"

int main(int argc, char **argv) {
  struct yam_config cfg = yam_args_to_config(argc, argv);
  return 0;
  return yam_main(&cfg);
}
