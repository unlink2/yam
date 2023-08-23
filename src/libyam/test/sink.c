#include "libyam/sink.h"
#include "libyam/config.h"
#include "libyam/data.h"
#include "libyam/drain.h"
#include "libyam/error.h"
#include "libyam/test/test.h"
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>

void test_c_char_array_sink(void **state) {
  const size_t result_len = 512;
  char result[result_len];

  {
    memset(result, 0, result_len);
    // simple case
    FILE *f = tmpfile();
    struct yam_drain drain = yam_drain_file(f);

    struct yam_sink sink = yam_sink_c_char_array(1, strdup("test_array"));

    const size_t data_len = 3;
    const char data[] = {'a', 'b', 'c'};
    yam_sink_start(&sink, &drain);
    yam_sink_convert(&sink, &drain, data, data_len);
    yam_sink_end(&sink, &drain);

    rewind(f);
    fread(result, result_len, 1, f);

    assert_string_equal("const char test_array[] = { 0x61, 0x62, 0x63, };",
                        result);
    assert_false(yam_err());

    yam_sink_free(&sink);
    yam_drain_free(&drain);

    fclose(f);
  }
  {
    memset(result, 0, result_len);
    // int
    FILE *f = tmpfile();
    struct yam_drain drain = yam_drain_file(f);

    struct yam_sink sink =
        yam_sink_int(1, YAM_FMT_HEX, YAM_FMT_SIGNED, YAM_ENDIANESS_LITTLE);
    const size_t data_len = 5;
    const char data[] = {0x1b, 0x2d, 0x3f, 0x12, 0x11};
    yam_sink_start(&sink, &drain);
    yam_sink_convert(&sink, &drain, data, data_len);
    yam_sink_end(&sink, &drain);

    rewind(f);
    fread(result, result_len, 1, f);

    assert_string_equal("0x123f2d1b 0x1b ", result);
    assert_false(yam_err());

    yam_sink_free(&sink);
    yam_drain_free(&drain);

    fclose(f);
  }
}

void test_sink(void **state) {
  struct yam_config cfg = yam_config_init();
  {
    // echo
    struct yam_sink s = yam_sink_from(&cfg, "echo");
    assert_false(yam_err());
    assert_int_equal(YAM_SINK_ECHO, s.type);
  }
  {
    // c-char
    struct yam_sink s = yam_sink_from(&cfg, "");
    assert_false(yam_err());
    assert_int_equal(YAM_SINK_C_CHAR_ARRAY, s.type);
    assert_string_equal(YAM_SINK_STD_VAR_NAME, s.var_name);

    yam_sink_free(&s);
  }
  {
    // c-char with name
    struct yam_sink s = yam_sink_from(&cfg, "name=test:cchar");
    assert_false(yam_err());
    assert_int_equal(YAM_SINK_C_CHAR_ARRAY, s.type);
    assert_string_equal("test", s.var_name);

    yam_sink_free(&s);
  }
  {
    // byte
    struct yam_sink s = yam_sink_from(&cfg, "endianess=big:fmt=d:sign=s:byte");
    assert_false(yam_err());
    assert_int_equal(YAM_SINK_BYTE, s.type);
    assert_int_equal(YAM_FMT_SIGNED, s.int_sign);
    assert_int_equal(YAM_FMT_DEC, s.int_fmt);

    yam_sink_free(&s);
  }
  {
    // short
    struct yam_sink s = yam_sink_from(&cfg, "endianess=big:fmt=d:sign=s:short");
    assert_false(yam_err());
    assert_int_equal(YAM_SINK_SHORT, s.type);
    assert_int_equal(YAM_ENDIANESS_BIG, s.int_endianess);
    assert_int_equal(YAM_FMT_SIGNED, s.int_sign);

    yam_sink_free(&s);
  }
  {
    // int
    struct yam_sink s = yam_sink_from(&cfg, "endianess=big:fmt=d:sign=s:int");
    assert_false(yam_err());
    assert_int_equal(YAM_SINK_INT, s.type);
    assert_int_equal(YAM_ENDIANESS_BIG, s.int_endianess);
    assert_int_equal(YAM_FMT_SIGNED, s.int_sign);

    yam_sink_free(&s);
  }
  {
    // int
    struct yam_sink s = yam_sink_from(&cfg, "endianess=big:fmt=d:sign=s:long");
    assert_false(yam_err());
    assert_int_equal(YAM_SINK_LONG, s.type);
    assert_int_equal(YAM_ENDIANESS_BIG, s.int_endianess);
    assert_int_equal(YAM_FMT_SIGNED, s.int_sign);

    yam_sink_free(&s);
  }
  {
    // float
    struct yam_sink s = yam_sink_from(&cfg, "endianess=big:float");
    assert_false(yam_err());
    assert_int_equal(YAM_SINK_FLOAT, s.type);
    assert_int_equal(YAM_ENDIANESS_BIG, s.float_endianess);

    yam_sink_free(&s);
  }
  {
    // double
    struct yam_sink s = yam_sink_from(&cfg, "endianess=big:double");
    assert_false(yam_err());
    assert_int_equal(YAM_SINK_DOUBLE, s.type);
    assert_int_equal(YAM_ENDIANESS_BIG, s.float_endianess);

    yam_sink_free(&s);
  }

  {
    // default case
    struct yam_sink s = yam_sink_from(&cfg, "");
    assert_false(yam_err());
    assert_int_equal(YAM_SINK_C_CHAR_ARRAY, s.type);
    assert_string_equal(YAM_SINK_STD_VAR_NAME, s.var_name);

    yam_sink_free(&s);
  }
  {
    // invalid input
    struct yam_sink s = yam_sink_from(&cfg, "test=invalid:cchar");
    assert_int_equal(YAM_ERR_EXPR_SYNTAX, yam_err());

    yam_sink_free(&s);
  }
}
