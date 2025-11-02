#include "ring_buf_circ.h"
#include "ring_buf.h"

int ring_buf_put_circ(struct ring_buf *buf, void *data, size_t size) {
  if (ring_buf_is_full(buf))
    (void)ring_buf_get_ack(buf, ring_buf_get(buf, NULL, size));
  if (size > ring_buf_free_space(buf))
    return -EMSGSIZE;
  return ring_buf_put_ack(buf, ring_buf_put(buf, data, size));
}
