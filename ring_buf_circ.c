#include "ring_buf_circ.h"
#include "ring_buf.h"

/*
 * Add a new floating-point scalar to the ring buffer. If the circular buffer is
 * full, remove the oldest scalar first.
 *
 * Fail if the new data will not fit. This should not happen if the buffer is
 * sized correctly. It will never happen if the buffer size is a multiple of the
 * scalar data size.
 */
int ring_buf_put_circ(struct ring_buf *buf, void *data, size_t size) {
  if (ring_buf_is_full(buf))
    (void)ring_buf_get_ack(buf, ring_buf_get(buf, NULL, size));
  if (size > ring_buf_free_space(buf))
    return -EMSGSIZE;
  return ring_buf_put_ack(buf, ring_buf_put(buf, data, size));
}
