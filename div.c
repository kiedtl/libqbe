#include "libqbe.h"

/*
 * Division method using only shifts, adds, and one multiply via himul.
 *
 * This approach was ~~shamelessly stolen~~ adapted from:
 *   Hoxey, Karim, Kay, Warren:
 *    The PowerPC's Compiler Writer's Guide
 *   pg 55-65
 *
 */

// Magic numbers {{{
#ifndef LIBQBE_NO_BUILTIN_MAGICS
#define UMAGICS_LEN 64
struct MagicSet umagics[UMAGICS_LEN] = {
	[ 2] = { 0x80000000, false, 0 },  [ 3] = { 0xaaaaaaab, false, 1 },
	[ 4] = { 0x40000000, false, 0 },  [ 5] = { 0xcccccccd, false, 2 },
	[ 6] = { 0xaaaaaaab, false, 2 },  [ 7] = { 0x24924925,  true, 3 },
	[ 8] = { 0x20000000, false, 0 },  [ 9] = { 0x38e38e39, false, 1 },
	[10] = { 0xcccccccd, false, 3 },  [11] = { 0xba2e8ba3, false, 3 },
	[12] = { 0xaaaaaaab, false, 3 },  [13] = { 0x4ec4ec4f, false, 2 },
	[14] = { 0x24924925,  true, 4 },  [15] = { 0x88888889, false, 3 },
	[16] = { 0x10000000, false, 0 },  [17] = { 0xf0f0f0f1, false, 4 },
	[18] = { 0x38e38e39, false, 2 },  [19] = { 0xaf286bcb,  true, 5 },
	[20] = { 0xcccccccd, false, 4 },  [21] = { 0x86186187,  true, 5 },
	[22] = { 0xba2e8ba3, false, 4 },  [23] = { 0xb21642c9, false, 4 },
	[24] = { 0xaaaaaaab, false, 4 },  [25] = { 0x51eb851f, false, 3 },
	[26] = { 0x4ec4ec4f, false, 3 },  [27] = { 0x2f684bdb,  true, 5 },
	[28] = { 0x24924925,  true, 5 },  [29] = { 0x8d3dcb09, false, 4 },
	[30] = { 0x88888889, false, 4 },  [31] = { 0x08421085,  true, 5 },
	[32] = { 0x08000000, false, 0 },  [33] = { 0x3e0f83e1, false, 3 },
	[34] = { 0xf0f0f0f1, false, 5 },  [35] = { 0xd41d41d5,  true, 6 },
	[36] = { 0x38e38e39, false, 3 },  [37] = { 0xbacf914d,  true, 6 },
	[38] = { 0xaf286bcb,  true, 6 },  [39] = { 0xa41a41a5,  true, 6 },
	[40] = { 0xcccccccd, false, 5 },  [41] = { 0xc7ce0c7d, false, 5 },
	[42] = { 0x86186187,  true, 6 },  [43] = { 0x2fa0be83, false, 3 },
	[44] = { 0xba2e8ba3, false, 5 },  [45] = { 0x6c16c16d,  true, 6 },
	[46] = { 0xb21642c9, false, 5 },  [47] = { 0xae4c415d, false, 5 },
	[48] = { 0xaaaaaaab, false, 5 },  [49] = { 0x5397829d, false, 4 },
	[50] = { 0x51eb851f, false, 4 },  [51] = { 0xa0a0a0a1, false, 5 },
	[52] = { 0x4ec4ec4f, false, 4 },  [53] = { 0x3521cfb3,  true, 6 },
	[54] = { 0x2f684bdb,  true, 6 },  [55] = { 0x29e4129f,  true, 6 },
	[56] = { 0x24924925,  true, 6 },  [57] = { 0x1f7047dd,  true, 6 },
	[58] = { 0x8d3dcb09, false, 5 },  [59] = { 0x22b63cbf, false, 3 },
	[60] = { 0x88888889, false, 5 },  [61] = { 0x4325c53f, false, 4 },
	[62] = { 0x08421085,  true, 6 },  [63] = { 0x04104105,  true, 6 },
};
#endif
// }}}

