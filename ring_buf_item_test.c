#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "ring_buf_item.h"

int ring_buf_item_test(int argc, char **argv) {
  int rc = EXIT_SUCCESS;

  {
    RING_BUF_DECLARE(buf, 1U);
    ring_buf_item_size_t size;
    int err = ring_buf_item_get(&buf, NULL, &size);
    assert(err == -EAGAIN);
  }

  {
    RING_BUF_DECLARE(buf, sizeof(float[32]));
    float number = 123.456F;
    int err = ring_buf_item_put(&buf, &number, sizeof(number));
    assert(err == 0);
    number = 0.0F;
    ring_buf_item_size_t size;
    err = ring_buf_item_get(&buf, &number, &size);
    assert(err == 0);
    assert(number == 123.456F);
    assert(size == sizeof(number));
  }

  return rc;
}
