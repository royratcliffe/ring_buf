/*!
 * \file ring_buf_item.h
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

#include "ring_buf.h"

/*!
 * \brief Size of the ring buffer's item length.
 * \details The length is the size of the item in bytes.
 */
typedef uint16_t ring_buf_item_size_t;

/*!
 * \details Puts an item's length and byte content.
 * \returns Error code; \c 0 on success, negative on error. Returns \c -EMSGSIZE
 * has insufficient space to put the data.
 */
int ring_buf_item_put(struct ring_buf *buf, const void *data,
                      ring_buf_item_size_t size);

int ring_buf_item_get(struct ring_buf *buf, void *data,
                      ring_buf_item_size_t *size);

#ifdef __cplusplus
}
#endif
