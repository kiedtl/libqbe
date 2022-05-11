#include "libqbe.h"
#include <stdio.h>

/* Taken from this SO answer:
 *
 * https://stackoverflow.com/a/19076173
 */
u32
__divu32_10(u32 A)
{
	u32 Q, R;
	Q = (A >> 1) + (A >> 2);           // Q = (A / 2) + (A / 4)
	Q = Q + (Q >> 4);                  // Q = Q + (Q /    16)
	Q = Q + (Q >> 8);                  // Q = Q + (Q /   256)
	Q = Q + (Q >> 16);                 // Q = Q + (Q / 65536)
	Q = Q >> 3;                        // Q = Q / 8
	R = A - (((Q << 2) + Q) << 1);     // R = A - (Q * 10) = (((Q * 4) + Q) * 2)
	return Q + (R >= 10);
}

/*
 * Modified from divu10.
 *
 * FIXME: this was modified in a lazy way, could be better.
 * For example, the initial approximation (Q = (a/2) + (a/4)) could possibly
 * be simplified to (Q = (a/4)). More research needed.
 */
u32
__divu32_5(u32 A)
{
	u32 Q, R;
	Q = (A >> 1) + (A >> 2);           // Q = (A / 2) + (A / 4)
	Q = Q + (Q >> 4);                  // Q = Q + (Q /    16)
	Q = Q + (Q >> 8);                  // Q = Q + (Q /   256)
	Q = Q + (Q >> 16);                 // Q = Q + (Q / 65536)
	Q = Q >> 2;                        // Q = Q / 4
	R = A - ((Q << 2) + Q);            // R = A - (Q * 5) = ((Q * 4) + Q)


	/* Unrolled loop of:
	 *     while (R >= 5) Q += 1, R -= 5;
	 * since we'll only need two iterations, max.
	 */
	if (R >= 5) Q += 1, R -= 5;
	if (R >= 5) Q += 1;

	return Q;
}

#ifdef TEST /* {{{ */
void
div_10_test_exhaustive(void)
{
	TEST_BEGIN();
	for (u32 dvd = 0; dvd < 0xFFFFF; ++dvd) {
		test(eq_u32, dvd / 10, __divu32_10(dvd));
	}
	TEST_END();
}

void
div_5_test_exhaustive(void)
{
	TEST_BEGIN();
	for (u32 dvd = 0; dvd < 0xFFFFF; ++dvd) {
		test(eq_u32, dvd /  5, __divu32_5(dvd));
	}
	TEST_END();
}

TestFunc div_test_funcs[] = {
	&div_10_test_exhaustive,
	&div_5_test_exhaustive,
};
#endif /* }}} */
