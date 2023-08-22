#include "libyam/sink.h"
#include "libyam/error.h"
#include "libyam/test/test.h"
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>

void test_c_char_array_sink(void **state) {
  const size_t result_len = 512;
  char result[result_len];
  memset(result, 0, result_len);

  {
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
}
