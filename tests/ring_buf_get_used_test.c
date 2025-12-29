#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "ring_buf.h"

int ring_buf_get_used_test(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  /*
   * Define and reset ring buffer.
   * The base index is set to sizeof(int[2]) to test non-zero bases.
   */
  RING_BUF_DEFINE(buf, sizeof(int[4]));
  ring_buf_reset(&buf, sizeof(int[2]));

  /*
   * Put four integers into the buffer. Put them one at a time. Putting "all or
   * none" automatically acknowledges the put zone. Assert that each put
   * operation succeeds.
   */
  for (int y = 1; y <= 4; y++)
  {
    int err = ring_buf_put_all(&buf, &y, sizeof(y));
    assert(err == 0);
  }

  /*
   * Get all the used space in the buffer. Copy the data into a dynamically
   * allocated array. Assert that the correct amount of data is copied.
   *
   * Does not acknowledge the get zone on purpose.
   */
  ring_buf_size_t len = ring_buf_used_space(&buf) / sizeof(int);
  assert(len == 4);
  int *data = malloc(len * sizeof(int));
  assert(data != NULL);
  ring_buf_size_t ack = ring_buf_get(&buf, data, len * sizeof(int));
  assert(ack == len * sizeof(int));
  ring_buf_get_ack(&buf, 0);

  /*
   * Verify the copied data. The data should be 1, 2, 3, 4 in that order.
   */
  assert(data[0] == 1);
  assert(data[1] == 2);
  assert(data[2] == 3);
  assert(data[3] == 4);

  /*
   * Print the data. The loop variable t is of type ring_buf_size_t to match the
   * type used in the ring buffer API. It stands for time, as in a time series.
   * Get each element using ring_buf_get_all to demonstrate its use. It
   * acknowledges each get operation and empties the buffer.
   */
  for (ring_buf_size_t t = 0; t < len; t++)
  {
    int y;
    int err = ring_buf_get_all(&buf, &y, sizeof(y));
    assert(err == 0);
    assert(data[t] == y);
    /*
     * Format as %zu for size_t type.
     */
    (void)printf("data[%zu] = %d\n", t, data[t]);
  }

  /*
   * Release the dynamically allocated memory.
   */
  free(data);

  return EXIT_SUCCESS;
}
