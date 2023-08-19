#include "libyam/config.h"
#include <string.h>

struct yam_config yam_config_init(void) {
  struct yam_config c;
  memset(&c, 0, sizeof(c));

  return c;
}
