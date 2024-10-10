/*!
 * \file ring_buf.c
 * \copyright Roy Ratcliffe, Northumberland, United Kingdom
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge,  to any person obtaining a
 * copy  of  this  software  and    associated   documentation  files  (the
 * "Software"), to deal in  the   Software  without  restriction, including
 * without limitation the rights to  use,   copy,  modify,  merge, publish,
 * distribute, sublicense, and/or sell  copies  of   the  Software,  and to
 * permit persons to whom the Software is   furnished  to do so, subject to
 * the following conditions:
 *
 *     The above copyright notice and this permission notice shall be
 *     included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT  WARRANTY OF ANY KIND, EXPRESS
 * OR  IMPLIED,  INCLUDING  BUT  NOT   LIMITED    TO   THE   WARRANTIES  OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR   PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS  OR   COPYRIGHT  HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY,  WHETHER   IN  AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM,  OUT  OF   OR  IN  CONNECTION  WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "ring_buf.h"

#include <memory.h>

static inline void ring_buf_clamp(ring_buf_size_t *clamp, ring_buf_size_t limit) {
  if (*clamp > limit)
    *clamp = limit;
}

/*!
 * \brief Head index of a zone.
 * \details Used as the wrap size when claiming. The wrap size equals the head
 * relative to the base.
 */
static inline ring_buf_size_t ring_buf_zone_head(const struct ring_buf_zone *zone) {
  return zone->head - zone->base;
}

static inline ring_buf_size_t ring_buf_zone_tail(const struct ring_buf_zone *zone) {
  return zone->tail - zone->base;
}

static inline ring_buf_size_t ring_buf_zone_claim(const struct ring_buf_zone *zone) {
  return zone->head - zone->tail;
}

ring_buf_size_t ring_buf_put_claim(struct ring_buf *buf, void **data, ring_buf_size_t size) {
  ring_buf_ptrdiff_t base = buf->put.base;
  ring_buf_size_t head = ring_buf_zone_head(&buf->put);
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

int ring_buf_put_ack(struct ring_buf *buf, ring_buf_size_t size) {
  ring_buf_size_t claim = ring_buf_zone_claim(&buf->put);
  if (size > claim)
    return -EINVAL;
  buf->put.head = (buf->put.tail += size);
  if (ring_buf_zone_tail(&buf->put) >= buf->size)
    buf->put.base += buf->size;
  return 0;
}

ring_buf_size_t ring_buf_put(struct ring_buf *buf, const void *data, ring_buf_size_t size) {
  ring_buf_size_t ack = 0U, claim;
  do {
    void *put;
    claim = ring_buf_put_claim(buf, &put, size);
    (void)memcpy(put, data, claim);
    *(const uint8_t **)&data += claim;
    ack += claim;
  } while (claim && (size -= claim));
  return ack;
}

ring_buf_size_t ring_buf_get_claim(struct ring_buf *buf, void **data, ring_buf_size_t size) {
  ring_buf_ptrdiff_t base = buf->get.base;
  ring_buf_size_t head = ring_buf_zone_head(&buf->get);
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

int ring_buf_get_ack(struct ring_buf *buf, ring_buf_size_t size) {
  ring_buf_size_t claim = ring_buf_zone_claim(&buf->get);
  if (size > claim)
    return -EINVAL;
  buf->get.head = (buf->get.tail += size);
  if (ring_buf_zone_tail(&buf->get) >= buf->size)
    buf->get.base += buf->size;
  return 0;
}

ring_buf_size_t ring_buf_get(struct ring_buf *buf, void *data, ring_buf_size_t size) {
  ring_buf_size_t ack = 0U, claim;
  do {
    void *get;
    claim = ring_buf_get_claim(buf, &get, size);
    if (data) {
      (void)memcpy(data, get, claim);
      *(uint8_t **)&data += claim;
    }
    ack += claim;
  } while (claim && (size -= claim));
  return ack;
}
