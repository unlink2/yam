#ifndef ARG_H_
#define ARG_H_

#include "argtable2.h"
#include <stdlib.h>

#include "libyam/config.h"
#include "libyam/log.h"
#include "libyam/macros.h"

struct yam_config yam_args_to_config(int argc, char **argv);

#endif
