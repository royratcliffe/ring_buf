#include "ring_buf_item.h"

int ring_buf_item_put(struct ring_buf *buf, const void *data,
                      ring_buf_item_size_t size) {
  if (sizeof(size) + size > ring_buf_free(buf))
    return -EMSGSIZE;
  const ring_buf_size_t claim = ring_buf_put(buf, &size, sizeof(size));
  return ring_buf_put_ack(buf, claim + ring_buf_put(buf, data, size));
}

/*
 * The claim cannot fail since a previous put succeeded. Do not mix item-based
 * puts with plain puts.
 */
int ring_buf_item_get(struct ring_buf *buf, void *data,
                      ring_buf_item_size_t *size) {
  if (ring_buf_is_empty(buf))
    return -EAGAIN;
  const ring_buf_size_t claim = ring_buf_get(buf, size, sizeof(*size));
  return ring_buf_get_ack(buf, claim + ring_buf_get(buf, data, *size));
}
