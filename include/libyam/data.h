#ifndef DATA_H_
#define DATA_H_

#include <stddef.h>

#define YAM_CMD_ENDIANESS "endianess="

enum yam_endianess { YAM_ENDIANESS_LITTLE, YAM_ENDIANESS_BIG };

enum yam_endianess yam_endianess_from(const char *expr, size_t len);

enum yam_int_fmt { YAM_FMT_HEX, YAM_FMT_BIN, YAM_FMT_OCT, YAM_FMT_DEC };

enum yam_int_fmt yam_int_fmt_from(const char *expr, size_t len);

enum yam_int_sign { YAM_FMT_SIGNED, YAM_FMT_UNSIGNED };

enum yam_int_sign yam_int_sign_from(const char *expr, size_t len);

#endif
