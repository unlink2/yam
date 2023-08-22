#include "libyam/test/test.h"
#include "libyam/test/sink.h"
#include "libyam/test/expr.h"
#include "libyam/test/source.h"
#include "libyam/test/handle.h"
#include "libyam/test/drain.h"
#include "libyam/error.h"
#include "libyam/log.h"

static int yam_setup(void **state) {
  yam_err_set(YAM_OK);

  return 0;
}

int main(int arc, char **argv) {
  yam_log_init(YAM_LOG_LEVEL_DBG);

  const struct CMUnitTest tests[] = {
      cmocka_unit_test_setup(test_c_char_array_sink, yam_setup),
      cmocka_unit_test_setup(test_sink, yam_setup),
      cmocka_unit_test_setup(test_expr, yam_setup),
      cmocka_unit_test_setup(test_source, yam_setup),
      cmocka_unit_test_setup(test_drain, yam_setup),
      cmocka_unit_test_setup(test_handle, yam_setup),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
