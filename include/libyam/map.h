#ifndef MAP_H_
#define MAP_H_

#include <stddef.h>
#include <stdio.h>

typedef size_t (*yam_map)(FILE *f, const char *data, size_t data_len,
                          void *usr);

size_t yam_map_c(FILE *f, const char *data, size_t data_len, void *usr);

#endif
