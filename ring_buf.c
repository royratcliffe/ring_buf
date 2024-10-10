#include "ring_buf.h"

#include <memory.h>

static inline void ring_buf_clamp(size_t *clamp, size_t limit) {
  if (*clamp > limit)
    *clamp = limit;
}

/*!
 * \brief Head index of a span.
 * \details Used as the wrap size when claiming. The wrap size equals the head
 * relative to the base.
 */
static inline size_t ring_buf_span_head(const struct ring_buf_span *span) {
  return span->head - span->base;
}

static inline size_t ring_buf_span_tail(const struct ring_buf_span *span) {
  return span->tail - span->base;
}

static inline size_t ring_buf_span_claim(const struct ring_buf_span *span) {
  return span->head - span->tail;
}

/*!
 * \details Claims contiguous space. Advances the "put" head.
 */
ring_buf_size_t ring_buf_put_claim(struct ring_buf *buf, void **data, size_t size) {
  ring_buf_ptrdiff_t base = buf->put.base;
  ring_buf_size_t head = ring_buf_span_head(&buf->put);
  if (head >= buf->size) {
    base += buf->size;
    head -= buf->size;
  }
  ring_buf_clamp(&size, buf->size - head);
  ring_buf_clamp(&size, ring_buf_free(buf));
  if (data)
    *data = (uint8_t *)buf->data + (buf->put.head - base);
  buf->put.head += size;
  return size;
}

int ring_buf_put_finish(struct ring_buf *buf, size_t size) {
  ring_buf_size_t claim = ring_buf_span_claim(&buf->put);
  if (size > claim)
    return -EINVAL;
  buf->put.head = (buf->put.tail += size);
  if (ring_buf_span_tail(&buf->put) >= buf->size)
    buf->put.base += buf->size;
  return 0;
}

ring_buf_size_t ring_buf_put(struct ring_buf *buf, const void *data, size_t size) {
  ring_buf_size_t finish = 0U, claim;
  do {
    void *ptr;
    claim = ring_buf_put_claim(buf, &ptr, size);
    (void)memcpy(ptr, data, claim);
    *(const uint8_t **)&data += claim;
    finish += claim;
  } while (claim && (size -= claim));
  (void)ring_buf_put_finish(buf, finish);
  return finish;
}

/*!
 * \details Advances the "get" head.
 */
ring_buf_size_t ring_buf_get_claim(struct ring_buf *buf, void **data, size_t size) {
  ring_buf_ptrdiff_t base = buf->get.base;
  ring_buf_size_t head = ring_buf_span_head(&buf->get);
  if (head >= buf->size) {
    base += buf->size;
    head -= buf->size;
  }
  ring_buf_clamp(&size, buf->size - head);
  ring_buf_clamp(&size, ring_buf_used(buf));
  if (data)
    *data = (uint8_t *)buf->data + (buf->get.head - base);
  buf->get.head += size;
  return size;
}

int ring_buf_get_finish(struct ring_buf *buf, size_t size) {
  ring_buf_size_t claim = ring_buf_span_claim(&buf->get);
  if (size > claim)
    return -EINVAL;
  buf->get.head = (buf->get.tail += size);
  if (ring_buf_span_tail(&buf->get) >= buf->size)
    buf->get.base += buf->size;
  return 0;
}

ring_buf_size_t ring_buf_get(struct ring_buf *buf, void *data, size_t size) {
  ring_buf_size_t finish = 0U, claim;
  do {
    void *ptr;
    claim = ring_buf_get_claim(buf, &ptr, size);
    if (data) {
      (void)memcpy(data, ptr, claim);
      *(uint8_t **)&data += claim;
    }
    finish += claim;
  } while (claim && (size -= claim));
  (void)ring_buf_get_finish(buf, finish);
  return finish;
}
