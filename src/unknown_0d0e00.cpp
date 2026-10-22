// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0D0E00.CPP: ending a unit's timed state (an outside function lane
   A's unit script functions need) */

#include "unknown_11c920.h"
#include "globals.h"

struct s_unit_0d0e00
{
	byte unknown000[0xd4];
	long index_d4;
	byte unknown0d8[0x134 - 0xd8];
	dword flags;
	byte unknown138[0x2b8 - 0x138];
	real rate;
};

struct s_object_header_0d0e00
{
	byte unknown00[8];
	s_unit_0d0e00 *object;
};

#define UNIT_GET_0D0E00(index) (((s_object_header_0d0e00 *)g_4e0300->data)[(index) & 0xffff].object)

void function_b58c0(long index, dword mask);

/* sets or clears one bit of a flags word */
static inline void flags_set_bit(dword *flags, long bit, bool value)
{
	if (value)
		*flags |= 1 << bit;
	else
		*flags &= ~(1 << bit);
}

// @retail 0xd0e00
void function_d0e00(long unit_index, real rate)
{
	s_unit_0d0e00 *unit = UNIT_GET_0D0E00(unit_index);
	flags_set_bit(&unit->flags, 3, false);
	flags_set_bit(&unit->flags, 4, false);
	unit->rate = rate * 0.25f;

	unit = UNIT_GET_0D0E00(unit_index);
	if (unit->index_d4 != NONE)
		function_b58c0(unit->index_d4, 0x800000);
}
