#include "libqbe.h"

void
__add64u32(u32 a_hi, u32 a_lo, u32 b_hi, u32 b_lo, u32 *dst_hi, u32 *dst_lo)
{
	*dst_hi = a_hi + b_hi;
	*dst_lo = a_lo + b_lo;

	if (*dst_lo < a_lo || *dst_lo < b_lo)
		++(*dst_hi);
}

#ifdef TEST /* {{{ */
void
add64_test_misc(void)
{
	TEST_BEGIN();

	struct { u64 a; u64 b; } cases[] = {
		{          1,          1 },
		{   19435827, 3290874132 },
		{ 0xFFFFFFFF, 0xFFFFFFFF },
	};

	for (u32 i = 0; i < ARRAY_LEN(cases); ++i) {
		u64 expect = cases[i].a + cases[i].b;

		u32 res_hi = 0;
		u32 res_lo = 0;
		__add64u32(
			HI32(cases[i].a), LO32(cases[i].a),
			HI32(cases[i].b), LO32(cases[i].b),
			&res_hi, &res_lo
		);
		u64 got = (u64)res_hi << 32 | (u64)res_lo;

		test(eq_u64, expect, got);
	}

	TEST_END();
}

void
add64_test_exhaustive(void)
{
	TEST_BEGIN();

	const u64 U32_MAX = 0xFFFFFFFF;
	for (u64 a = U32_MAX; a < U32_MAX + 0xFF; ++a) {
		for (u64 b = U32_MAX; b < U32_MAX + 0xFF; ++b) {
			u32 res_hi = 0;
			u32 res_lo = 0;
			__add64u32(HI32(a), LO32(a), HI32(b), LO32(b),
					&res_hi, &res_lo);
			u64 got = (u64)res_hi << 32 | (u64)res_lo;
			test(eq_u64, a + b, got);
		}
	}

	TEST_END();
}

TestFunc add_test_funcs[] = {
	&add64_test_misc,
	&add64_test_exhaustive,
};
#endif /* }}} */
