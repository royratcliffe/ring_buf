#pragma once

#include <errno.h>
#include <stddef.h>

typedef ptrdiff_t ring_buf_ptrdiff_t;

typedef size_t ring_buf_size_t;

#define RING_BUF_SIZE_MAX ((ring_buf_size_t)PTRDIFF_MIN)

/*!
 * \defgroup ring_buf_span Ring Buffer Span
 * \ingroup ring_buf_span
 * \{
 */

/*!
 * \details This structure needs to exist within the header.
 */
struct ring_buf_span {
  ring_buf_ptrdiff_t base, head, tail;
};

/*!
 * \}
 */

/*!
 * \brief Ring buffer instance.
 */
struct ring_buf {
  void *data;
  ring_buf_size_t size;
  struct ring_buf_span put, get;
};

static inline ring_buf_size_t ring_buf_used(const struct ring_buf *buf) {
  return buf->put.tail - buf->get.head;
}

static inline ring_buf_size_t ring_buf_free(const struct ring_buf *buf) {
  return buf->size - (buf->put.head - buf->get.tail);
}

/*!
 * \details Claims contiguous space. Advances the "put" head.
 */
ring_buf_size_t ring_buf_put_claim(struct ring_buf *buf, void **data, ring_buf_size_t size);

int ring_buf_put_finish(struct ring_buf *buf, ring_buf_size_t size);

ring_buf_size_t ring_buf_put(struct ring_buf *buf, const void *data, ring_buf_size_t size);

/*!
 * \details Advances the "get" head.
 */
ring_buf_size_t ring_buf_get_claim(struct ring_buf *buf, void **data, ring_buf_size_t size);

int ring_buf_get_finish(struct ring_buf *buf, ring_buf_size_t size);

ring_buf_size_t ring_buf_get(struct ring_buf *buf, void *data, ring_buf_size_t size);

#include <stdint.h>

#define RING_BUF_DECLARE(_name_, _size_)                                       \
  static uint8_t _ring_buf_data_##_name_[_size_];                              \
  struct ring_buf _name_ = {.data = _ring_buf_data_##_name_, .size = _size_}
