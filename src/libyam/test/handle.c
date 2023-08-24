#include "libyam/drain.h"
#include "libyam/expr.h"
#include "libyam/error.h"
#include "libyam/config.h"
#include "libyam/source.h"
#include "libyam/test/handle.h"
#include "libyam/test/test.h"
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>

#define yam_setup(source_str, sink_str)                                        \
  struct yam_config cfg = yam_config_init();                                   \
  const size_t result_len = 512;                                               \
  char result[result_len];                                                     \
  memset(result, 0, result_len);                                               \
  FILE *fo = tmpfile();                                                        \
  struct yam_source source[] = {yam_source_from(&cfg, (source_str))};          \
  struct yam_drain drain = yam_drain_file(fo);                                 \
  struct yam_sink sink = yam_sink_from(&cfg, (sink_str));                      \
  struct yam_handle handle = yam_handle_init(source, 1, sink, drain);          \
  yam_handle_run(&handle);                                                     \
  rewind(fo);                                                                  \
  fread(result, result_len, 1, fo);

#define yam_teardown()                                                         \
  yam_source_free(&source[0]);                                                 \
  yam_sink_free(&sink);                                                        \
  yam_drain_free(&drain);                                                      \
  yam_handle_free(&handle);                                                    \
  fclose(fo);

// integration tests
void test_handle(void **state) {
  {
    yam_setup("str=abc", "");

    assert_false(yam_err());
    assert_string_equal("const char var[] = { 0x61, 0x62, 0x63, };\n", result);

    yam_teardown();
  }
  {
    yam_setup("endianess=little:short=0x1234", "endianess=big:short");

    assert_false(yam_err());
    assert_string_equal("0x3412 \n", result);

    yam_teardown();
  }
}
