#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "ring_buf.h"
#include "ring_buf_circ.h"

int put_circular_float(struct ring_buf *buf, float number) {
  return ring_buf_put_circ(buf, &number, sizeof(number));
}

int ring_buf_circular_float_test(int argc, char **argv) {
  (void)argc;
  (void)argv;
  RING_BUF_DEFINE_STATIC(buf, sizeof(float[2U]));

  /*
   * Reset the buffer to a size just below the maximum. This forces the buffer
   * to wrap around when putting data.
   */
  ring_buf_reset(&buf, RING_BUF_SIZE_MAX - 1);

  /*
   * Fill the buffer with ten floats, causing the first eight to be overwritten.
   * Then read back the floats and verify that only the last two remain.
   */
  for (float number = 1.0F; number <= 10.0F; number += 1.0F) {
    int err = put_circular_float(&buf, number);
    assert(err == 0);
  }
  float sum = 0.0F, number;
  ring_buf_size_t ack;
  while ((ack = ring_buf_get(&buf, &number, sizeof(number))))
    sum += number;
  assert(sum == 9.0F + 10.0F);
  ring_buf_get_ack(&buf, 0U);
  while ((ack = ring_buf_get(&buf, &number, sizeof(number)))) {
    int err = ring_buf_get_ack(&buf, ack);
    assert(err == 0);
    printf("%f\n", number);
  }

  return EXIT_SUCCESS;
}
