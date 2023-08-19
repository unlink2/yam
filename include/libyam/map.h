#ifndef MAP_H_
#define MAP_H_

#include <stddef.h>
#include <stdio.h>

enum yam_maps { YAM_MAP_C_CHAR_ARRAY };

struct yam_map {
  enum yam_maps type;
  size_t stride;
  FILE *f;
  union {
    const char *var_name;
  };
};

struct yam_map yam_map_init(const char *path, enum yam_maps type,
                            size_t stride);
struct yam_map yam_map_c_char_array(const char *path, enum yam_maps type,
                                    size_t stride, const char *var_name);
size_t yam_map_start(struct yam_map *self);
size_t yam_map_convert(struct yam_map *self, const char *data, size_t data_len);
size_t yam_map_end(struct yam_map *self);
void yam_map_free(struct yam_map *self);

#endif
