/*
 * Ring Buffer Module - Homework Test Skeleton
 *
 * test_fresh_state is provided as a worked example. Fill in the remaining
 * 7 ZTEST bodies according to TEST_SPEC.md. Stubs call ztest_test_skip()
 * so the binary builds and runs cleanly before each test is implemented.
 *
 * Run:
 *   west twister -T tests/ring_buf -p native_sim
 */

#include <errno.h>
#include <zephyr/ztest.h>

#include "ring_buf.h"

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state. Capacity 4 is
 * enough to exercise FIFO order (push 1, 2, 3) and overflow (full at 4).
 */
static void before(void *f) {
  ARG_UNUSED(f);
  rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 *
 * Initial state and re-initialization behaviour.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — study this test before writing the rest. */
ZTEST(ring_buf_init, test_fresh_state) {
  zassert_true(rb_is_empty(), "Fresh buffer must be empty");
  zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state) {
  zassert_equal(rb_push(99), 0, "Push value 99 into buffer");
  zassert_equal(rb_init(4), 0, "Init buffer");
  zassert_true(rb_is_empty(), "Reinit buffer must be empty");
  zassert_equal(rb_count(), 0, "Reinit buffer count must be 0");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 *
 * Single push/pop round-trip, FIFO order, full error path.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop) {
  zassert_equal(rb_push(42), 0, "Push value 42 into buffer");
  int v;
  zassert_equal(rb_pop(&v), 0, "Pop value 42");
  zassert_equal(v, 42, "Check value returned = 42");
  zassert_true(rb_is_empty(), "Check if buffer is empty");
}

ZTEST(ring_buf_push_pop, test_fifo_order) {
  zassert_equal(rb_push(1), 0, "Push value 1 into buffer");
  zassert_equal(rb_push(2), 0, "Push value 2 into buffer");
  zassert_equal(rb_push(3), 0, "Push value 3 into buffer");
  int v;
  zassert_equal(rb_pop(&v), 0, "Pop value 1");
  zassert_equal(v, 1, "Check value returned = 1");
  zassert_equal(rb_pop(&v), 0, "Pop value 2");
  zassert_equal(v, 2, "Check value returned = 2");
  zassert_equal(rb_pop(&v), 0, "Pop value 3");
  zassert_equal(v, 3, "Check value returned = 3");
  zassert_true(rb_is_empty(), "Check if buffer is empty");
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc) {
  zassert_equal(rb_push(1), 0, "Push value 1 into buffer");
  zassert_equal(rb_push(2), 0, "Push value 2 into buffer");
  zassert_equal(rb_push(3), 0, "Push value 3 into buffer");
  zassert_equal(rb_push(4), 0, "Push value 4 into buffer");
  zassert_true(rb_is_full(), "Check if buffer is full");
  zassert_equal(rb_push(99), -ENOSPC, "Check if buffer full");
  zassert_equal(rb_count(), 4, "Check 4 elements into FIFO");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 *
 * Peek semantics and NULL-pointer boundary conditions.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume) {
  zassert_equal(rb_push(7), 0, "Push value 7 into buffer");
  int v;
  zassert_equal(rb_peek(&v), 0, "Peek the value 7");
  zassert_equal(v, 7, "Check value returned = 7");
  zassert_equal(rb_peek(&v), 0, "Peek the value 7 again");
  zassert_equal(v, 7, "Check value returned = 7");
  zassert_equal(rb_count(), 1, "Check 1 element into FIFO");
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval) {
  zassert_equal(rb_pop(NULL), -EINVAL, "Pop an empty FIFO is an error");
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill) {
  zassert_equal(rb_push(1), 0, "Push value 1 into buffer");
  zassert_equal(rb_push(2), 0, "Push value 2 into buffer");
  zassert_equal(rb_push(3), 0, "Push value 3 into buffer");
  zassert_equal(rb_push(4), 0, "Push value 4 into buffer");
  zassert_true(rb_is_full(), "Check if buffer is full");
  zassert_equal(rb_count(), 4, "Check 4 elements into FIFO");
}
