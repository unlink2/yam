#include "libyam/expr.h"
#include "libyam/error.h"
#include "libyam/test/test.h"
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>

void test_expr(void **state) {
  {
    // next expr
    size_t len = 0;
    const size_t buffer_len = 64;
    char buffer[buffer_len];

    const char *expr = yam_tok_next("expr1:expr2", ':', &len);
    memset(buffer, 0, buffer_len);

    memcpy(buffer, expr, len);

    assert_int_equal(5, len);
    assert_string_equal("expr1", buffer);

    expr = yam_tok_next(expr + len, ':', &len);
    memset(buffer, 0, buffer_len);
    memcpy(buffer, expr, len);
    assert_int_equal(5, len);
    assert_string_equal("expr2", buffer);

    // null now
    expr = yam_tok_next(expr + len, ':', &len);
    assert_null(expr);

    // trim expr
    expr = yam_tok_next("  test", ':', &len);
    memset(buffer, 0, buffer_len);
    memcpy(buffer, expr, len);
    assert_int_equal(6, len);
    assert_string_equal("  test", buffer);

    expr = yam_tok_trim(expr, &len);
    memset(buffer, 0, buffer_len);
    memcpy(buffer, expr, len);
    assert_int_equal(4, len);
    assert_string_equal("test", buffer);

    len = 3;
    assert_null(yam_tok_trim("   ", &len));
  }

  {
    // to int
    assert_int_equal(123, yam_tok_to_int("123 junk", 3));
    assert_false(yam_err());

    assert_int_equal(123, yam_tok_to_int("123", 3));
    assert_false(yam_err());

    yam_tok_to_int("123 junk", 5);
    assert_int_equal(YAM_ERR_INT_CONVERSION, yam_err());
  }
}
