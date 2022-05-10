#include <stdio.h>
#include "libqbe.h"

/*
 * This file contains utility functions for dealing with the MagicSets used
 * with __divu32_magic() and friends.
 *
 * It is NOT intended for calling from a compiled program.
 *
 * This method was adapted from:
 * The PowerPC's Compiler Writer's Guide, pg 55-65
 */

/*
 * Attempt to find a reduced divisor such that
 *     reduced = 2^k / dvs
 *  && reduced mod 2 == 1
 *
 * i.e.
 *     dvs = reduced * 2^k, reduced mod 2 = 1
 *
 * The new reduced divisor can be used to calculate
 * the same magic number/shift/etc.
 *
 * TODO: test this function.
 * TODO: can this be done recursively? (probably not)
 *
 */
bool
_reducedvs32(u32 dvs, u32 *dst_reduced, u32 *dst_k)
{
	/*
	 * Search backwards to find larger reductions
	 * if possible
	 */
	for (u32 k = 30; k > 1; --k) {
		u32 n = 2 << k;
		u32 b = dvs / n;
		if (b > 1 && (b % 2) == 1) {
			*dst_reduced = b;
			*dst_k = k;
			return true;
		}
	}
	return false;
}


/*
 * Calculate the magic numbers for a given divisor.
 *
 * It is expected that the compiler will generate the needed magic numbers at
 * compile-time and emit the necessary calls to __divu32_magic.
 *
 * Precondition: 1 < d <= 2^32-1
 */
struct MagicSet
umagiccalc(u32 dvs)
{
	struct MagicSet m = {0};

	m.a = 0;
	u32 nc = - 1 - (-dvs) % dvs;
	i32 p = 31;

	u32 q1 = 0x80000000 / nc;             /* q1 = 2^p/nc */
	u32 r1 = 0x80000000 - (q1 * nc);      /* r1 = rem(2^p,nc) */
	u32 q2 = 0x7FFFFFFF / dvs;            /* q2 = (2^p-1)/d */
	u32 r2 = 0x7FFFFFFF - (q2 * dvs);     /* r2 = rem((2^p-1),d) */

	u32 delta = 0;

	do {
		++p;

		if (r1 >= nc - r1) {
			q1 = 2 * q1 + 1;
			r1 = 2 * r1 - nc;
		} else {
			q1 = 2 * q1;
			r1 = 2 * r1;
		}

		if (r2 + 1 >= dvs - r2) {
			if (q2 >= 0x7FFFFFFF) {
				m.a = 1;
			}
			q2 = 2 * q2 + 1;
			r2 = 2 * r2 + 1 - dvs;
		} else {
			if (q2 >= 0x80000000) {
				m.a = 1;
			}

			q2 = 2 * q2;
			r2 = 2 * r2 + 1;
		}

		delta = dvs - 1 - r2;
	} while (p < 64 && (q1 < delta || (q1 == delta && r1 == 0)));

	m.m = q2 + 1;
	m.s = p - 32;

	return m;
}

/*
 * Print a selection of some magic constants for including in a compiled
 * program.
 */
void
generate_magics(u32 until, FILE *f)
{
	for (u32 i = 2; i < until; ++i) {
		struct MagicSet m = umagiccalc(i);
		fprintf(f, "%c", i % 2 == 1 ? ' ' : '\t');
		fprintf(f, "[%2i] = { 0x%08x, %5s, %1u },",
				i, m.m, m.a ? "true" : "false", m.s);
		fprintf(f, "%c", i % 2 == 1 ? '\n' : ' ');
	}
}
