#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "ring_buf.h"

/*
 * Use \c ring_buf_get rather than \c ring_buf_get_claim, as the former handles
 * discontiguous items. They amount to the same thing when the buffer size is a
 * multiple of the item size.
 */
int put_circular_float(struct ring_buf *buf, float number) {
  if (ring_buf_is_full(buf))
    (void)ring_buf_get_ack(buf, ring_buf_get(buf, NULL, sizeof(number)));
  if (sizeof(number) > ring_buf_free_space(buf))
    return -EMSGSIZE;
  return ring_buf_put_ack(buf, ring_buf_put(buf, &number, sizeof(number)));
}

int ring_buf_circular_float_test(int argc, char **argv) {
  RING_BUF_DEFINE(buf, sizeof(float[2U]));
  ring_buf_reset(&buf, RING_BUF_SIZE_MAX - 1);
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
