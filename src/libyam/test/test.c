#include "libyam/test/test.h"
#include "libyam/test/sink.h"
#include "libyam/error.h"
#include "libyam/log.h"

static int yam_setup(void **state) {
  yam_err_set(YAM_OK);

  return 0;
}

int main(int arc, char **argv) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test_setup(test_c_char_array_sink, yam_setup),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}

