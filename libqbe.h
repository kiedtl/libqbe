#ifndef LIBQBE_H
#define LIBQBE_H

#ifndef NULL
#define NULL 0
#endif

#define false 0
#define true  1

#define ARRAY_LEN(a) (sizeof(a) / sizeof((a)[0]))

#define HI16(x) ((x) >> 16)
#define LO16(x) ((x) & 0xFFFF)
#define HI32(x) ((x) >> 32)
#define LO32(x) ((x) & 0xFFFFFFFF)

typedef unsigned char       u8;
typedef unsigned char       bool;
typedef unsigned long long  u64;
typedef unsigned int        u32;
typedef unsigned short      u16;
typedef          int        i32;

struct S64 {
	u32 hi; u32 lo;
};

/* Magic number set for use with __divu32_magic() and friends.
 *
 * Taken from:
 * The PowerPC's Compiler Writer's Guide, pg 55-65
 *
 */
struct MagicSet {
	u32 m;   /* Magic number (multiplier) */
	bool a;  /* Add indicator */
	u8 s;    /* Shift amount */
};


/* Naming convention:
 *
 * __<OPERATION><TYPE><WIDTH>
 *
 * TYPE:  either `s` for signed or `u` for unsigned
 * WIDTH: 16, 32, 64
 */

struct S64 __add64u32(u32 a_hi, u32 a_lo, u32 b_hi, u32 b_lo);
u32 __himulu32(u32 a, u32 b);
u32 __divu32_10(u32 dvd);
u32 __divu32_5(u32 dvd);
u32 __divu32_magic(u32 dvd, u32 dvs, u32 magic, bool a, u8 shift);
u32 __divmodu32(u32 dvd, u32 dvs, u32 *rem_dest);
u32 __modu32(u32 dvd, u32 dvs);

#endif
