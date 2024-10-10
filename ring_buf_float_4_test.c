#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "ring_buf.h"

int ring_buf_float_4_test(int argc, char **argv) {
  RING_BUF_DECLARE(buf, sizeof(float[4U]));
  float number = 1.0F;
  ring_buf_size_t ack;
  while ((ack = ring_buf_put(&buf, &number, sizeof(number)))) {
    int err = ring_buf_put_ack(&buf, ack);
    assert(err == 0);
    number += 1.0F;
  }
  float sum = 0.0F;
  while ((ack = ring_buf_get(&buf, &number, sizeof(number))))
    sum += number;
  assert(sum == 1.0F + 2.0F + 3.0F + 4.0F);
  assert(ring_buf_free(&buf) == 0U);
  ring_buf_get_ack(&buf, 0U);
  assert(ring_buf_used(&buf) == sizeof(float[4U]));
  while ((ack = ring_buf_get(&buf, &number, sizeof(number)))) {
    int err = ring_buf_get_ack(&buf, ack);
    assert(err == 0);
    printf("%f\n", number);
  }
  assert(ring_buf_free(&buf) == sizeof(float[4U]));
  assert(ring_buf_used(&buf) == 0U);

  return EXIT_SUCCESS;
}
