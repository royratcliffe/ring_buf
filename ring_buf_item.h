#pragma once

#include "ring_buf.h"

typedef uint16_t ring_buf_item_size_t;

int ring_buf_item_put(struct ring_buf *buf, const void *data,
                      ring_buf_item_size_t size);

int ring_buf_item_get(struct ring_buf *buf, void *data,
                      ring_buf_item_size_t *size);
