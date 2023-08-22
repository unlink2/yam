#include "libyam/expr.h"
#include "libyam/error.h"
#include "libyam/source.h"
#include "libyam/test/test.h"
#include "libyam/config.h"
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>

void test_source(void **state) {
  struct yam_config cfg = yam_config_init();
  // init_from
  {
    // file source 1
    struct yam_source s = yam_source_from(&cfg, "-");
    assert_false(yam_err());
    assert_int_equal(YAM_FILE, s.type);
    assert_ptr_equal(stdin, s.f);
  }
  {
    // file source 1
    struct yam_source s = yam_source_from(&cfg, "file=-");
    assert_false(yam_err());
    assert_int_equal(YAM_FILE, s.type);
    assert_ptr_equal(stdin, s.f);
  }
  {
    // string source
    struct yam_source s = yam_source_from(&cfg, "str=Test String");
    assert_false(yam_err());
    assert_int_equal(YAM_STRING, s.type);
    assert_string_equal("Test String", s.sval);
  }
  {
    // from and read
    struct yam_source s =
        yam_source_from(&cfg, "from=2:read=4:str=Test String");
    assert_false(yam_err());
    assert_int_equal(YAM_STRING, s.type);
    assert_int_equal(2, s.from);
    assert_int_equal(4, s.read);
    assert_string_equal("st String", s.sval);
  }
  {
    // invalid kv pair
    yam_source_from(&cfg, "invalid=123");
    assert_int_equal(YAM_ERR_EXPR_SYNTAX, yam_err());

    yam_err_set(YAM_OK);
  }
  {
    // default source
    struct yam_source s = yam_source_from(&cfg, "");
    assert_false(yam_err());
    assert_int_equal(0, s.from);
    assert_int_equal(YAM_READ_TO_END, s.read);
    assert_int_equal(YAM_FILE, s.type);
    assert_ptr_equal(stdin, s.f);
  }
}
