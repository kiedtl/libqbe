#include "libqbe.h"

/*
 * Multiply a * b, returning the high word of the product without overflowing.
 * Very similar to PowerPC's mulhwu instruction.
 *
 */
u32
__himulu32(u32 a, u32 b)
{
	u32 a_hi = HI16(a), a_lo = LO16(a);
	u32 b_hi = HI16(b), b_lo = LO16(b);

	/*
	 * u64 res = a_lo * b_lo
	 *         + (((u64)a_hi * (u64)b_lo) << 16)
	 *         + (((u64)a_lo * (u64)b_hi) << 16)
	 *         + (((u64)a_hi * (u64)b_hi) << 32);
	 */

	u32 i1 = a_hi * b_hi; // << 32
	u32 i2 = a_lo * b_hi; // << 16
	u32 i3 = a_hi * b_lo; // << 16
	u32 i4 = a_lo * b_lo;

	/*
	 * Add each 16-bit chunk one at a time to avoid overflow.
	 *
	 * u32 res = (((((i4 >> 16) + i3) + i2) >> 16) + i1);
	 *
	 */

	u32 j1 = (i4 >> 16) + i3;

	u32 j2_hi = HI16(j1) + HI16(i2);
	u16 j2_lo = LO16(j1) + LO16(i2);
	if (j2_lo < LO16(j1) || j2_lo < LO16(i2))
		++j2_hi;

	u32 j3 = (u32)j2_hi + i1;

	return j3;
}

#ifdef TEST
void
himul_test_misc(void)
{
	TEST_BEGIN();

	struct { u32 a; u32 b; } cases[] = {
		{     0xFFFF,     0xFFFF },
		{   18481382,    1925527 },
		{   19435827, 3290874132 },
		{ 0xFFFFFFFF, 0xFFFFFFFF },
	};

	for (u32 i = 0; i < ARRAY_LEN(cases); ++i) {
		u32 expect = (u32)(((u64)cases[i].a * (u64)cases[i].b) >> 32);
		test(eq_u32, expect, __himulu32(cases[i].a, cases[i].b));
	}

	TEST_END();
}

void
himul_test_exhaustive(void)
{
	TEST_BEGIN();

	u32 U32_MAX = 0xFFFFFFFF;
	for (u32 a = U32_MAX; a > U32_MAX - 0xFFF; --a) {
		for (u32 b = U32_MAX; b > U32_MAX - 0xFFF; --b) {
			u32 expected = ((u64)a * (u64)b) >> 32;
			u32 got = __himulu32(a, b);
			test(eq_u64, expected, got);
		}
	}

	TEST_END();
}

TestFunc himul_test_funcs[] = {
	&himul_test_misc,
	&himul_test_exhaustive,
};
#endif
