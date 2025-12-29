#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "ring_buf_yield.h"

static int yield_putchar(void *space, int index, void *extra)
{
  fprintf((FILE *)extra, "%d %c\n", index, *(const char *)space);
  return -EAGAIN;
}

int ring_buf_yield_test(int argc, char **argv)
{
  (void)argc;
  (void)argv;
  int rc = EXIT_SUCCESS;

  {
    RING_BUF_DEFINE_STATIC(buf, 64U);
    for (const char *p = "Hello, World!"; *p != '\0'; p++)
      assert(ring_buf_put_ack(&buf, ring_buf_put(&buf, p, sizeof(*p))) == 0);
    assert(ring_buf_get_claim_yield(&buf, sizeof(char), yield_putchar, stdout) == 13);
  }

  return rc;
}
