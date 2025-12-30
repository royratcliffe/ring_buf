#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "ring_buf_item.h"

int ring_buf_item_test(int argc, char **argv) {
  (void)argc;
  (void)argv;
  int rc = EXIT_SUCCESS;

  {
    RING_BUF_DEFINE_STATIC(buf, 1U);
    ring_buf_item_length_t size;
    int err = ring_buf_item_get(&buf, NULL, &size);
    assert(err == -EAGAIN);
  }

  {
    RING_BUF_DEFINE_STATIC(buf, sizeof(float[32]));
    float number = 123.456F;
    int ack = ring_buf_item_put(&buf, &number, sizeof(number));
    assert(ack >= 0);
    (void)ring_buf_put_ack(&buf, ack);
    number = 0.0F;
    ring_buf_item_length_t size;
    ack = ring_buf_item_get(&buf, &number, &size);
    assert(ack >= 0);
    (void)ring_buf_get_ack(&buf, ack);
    assert(number == 123.456F);
    assert(size == sizeof(number));
  }

  return rc;
}
