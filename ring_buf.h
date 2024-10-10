/*!
 * \file ring_buf.h
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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>

typedef ptrdiff_t ring_buf_ptrdiff_t;

typedef size_t ring_buf_size_t;

#define RING_BUF_SIZE_MAX ((ring_buf_size_t)PTRDIFF_MIN)

/*!
 * \defgroup ring_buf_zone Ring Buffer Zone
 * \ingroup ring_buf_zone
 * \{
 */

/*!
 * \details This structure needs to exist within the header.
 */
struct ring_buf_zone {
  ring_buf_ptrdiff_t base, head, tail;
};

/*!
 * \}
 */

/*!
 * \defgroup ring_buf Ring Buffer
 * \ingroup ring_buf
 * \{
 */

/*!
 * \brief Ring buffer instance.
 */
struct ring_buf {
  void *data;
  ring_buf_size_t size;
  struct ring_buf_zone put, get;
};

static inline ring_buf_size_t ring_buf_used(const struct ring_buf *buf) {
  return buf->put.tail - buf->get.head;
}

static inline bool ring_buf_is_empty(const struct ring_buf *buf) {
  return ring_buf_used(buf) == 0U;
}

static inline ring_buf_size_t ring_buf_free(const struct ring_buf *buf) {
  return buf->size - (buf->put.head - buf->get.tail);
}

static inline bool ring_buf_is_full(const struct ring_buf *buf) {
  return ring_buf_free(buf) == 0U;
}

/*!
 * \brief Claims space for putting data into a ring buffer.
 * \details Claims contiguous space. Advances the "put" head.
 */
ring_buf_size_t ring_buf_put_claim(struct ring_buf *buf, void **data,
                                   ring_buf_size_t size);

/*!
 * \brief Acknowledges space claimed for putting data into a ring buffer.
 * \param buf Ring buffer address.
 * \param size Number of bytes to acknowledge.
 * \returns 0 Successful put.
 * \returns -EINVAL Size exceeds previously claimed aggregate space.
 */
int ring_buf_put_ack(struct ring_buf *buf, ring_buf_size_t size);

/*!
 * \brief Puts non-contiguous bytes into the ring buffer.
 * \details The return value may be less than the given size if the buffer runs
 * out of free space.
 * \note Does \e not automatically acknowledge the space.
 * \param buf Ring buffer.
 * \param data Address of bytes to put.
 * \param size Number of bytes to put.
 * \returns Buffer space to acknowledge in bytes.
 */
ring_buf_size_t ring_buf_put(struct ring_buf *buf, const void *data,
                             ring_buf_size_t size);

/*!
 * \brief Claims contiguous bytes for getting.
 * \details Advances the "get" head.
 */
ring_buf_size_t ring_buf_get_claim(struct ring_buf *buf, void **data,
                                   ring_buf_size_t size);

int ring_buf_get_ack(struct ring_buf *buf, ring_buf_size_t size);

/*!
 * \brief Gets data from a ring buffer.
 * \details Copies discontinuous data.
 * \param data Address of copied data, or \c NULL to ignore.
 * \param size Number of bytes to get.
 * \returns Number of bytes to acknowledge.
 */
ring_buf_size_t ring_buf_get(struct ring_buf *buf, void *data,
                             ring_buf_size_t size);

#include <stdint.h>

#define RING_BUF_DECLARE(_name_, _size_)                                       \
  static uint8_t _ring_buf_data_##_name_[_size_];                              \
  struct ring_buf _name_ = {.data = _ring_buf_data_##_name_, .size = _size_}

/*!
 * \}
 */

#ifdef __cplusplus
}
#endif