u32
__divu32_magic(u32 dvd, u32 dvs, u32 magic, bool a, u8 shift)
{
	if (dvd == 0)       return 0;
	if (dvs == 1)       return dvd;
	if (dvd == dvs)     return 1;
	if (dvd == dvs + 1) return 1;
	if (dvd < dvs)      return 0;

	u32 q = __himulu32(magic, dvd);
	return a == 0 ? q >> shift
		      : (((dvd - q) >> 1) + q) >> (shift - 1);
}

u32
__divmodu32(u32 dvd, u32 dvs, u32 *rem_dest)
{
	/* Disabled, need to populate rem_dest! */

	/* if (dvd == 0)       return 0; */
	/* if (dvs  == 1)      return dvd; */
	/* if (dvd == dvs)     return 1; */
	/* if (dvd == dvs + 1) return 1; */
	/* if (dvd < dvs)      return 0; */

	u32 bit = 1;
	while (dvs < dvd && bit && (dvs & (1 << 31)) == 0) {
		dvs <<= 1;
		bit <<= 1;
	}

	u32 rem = dvd;
	u32 res = 0;

	while (bit != 0) {
		if (rem >= dvs) {
			rem -= dvs;
			res |= bit;
		}

		bit >>= 1;
		dvs >>= 1;
	}

	if (rem_dest != NULL)
		*rem_dest = rem;
	return res;
}


u32
__modu32(u32 dvd, u32 dvs)
{
	u32 dest = -1;
	(void)__divmodu32(dvd, dvs, &dest);
	return dest;
}



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
		if (!test(eq_u32, dvd /  10, __divu32_10(dvd)))
			break;
	}
	TEST_END();
}

void
div_5_test_exhaustive(void)
{
	TEST_BEGIN();
	for (u32 dvd = 0; dvd < 0xFFFFF; ++dvd) {
		if (!test(eq_u32, dvd /  5, __divu32_5(dvd)))
			break;
	}
	TEST_END();
}

void
divmod_test_exhaustive(void)
{
	TEST_BEGIN();
	for (u32 dvd = 0xFFF; dvd > 0; --dvd) {
		for (u32 dvs = 1; dvs < 0xFFF; ++dvs) {
			u32 rem = 0;
			if (!test(eq_u32, dvd / dvs, __divmodu32(dvd, dvs, &rem))
			||  !test(eq_u32, dvd % dvs, rem))
				goto End;
		}
	}
End:
	TEST_END();
}

void
divu32_magic_test_simple(void)
{
	TEST_BEGIN();

	struct { u32 a; u32 b; } cases[] = {
		{   9,   3 },
		{  10,   2 },
		{  20,   5 },
		{ 100,  10 },
		{ 128,   7 },
	};

	for (u32 i = 0; i < ARRAY_LEN(cases); ++i) {
		struct MagicSet m = umagics[cases[i].b];
		test(eq_u32,
			cases[i].a / cases[i].b,
			__divu32_magic(cases[i].a, cases[i].b, m.m, m.a, m.s)
		);
	}

	TEST_END();
}

void
divu32_magic_test_exhaustive(void)
{
	TEST_BEGIN();

	for (u32 dvd = 0xFF; dvd > 0; --dvd) {
		for (u32 dvs = 1; dvs < UMAGICS_LEN; ++dvs) {
			struct MagicSet m = umagics[dvs];
			u32 got = __divu32_magic(dvd, dvs, m.m, m.a, m.s);
			if (!test(eq_u32, dvd / dvs, got))
				goto End;
		}
	}

End:
	TEST_END();
}

TestFunc div_test_funcs[] = {
	&div_10_test_exhaustive,
	&div_5_test_exhaustive,
	&divmod_test_exhaustive,
	&divu32_magic_test_simple,
	&divu32_magic_test_exhaustive,
};
#endif /* }}} */
