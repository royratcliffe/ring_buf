#include <assert.h>
#include <stdlib.h>

#include "ring_buf.h"

int ring_buf_size_max_test(int argc, char **argv) {
  (void)argc;
  (void)argv;
  assert(RING_BUF_SIZE_MAX == (size_t)PTRDIFF_MAX + 1);

  return EXIT_SUCCESS;
}
